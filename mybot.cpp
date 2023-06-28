#include <dpp/dpp.h>
#include<fstream>
#include<vector>
#include<iostream>
#include<functional>
#include "mobster.cpp"
#include "Dictionary.h"
#include "Dictionary.cpp" // include RBT structs to use

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

void createEntry(Dictionary& dict, std::string username){
    // if a new user is being added, create entry for them
    // and initialize all inventory elems to 0
    dict.setValue(username, val_hold);
    valType* inventory = dict.getArray(username);
    for(int i=0; i<15; i++){
        inventory[i] = 0;
    }
}

std::string doub_to_str(double x){
    std::ostringstream p;
    p << std::fixed << std::setprecision(2) << x;
    std::string pock = p.str();
    return pock;
}

int main() {

    hello();

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
                    for(int i=0; i<15; i++){
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
            first_Set = false; // use to reset once we get to next line (each line is one user)
            // cout << "Middle index of " << first << " " << inventory[5] << endl; 
            // cout << "Last index of " << first << " " << inventory[index-1] << endl;
            // inventory[index-1] = 20;
            // cout << "Changing last index to " << inventory[index-1] << endl;
            // for(int i=0; i<13; i++){
            //     cout << inventory[i] << " ";
            // } cout << endl;
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

        if(event.command.get_command_name() == "bal"){
            dpp::user who = event.command.get_issuing_user();
            try{
                valType* valarray = userDict.getArray(who.username);
                valType pocket = valarray[0];
                valType bank = valarray[1];
                std::ostringstream p;
                p << std::fixed << std::setprecision(2) << pocket;
                std::string pock = p.str();
                std::ostringstream b;
                b << std::fixed << std::setprecision(2) << bank;
                std::string banc = b.str();
                std::string response = who.get_mention() + " you have\n";
                response += "$" + pock + " in your pocket\n";
                response += "$" + banc + " in your bank account";
                event.reply(response);
            }catch(logic_error& e){
                createEntry(userDict, who.username);
                std::string response = who.get_mention() + " you have $0 to your name.";
                event.reply(response);
            }
        }

        if(event.command.get_command_name() == "inventory"){
            dpp::user who = event.command.get_issuing_user();
            try{
                valType* valarray = userDict.getArray(who.username);
                std::ostringstream output;
                std::string pocket = doub_to_str(valarray[0]);
                std::string bank = doub_to_str(valarray[1]);
                std::string guns = std::to_string(static_cast<int>(valarray[2]));
                std::string associates = std::to_string(static_cast<int>(valarray[3]));
                std::string stills = std::to_string(static_cast<int>(valarray[4]));
                std::string moonshine = "xx";
                std::string speaks = "xx";
                std::string casinos = "xx";
                std::string response = who.get_mention() + " this is what you have: ```";
                output << std::left << std::setw(12) << "Cash: " << std::setw(15) << pocket << std::setw(15) << "Stills: " << stills << std::endl;
                output << std::left << std::setw(12) << "Bank: " << std::setw(15) << bank << std::setw(15) << "Moonshine: " << moonshine << std::endl;
                output << std::left << std::setw(12) << "Guns: " << std::setw(15) << guns << std::setw(15) << "Speaks': " << speaks << std::endl;
                output << std::left << std::setw(12) << "Associates: " << std::setw(15) << associates << std::setw(15) << "Casinos: " << casinos << std::endl;
                response += output.str() + "```";
                event.reply(response);
            }catch(logic_error& e){
                createEntry(userDict, who.username);
                std::string response = who.get_mention() + " you have nothing to your name.";
                event.reply(response);
            }
        }
    
        if(event.command.get_command_name() == "work"){
            // first use will register user - add them to database
            // we can just setvalue since that already deals with nonexistent entry
            dpp::user who = event.command.get_issuing_user();
            std::string labor_type;
            std::variant<monostate, string, long int, bool, dpp::snowflake, double> laborVariant = event.get_parameter("labor");
            try{
                labor_type = std::get<std::string>(laborVariant);
            }catch(const std::bad_variant_access& ex){
                labor_type = "default";
            }
            cout << "User selected: " << labor_type << endl;
            // std::string rep = who.username;
            try{ // check to see if token exists
                // valType& value = userDict.getValue(rep);
                // if it does, increment value
                valType* valarray = userDict.getArray(who.username);
                valarray[0] += min_wage;
                std::string msg = who.get_mention() + " you earned " + to_string(min_wage);
                msg += " you have " + std::to_string(valarray[0]) + " in earnings"; 
                event.reply(msg);
            }catch(logic_error& e){
                // if can't find, add it
                createEntry(userDict, who.username);
                // userDict.setValue(rep, val_hold); // create user entry
                valType* valarray = userDict.getArray(who.username); // since array will be uninitialized, do that here
                valarray[0] = min_wage;
                std::string msg = who.get_mention() + " you earned " +  to_string(min_wage) + " from your first hour\n";
                event.reply(msg);
            }
        }

        if(event.command.get_command_name() == "buy"){
            std::string param = std::get<std::string>(event.get_parameter("item")); // reading parameter type
            std::int64_t amount = 1;
            try{
                amount = std::get<std::int64_t>(event.get_parameter("amount")); // reading parameter amount
            }catch(logic_error& e){
                // do nothing since amount initialized to 1
            }
            std::string am = std::to_string(amount); // converting to string so can output properly
            dpp::user who = event.command.get_issuing_user();
            double total_cost = calc_cost(param, amount);
            double assoc_cost = assoc_check(param, amount);
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(2) << total_cost;
            std::string cost = oss.str();
            try{
                valType* valarray = userDict.getArray(who.username);
                // index 0 is pocket balance, 1 is bank balance
                if((valarray[0] + valarray[1]) - total_cost < 0){ // if you can't afford
                    std::string response = who.get_mention() + " you could not afford that purchase\n";
                    response += "Ordered: " + am + " " + param + " || " + "Cost: $" + cost;
                    event.reply(response);
                    return;
                }
                if((valarray[3] - assoc_cost) < 0){ // if insufficient associates
                    std::string response = who.get_mention() + " you need more associates to run that\n";
                    response += "Ordered: " + am + " " + param;
                    event.reply(response);
                }else{
                    if((valarray[0]-total_cost) >= 0){
                        // if cost can be subbed from pocket without negative, do it
                        valarray[0] -= total_cost;
                    }
                    else{
                        // else, sub what I can using pocket, pay the rest with bank
                        total_cost -= valarray[0];
                        valarray[0] = 0;
                        valarray[1] -= total_cost;
                    } // end of money payment methods

                    if(param == "item_gun"){
                        valarray[2] += amount;
                    }
                    if(param == "item_assoc"){
                        // check to see if I can buy, need minimum one gun per assoc
                        if(valarray[2] < amount){
                            std::string response = who.get_mention() + " you have the cash, but you need x guns to hire x associates.\n";
                            response += "You have " + std::to_string(static_cast<int>(valarray[2])) + " guns.";
                            event.reply(response);
                            return;
                        }
                        else{ // you do have the guns
                            valarray[2] -= amount;
                            valarray[3] += amount;
                            std::string response = who.get_mention() + " you've successfully hired " + std::to_string(amount) + " associates.";
                            event.reply(response);
                            return;
                        }
                    }
                    if(param == "item_still"){
                        valarray[3] -= assoc_cost; //
                        valarray[4] += amount;
                    }
                    // cout << "testing if event.reply is like a return\n";
                    std::string response = who.get_mention() + " you bought " + am + " " + param + " for $" + std::to_string(total_cost); 
                    event.reply(response);
                }
                
            }catch(logic_error& e){
                // add user entry to dictionary if they don't exist (failed getArray)
                // then return a failed to purchase message
                createEntry(userDict, who.username);
                std::string response = who.get_mention() + " you could not afford that purchase\n";
                response += "Ordered: " + am + " " + param + " || " + "Cost: $" + cost;
                event.reply(response);
            }

            
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
                add_choice(dpp::command_option_choice("Moonshine Still", std::string("item_still")))
            );
            buy.add_option(
                dpp::command_option(dpp::co_integer, "amount", "purchase x item(s)", true).
                set_min_value(1)
            );

            dpp::slashcommand work("work", "perform 1 hour of labor", bot.me.id);
            work.add_option(
                dpp::command_option(dpp::co_string, "labor", "kind of labor", false).
                add_choice(dpp::command_option_choice("Distill", std::string("distill")))
            );
            dpp::slashcommand bal("bal", "shows your balance", bot.me.id);
            dpp::slashcommand inventory("inventory", "shows what you own", bot.me.id);

            std::vector<dpp::slashcommand> new_comms;
            new_comms.push_back(work);
            new_comms.push_back(buy);
            new_comms.push_back(bal);
            new_comms.push_back(inventory);

            // bot.global_bulk_command_create(new_comms);
        }
    });
    
    Dictionary& dict = userDict;

    auto writeTaskWrapper = [&dict]() {
        writeTask(dict);
    };

    cout << "The dict: \n" << userDict << endl;

    std::thread timerThread(writeTaskWrapper); // run something every x time, concurrently with the bot
    bot.start(dpp::st_wait);

    return 0;
}