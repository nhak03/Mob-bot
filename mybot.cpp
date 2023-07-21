#include <dpp/dpp.h>
#include<fstream>
#include<vector>
#include<iostream>
#include<random>
#include<functional>
#include "src/Dictionary.h"
#include "src/commons.h"
#include "src/action_work.h"
#include "src/action_buy.h"

#include <thread>
using namespace std;

const std::string    BOT_TOKEN    = "NjkyMDc0OTI3MjQ2OTM0MDM3.XnpPAA.ArATND3efgNt0OSJysPlnE94l2g";
 
// messy so far, just testing out certain methods before cleaning up and writing real functions
// https://dpp.dev/build-a-discord-bot-windows-wsl.html

// Function to write dictionary to a file
void writeDictionaryToFile(const Dictionary& dict) {
    std::ofstream file;
    try{
        file.open("userData.txt");
        std::string data = dict.to_string();
        file << data;
        file.close();
    }catch(logic_error& e){
        // Handle file opening error
        std::cout << "couldn't find file" << endl;
    }
}

// Function to run the dictionary writing task every 15 seconds
void writeTask(const Dictionary& dict) {
    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(15));
        writeDictionaryToFile(dict);
        cout << "Did a file save" << endl;
    }
}

int main() {

    ifstream inputFile;
    size_t begin, end, len;
    string line;
    string tokenBuffer;
    string token;
    string delim = " \t\\\"\',<>/?;:[{]}|`~!@%#$^&*()_=+";
    Dictionary userDict;

    try{
        inputFile.open("userData.txt", ios::in);
        string first;
        bool first_Set = false;
        while(getline(inputFile, line)){
            len = line.length();
            // cout << "Processing this line: " << line << endl;
            // get first token
            begin = min(line.find_first_not_of(delim, 0), len);
            end   = min(line.find_first_of(delim, begin), len);
            token = line.substr(begin, end-begin);
            if(first_Set == false){
                first = token;
                first_Set = true;
            }
            int index = 0;
            valType* inventory; 
            while( token!="" ){  // we have a token
                try{
                    double doub = std::stod(token);
                    // cout << doub << " assoc w/ " << first << endl;
                    if(doub != val_hold){
                        inventory[index] = doub;
                        index++;
                    }
                }catch(logic_error& e){
                    // cout << "User " << token << ": ";
                    userDict.setValue(token, val_hold);
                    inventory = userDict.getArray(token);
                    // -3.14 is our junk holder data
                    // first token is a username, do not add
                    for(int i=0; i<20; i++){
                        // this for loop zeroes out the whole array
                        // that way, even if the lines have less tokens than indexes
                        // each index gets set to 0 properly, that way on file write
                        // each line will have the proper amount of tokens
                        inventory[i] = 0;
                    }
                }
            begin = min(line.find_first_not_of(delim, end+1), len);
            end   = min(line.find_first_of(delim, begin), len);
            token = line.substr(begin, end-begin);
            }
            first_Set = false; 
        }
        inputFile.close();
    }catch(logic_error& e){
        std::cout << "File not found" << endl;
    }

    // cout << "Reading the dictionary" << endl;
    // cout << userDict << endl;

    dpp::cluster bot(BOT_TOKEN);
 
    bot.on_log(dpp::utility::cout_logger());
 
    bot.on_slashcommand([&userDict](const dpp::slashcommand_t& event) {
        // each thing will call some function 

        if(event.command.get_command_name() == "bal"){
            dpp::user who = event.command.get_issuing_user();
            std::string msg = action_bal(userDict, who.username, who.get_mention());
            event.reply(msg);
        }

        if(event.command.get_command_name() == "inventory"){
            dpp::user who = event.command.get_issuing_user();
            if(who.username == "dajujumaster"){
                // if admin param is selected, try to find the user
                std::variant<monostate, string, long int, bool, dpp::snowflake, double> viewUser = event.get_parameter("admin");
                try{
                    // if succeeded we lookup their inventory
                    dpp::snowflake temp = std::get<dpp::snowflake>(viewUser);
                    who = event.command.get_resolved_user(temp);
                }catch(const std::bad_variant_access& ex){
                    // if fail, just look at our own
                    who = event.command.get_issuing_user();
                }
            }
            std::string msg = action_inventory(userDict, who.username, who.get_mention());
            event.reply(msg);
        }

        if(event.command.get_command_name() == "work"){
            dpp::user who = event.command.get_issuing_user();
            std::string labor_type;
            std::variant<monostate, string, long int, bool, dpp::snowflake, double> laborVariant = event.get_parameter("labor");
            try{
                labor_type = std::get<std::string>(laborVariant);
            }catch(const std::bad_variant_access& ex){
                labor_type = "default";
            }

            std::string msg = action_work(userDict, who.username, who.get_mention(), labor_type);
            event.reply(msg);
        }

        if(event.command.get_command_name() == "buy"){
            std::string param = std::get<std::string>(event.get_parameter("item")); // reading parameter type
            std::int64_t amount = 1;
            try{
                amount = std::get<std::int64_t>(event.get_parameter("amount")); // reading parameter amount
            }catch(logic_error& e){
                // do nothing since amount initialized to 1
            }
            dpp::user who = event.command.get_issuing_user();
            std::string msg = action_buy(userDict, who.username, who.get_mention(), param, amount);
            event.reply(msg);
        }

        if(event.command.get_command_name() == "pay"){
            dpp::user sender = event.command.get_issuing_user();
            double amount = std::get<double>(event.get_parameter("amount"));
            std::variant<monostate, string, long int, bool, dpp::snowflake, double> recieve = event.get_parameter("user");
            dpp::user reciever;
            try{ // this try and catch block is to find the user to send money to
                // try to get a user obj
                dpp::snowflake temp = std::get<dpp::snowflake>(recieve);
                reciever = event.command.get_resolved_user(temp);
            }catch(const std::bad_variant_access& ex){
                // if fail, just look at our own
                // send fail message
                event.reply("Could not find that user to send money to.");
                return;
            }

            std::string msg = action_pay(userDict, sender.username, reciever.username, amount, reciever.get_mention());
            event.reply(msg);
        }
        


    });
 
    bot.on_ready([&bot](const dpp::ready_t& event) {
        // in this on_ready block, shows how to create commands,
        // thier descriptions, 
        if (dpp::run_once<struct register_bot_commands>()) {

            dpp::slashcommand buy("buy", "buy an item", bot.me.id);
            buy.add_option(
                dpp::command_option(dpp::co_string, "item", "type of item", true).
                add_choice(dpp::command_option_choice("Gun", std::string("item_gun"))).
                add_choice(dpp::command_option_choice("Associate", std::string("item_assoc"))).
                add_choice(dpp::command_option_choice("Moonshine(L)", std::string("item_moonshine"))).
                add_choice(dpp::command_option_choice("Moonshine Still", std::string("item_still"))).
                add_choice(dpp::command_option_choice("Speakeasy", std::string("item_speaks"))).
                add_choice(dpp::command_option_choice("Casino", std::string("item_casino"))).
                add_choice(dpp::command_option_choice("Front", std::string("item_front")))
            );
            buy.add_option(
                dpp::command_option(dpp::co_integer, "amount", "purchase x item(s)", true).
                set_min_value(1)
            );

            dpp::slashcommand work("work", "perform 1 hour of labor", bot.me.id);
            work.add_option(
                dpp::command_option(dpp::co_string, "labor", "kind of labor", false).
                add_choice(dpp::command_option_choice("Distill", std::string("distill"))).
                add_choice(dpp::command_option_choice("Operate Speakeasy", std::string("speak_op"))).
                add_choice(dpp::command_option_choice("Operate Casino", std::string("casino_op")))
            );

            dpp::slashcommand bal("bal", "shows your balance", bot.me.id);

            dpp::slashcommand inventory("inventory", "shows what you own", bot.me.id);
            inventory.add_option(
                dpp::command_option(dpp::co_user, "admin", "admin user only, does nothing if not admin", false)
            );

            dpp::slashcommand pay("pay", "pay another user", bot.me.id);
            pay.add_option(
                dpp::command_option(dpp::co_user, "user", "send money to this user", true)
            );
            pay.add_option(
                dpp::command_option(dpp::co_number, "amount", "amount of cash to send", true).
                set_min_value(0.01)
            );

            dpp::slashcommand rob("rob", "rob another player", bot.me.id);
            rob.add_option(
                dpp::command_option(dpp::co_user, "user", "attempt to rob this user", true)
            );
            rob.add_option( // defaults to cash robbery
                dpp::command_option(dpp::co_integer, "item", "what you want to steal", false).
                add_choice(dpp::command_option_choice("Cash", int(0))).
                add_choice(dpp::command_option_choice("Moonshine", int(5)))
            );


            dpp::slashcommand edit("edit", "edit user's inventory, admin use only", bot.me.id);
            edit.add_option(
                dpp::command_option(dpp::co_user, "user", "user to edit", true)
            );
            edit.add_option(
                dpp::command_option(dpp::co_integer, "item", "item to edit", true).
                add_choice(dpp::command_option_choice("Cash", int(0))).
                add_choice(dpp::command_option_choice("Bank", int(1))).
                add_choice(dpp::command_option_choice("Guns", int(2))).
                add_choice(dpp::command_option_choice("Associates", int(3))).
                add_choice(dpp::command_option_choice("Stills", int(4))).
                add_choice(dpp::command_option_choice("Moonshine(L)", int(5))).
                add_choice(dpp::command_option_choice("Speaks' (BASE)", int(6))).
                add_choice(dpp::command_option_choice("Speaks' (TIER1)", int(7))).
                add_choice(dpp::command_option_choice("Speaks' (TIER2)", int(8))).
                add_choice(dpp::command_option_choice("Speaks' (TIER3)", int(9))).
                add_choice(dpp::command_option_choice("Casino Balance", int(10))).
                add_choice(dpp::command_option_choice("Casino (BASE)", int(11))).
                add_choice(dpp::command_option_choice("Casino (TIER1)", int(12))).
                add_choice(dpp::command_option_choice("Casino (TIER2)", int(13))).
                add_choice(dpp::command_option_choice("Casino (TIER3)", int(14))).
                add_choice(dpp::command_option_choice("Front", int(15))).
                add_choice(dpp::command_option_choice("Wisdom", int(16)))
            );
            edit.add_option(
                dpp::command_option(dpp::co_number, "amount", "amount to set to", true)
            );

            dpp::slashcommand sell("sell", "offload your moonshine", bot.me.id);

            dpp::slashcommand roulette("roulette", "play a game of roulette", bot.me.id);
            roulette.add_option(
                dpp::command_option(dpp::co_string, "color", "color to bet on", true).
                add_choice(dpp::command_option_choice("Green", std::string("green"))).
                add_choice(dpp::command_option_choice("Red", std::string("red"))).
                add_choice(dpp::command_option_choice("Black", std::string("black")))
            );
            roulette.add_option(
                dpp::command_option(dpp::co_number, "bet", "how much you are betting", true).
                set_min_value(0.01)
            );
            roulette.add_option(
                dpp::command_option(dpp::co_user, "user", "roulette at this user's casino", false)
            );

            dpp::slashcommand crash_roulette("crash_roulette", "play a game of crash roulette", bot.me.id);
            crash_roulette.add_option(
                dpp::command_option(dpp::co_number, "bet", "how much you are betting", true).
                set_min_value(1.00)
            );
            crash_roulette.add_option(
                dpp::command_option(dpp::co_number, "multiplier", "guess the multiplier it'll land on", true).
                set_min_value(1.01)
            );
            crash_roulette.add_option(
                dpp::command_option(dpp::co_user, "user", "crash roulette at this user's casino", false)
            );

            dpp::slashcommand upgrade("upgrade", "what business to upgrade", bot.me.id);
            upgrade.add_option(
                dpp::command_option(dpp::co_string, "business", "which business to upgrade", true).
                add_choice(dpp::command_option_choice("Speakeasy", std::string("speaks"))).
                add_choice(dpp::command_option_choice("Casino", std::string("casino")))
            );
            upgrade.add_option(
                dpp::command_option(dpp::co_integer, "tier", "what tier to upgrade to (1-3)", true).
                set_min_value(1).
                set_max_value(3)
            );

            dpp::slashcommand casino("casino", "casino management for owners", bot.me.id);
            casino.add_option(
                dpp::command_option(dpp::co_string, "action", "casino management", true).
                add_choice(dpp::command_option_choice("View", std::string("casino_view"))).
                add_choice(dpp::command_option_choice("Restock", std::string("casino_restock"))).
                add_choice(dpp::command_option_choice("Cashout", std::string("casino_cashout")))
            );

            dpp::slashcommand deposit("deposit", "deposit your money into bank for safekeeping", bot.me.id);
            deposit.add_option(
                dpp::command_option(dpp::co_number, "amount", "amount to deposit", true).
                set_min_value(0.01)
            );
    

            std::vector<dpp::slashcommand> new_comms;
            new_comms.push_back(work);
            new_comms.push_back(buy);
            new_comms.push_back(upgrade);
            new_comms.push_back(deposit);
            new_comms.push_back(bal);
            new_comms.push_back(inventory);
            new_comms.push_back(pay);
            new_comms.push_back(edit);
            new_comms.push_back(rob);
            new_comms.push_back(roulette);
            new_comms.push_back(crash_roulette);
            new_comms.push_back(casino);

            // bot.global_bulk_command_create(new_comms);
        }
    });
    
    Dictionary& dict = userDict;

    auto writeTaskWrapper = [&dict]() {
        writeTask(dict);
    };

    std::thread timerThread(writeTaskWrapper); // run something every x time, concurrently with the bot
    bot.start(dpp::st_wait);

    return 0;
}