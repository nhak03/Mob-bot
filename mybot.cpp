#include <dpp/dpp.h>
#include<fstream>
#include<vector>
#include<iostream>
#include<random>
#include<functional>
#include "Dictionary.h"
#include "Dictionary.cpp" // include RBT structs to use
#include "mobster.cpp"

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
                response += "$`" + pock + "` in your pocket\n";
                response += "$`" + banc + "` in your bank account";
                event.reply(response);
            }catch(logic_error& e){
                createEntry(userDict, who.username);
                std::string response = who.get_mention() + " you have $0 to your name.";
                event.reply(response);
            }
        }

        if(event.command.get_command_name() == "inventory"){
            dpp::user who = event.command.get_issuing_user();
            std::string user_name = who.username;
            if(who.username == "dajujumaster"){
                // if admin param is selected, try to find the user
                std::variant<monostate, string, long int, bool, dpp::snowflake, double> viewUser = event.get_parameter("admin");
                try{
                    // if succeeded we lookup their inventory
                    dpp::snowflake temp = std::get<dpp::snowflake>(viewUser);
                    dpp::user temp_user = event.command.get_resolved_user(temp);
                    user_name = temp_user.username;
                    who = temp_user;
                }catch(const std::bad_variant_access& ex){
                    // if fail, just look at our own
                    // cout << "in the catch\n";
                    user_name = "dajujumaster";
                }
            }
            
            try{
                valType* valarray = userDict.getArray(user_name);
                std::ostringstream output;
                std::string pocket = doub_to_str(valarray[0]);
                std::string bank = doub_to_str(valarray[1]);
                std::string guns = std::to_string(static_cast<int>(valarray[2]));
                std::string associates = std::to_string(static_cast<int>(valarray[3]));
                std::string stills = std::to_string(static_cast<int>(valarray[4]));
                std::string moonshine = doub_to_str(valarray[5]);
                double speak_tot = valarray[6] + valarray[7];
                speak_tot += valarray[8]; speak_tot+= valarray[9];
                std::string speaks = std::to_string(static_cast<int>(speak_tot));
                double casino_tot = valarray[11]; casino_tot += valarray[12];
                casino_tot += valarray[13]; casino_tot += valarray[14];
                std::string casinos = std::to_string(static_cast<int>(casino_tot));
                std::string response = who.get_mention() + "'s Inventory: ```";
                output << std::left << std::setw(12) << "Cash: " << std::setw(15) << pocket << std::setw(15) << "Stills: " << stills << std::endl;
                output << std::left << std::setw(12) << "Bank: " << std::setw(15) << bank << std::setw(15) << "Moonshine(L): " << moonshine << std::endl;
                output << std::left << std::setw(12) << "Guns: " << std::setw(15) << guns << std::setw(15) << "Speaks': " << speaks << std::endl;
                output << std::left << std::setw(12) << "Associates: " << std::setw(15) << associates << std::setw(15) << "Casinos: " << casinos << std::endl;
                response += output.str() + "```";
                event.reply(response);
            }catch(logic_error& e){
                createEntry(userDict, user_name);
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
            valType* valarray; // declare it so it's visible
            try{
                // fetch it from dictionary if it exists
                valarray = userDict.getArray(who.username);
            }catch(logic_error& e){
                // otherwise make a new entry for it
                createEntry(userDict, who.username);
                valarray = userDict.getArray(who.username);
            }
            if(labor_type == "default"){
                valarray[0] += min_wage;
                std::string msg = who.get_mention() + " you earned $`" + doub_to_str(min_wage) + "`";
                msg += "\nyou have $`" + doub_to_str(valarray[0]) + "` in earnings"; 
                event.reply(msg);
                return;
            }
            if(labor_type == "distill"){
                if(valarray[4] < 1){ // if i have no stills, then I can't distill
                    std::string msg = who.get_mention() + " you need stills to be able to distill moonshine";
                    event.reply(msg);
                }else{
                    double product = valarray[4] * still_prod;
                    valarray[5] += product;
                    std::string msg = who.get_mention() + " you made " + doub_to_str(product) + " liters of moonshine";
                    event.reply(msg);
                }
                return;
            }
            if(labor_type == "speak_op"){
                double result = speak_revenue(valarray);
                if(result == 0){
                    event.reply("You have no speakeasies to operate.");
                    return;
                }
                if(result == -1){
                    event.reply("You have no alcohol sell at your speakeasies.");
                    return;
                }
                if(result < -1){
                    std::string msg = "You don't have enough alcohol to operate your speakeasies.\n";
                    msg += "You need `" + doub_to_str(result) + "` liters minimum.";
                    event.reply(msg);
                    return;
                }
                std::string msg = "You've earned $`" + doub_to_str(result) + "` in revenue from your speakeasies!";
                event.reply(msg);
            }
            if(labor_type == "casino_op"){
                double resArray[] = {0, 0, 0}; // revenue, multiplier, alc consumed
                double* ptrArr = resArray;
                casino_revenue(valarray, ptrArr);
                if(resArray[0] == 0){
                    event.reply("You have no casinos to operate.");
                    return;
                }
                std::string msg;
                msg = "You've earned $`" + doub_to_str(resArray[0]) + "` in revenue from your casinos!";
                if(resArray[1] > 1.00){
                    msg += "\nRevenue multiplier: `" + doub_to_str(resArray[1]) + "` || ";
                    msg += "Alcohol consumed(L): `" + doub_to_str(resArray[2]) + "`";
                }
                event.reply(msg);
            }

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

            // this try and catch block is for inventory lookup for sender
            valType* senderArr; valType* recieverArr;
            try{
                senderArr = userDict.getArray(sender.username);
                if((senderArr[0] + senderArr[1]) < amount){
                    event.reply("You don't have enough money to send that amount.");
                    return;
                }
            }catch(logic_error& e){
                createEntry(userDict, sender.username);
                event.reply("You don't have enough money to send that amount.");
                return;
            }

            // this try and catch block is inventory lookup for reciever
            try{
                recieverArr = userDict.getArray(reciever.username);
            }catch(logic_error& e){
                createEntry(userDict, reciever.username);
                recieverArr = userDict.getArray(reciever.username);
            }

            recieverArr[0] += amount; // send money to reciever
            std::string response = "You've sent $`" + doub_to_str(amount) + "` to " + reciever.get_mention();
            event.reply(response);

            if(senderArr[0] - amount >= 0){ // if can subtract from pocket fully, do so
                senderArr[0] -= amount;
            }else{
                amount -= senderArr[0];
                senderArr[0] = 0;
                senderArr[1] -= amount;
            }
        }

        if(event.command.get_command_name() == "roulette"){
            dpp::user player = event.command.get_issuing_user();
            double bet_amount = std::get<double>(event.get_parameter("bet"));
            std::string outcome = std::get<std::string>(event.get_parameter("color"));
            valType* playerArr = userDict.getArray(player.username);
            std::string response;
            if((playerArr[0] + playerArr[1]) < bet_amount){
                response = "You don't have enough money to place that bet.";
                event.reply(response);
                return;
            }
            // implement optional handler for user_casino, by default it's bot
            std::string house = "bot";
            std::variant<monostate, string, long int, bool, dpp::snowflake, double> recieve = event.get_parameter("user");
            dpp::user usr_house;
            try{ // this try and catch block is to find the user to send money to
                // try to get a user obj
                dpp::snowflake temp = std::get<dpp::snowflake>(recieve);
                usr_house = event.command.get_resolved_user(temp);
                house = usr_house.username;
            }catch(const std::bad_variant_access& ex){
                // if fail, just gamble with bot as house
                // do nothing, house already set to bot by default
            }
            double bet_outcome = bot_roulette_spin(outcome, bet_amount);
            if(house == "bot"){
                if(bet_outcome < 0){ // if neg, player lost
                    // case where bet_outcome is negative
                    if(playerArr[0] - bet_outcome >= 0){
                        // if can subtract fully from pocket, do so
                        playerArr[0] += bet_outcome;
                    }
                    else{
                        // otherwise, do fully from pocket and rest from bank
                        // bet_outcome is neg value
                        bet_outcome += playerArr[0];
                        playerArr[0] = 0;
                        playerArr[1] += bet_outcome;
                    }
                    response = roulette_loss_msg(outcome);
                    response += "\nYou lost $`" + doub_to_str(bet_amount) + "` in that roulette spin.";
                    event.reply(response);
                }else{
                    // bet_outcome > 0, player won the bet
                    playerArr[0] += bet_outcome;
                    response = roulette_win_msg(outcome);
                    response += "\nYou won $`" + doub_to_str(bet_outcome) + "` in that roulette spin!";
                    event.reply(response);
                }
            }else{
                // handler to deal with non-bot house
                valType* houseArr = userDict.getArray(house);
                std::string response = roulette_player_house(playerArr, houseArr, house);
                event.reply(response);
            }
        }

        if(event.command.get_command_name() == "crash_roulette"){
            // get a user bet, user guess
            // run crash and see what mult is given
            // calculate winnings/loss
            dpp::user player = event.command.get_issuing_user();
            double bet_amount = std::get<double>(event.get_parameter("bet"));
            double mult_guess = std::get<double>(event.get_parameter("multiplier"));
            valType* playerArr = userDict.getArray(player.username);

            if((playerArr[0] + playerArr[1]) < bet_amount){
                std::string response;
                response = "You don't have enough money to place that bet.";
                event.reply(response);
                return;
            }

            std::string house = "bot";
            std::variant<monostate, string, long int, bool, dpp::snowflake, double> recieve = event.get_parameter("user");
            dpp::user usr_house;
            try{ // this try and catch block is to find the user to send money to
                // try to get a user obj
                dpp::snowflake temp = std::get<dpp::snowflake>(recieve);
                usr_house = event.command.get_resolved_user(temp);
                house = usr_house.username;
            }catch(const std::bad_variant_access& ex){
                // if fail, just gamble with bot as house
                // do nothing, house already set to bot by default
            }

            double mult_real = crash();
            std::string response;
            if(house == "bot"){
                if(mult_guess > mult_real){
                    // loss
                    response = "🚀~~~>💥🔥\n";
                    response += "You lost $`" + doub_to_str(bet_amount) + "` in that crash bet.\n";
                    if(playerArr[0] - bet_amount >= 0){
                        // if can subtract from pocket fully, do so
                        playerArr[0] -= bet_amount;
                    }else{
                        // otherwise, subtract from pocket, and then from bank
                        bet_amount -= playerArr[0];
                        playerArr[0] = 0;
                        playerArr[1] -= bet_amount;
                    }
                    response += "Your guess: `" + doub_to_str(mult_guess) + "` || ";
                    response += "Real multiplier: `" + doub_to_str(mult_real) + "`"; 
                    event.reply(response); 
                }else{
                    // win
                    double winnings = bet_amount * mult_guess;
                    playerArr[0] += winnings;
                    response = "🚀~~~>✨🎆\n";
                    response += "You won $`" + doub_to_str(winnings) + "` in that crash bet!\n";
                    response += "Your guess: `" + doub_to_str(mult_guess) + "` || ";
                    response += "Real multiplier: `" + doub_to_str(mult_real) + "`"; 
                    event.reply(response);
                }
            }
            else{
                // non-bot house handler
            }
        }

        if(event.command.get_command_name() == "rob"){
            dpp::user robber = event.command.get_issuing_user();
            std::variant<monostate, string, long int, bool, dpp::snowflake, double> recieve = event.get_parameter("user");
            dpp::user victim;
            try{ // this try and catch block is to find the user to send money to
                // try to get a user obj
                dpp::snowflake temp = std::get<dpp::snowflake>(recieve);
                victim = event.command.get_resolved_user(temp);
            }catch(const std::bad_variant_access& ex){
                // if fail, just look at our own
                // send fail message
                event.reply("Could not find that user to rob.");
                return;
            }

            int steal_type;
            std::variant<monostate, string, long int, bool, dpp::snowflake, double> stealVar = event.get_parameter("item");
            try{
                steal_type = std::get<long int>(stealVar);
            }catch(const std::bad_variant_access& ex){
                steal_type = 0;
            }

            valType* robberArray;
            try{
                robberArray = userDict.getArray(robber.username);
            }catch(logic_error& e){
                createEntry(userDict, robber.username);
                robberArray = userDict.getArray(robber.username);
            }

            valType* victimArray;
            try{
                victimArray = userDict.getArray(victim.username);
            }catch(logic_error& e){
                createEntry(userDict, victim.username);
                victimArray = userDict.getArray(victim.username);
            }

            if(rob_odds(robberArray[3], victimArray[3]) == true){ //robbery success
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<int> distribution(10, 20);
                int percent_taken = distribution(gen);
                double percent_to_take = percent_taken/100.00; // returns some decimal from .10 to .20
                double amount_stole = victimArray[steal_type] * percent_to_take;
                victimArray[steal_type] -= amount_stole;
                robberArray[steal_type] += amount_stole;
                std::string item;
                if(steal_type == 0){
                    item = " dollars";
                }else{
                    item = " liters of moonshine";
                }
                std::string response = "You've robbed " + victim.get_mention() + " and stole " + doub_to_str(amount_stole) + item;
                // implement later, random chance to lose associates even when success
                event.reply(response);
            }else{
                // false, robbery failed
                int assocs_lost = assoc_loss(robberArray[3]);
                robberArray[3] -= assocs_lost;
                std::string response = "You lost " + std::to_string(assocs_lost) + " associates in a botched robbery on " + victim.get_mention();
                event.reply(response);
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
                    response += "Ordered: " + am + " " + param + " || Cost: $" + cost;
                    event.reply(response);
                    return;
                }
                if((valarray[3] - assoc_cost) < 0){ // if insufficient associates
                    std::string response = who.get_mention() + " you need more associates to run that\n";
                    response += "Ordered: " + am + " " + param + " || Need " + to_string(static_cast<int>(assoc_cost)) + " associates";
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
                    if(param == "item_speaks"){
                        valarray[3] -= assoc_cost;
                        valarray[6] += amount;
                        // [6] for base, [7] for tier 1, [8] for tier 2, [9] for tier 3
                    }
                    if(param == "item_casino"){
                        valarray[3] -= assoc_cost;
                        valarray[11] += amount;
                    }

                    // cout << "testing if event.reply is like a return\n";
                    std::string response = who.get_mention() + " you bought " + am + " " + param + " for $`" + doub_to_str(total_cost) + "`"; 
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

        if(event.command.get_command_name() == "upgrade"){
            std::string business = std::get<std::string>(event.get_parameter("business"));
            int tier = std::get<std::int64_t>(event.get_parameter("tier"));
            dpp::user who = event.command.get_issuing_user();
            valType* valarray = getEntry(userDict, who.username);
            int base; // what index from user inventory
            if(business == "speaks"){
                business = "speakeasy";
                base = 6; // 6, 7, 8, 9
            }
            if(business == "casino"){
                base = 11; // 11, 12, 13, 14
            }
            
            std::string response = upgrade_check(valarray, business, tier, base);
            event.reply(response);
        }

        if(event.command.get_command_name() == "edit"){
            // int param = std::get<long int>(event.get_parameter("item"));
            // double amount = std::get<double>(event.get_parameter("amount"));
            // std::string response = "You selected " + to_string(param) + "\n Will change value stored there to " + doub_to_str(amount);
            // event.reply(response);
            dpp::user who = event.command.get_issuing_user();
            if(who.username == "dajujumaster"){
                int param = std::get<long int>(event.get_parameter("item"));
                double amount = std::get<double>(event.get_parameter("amount"));
                std::variant<monostate, string, long int, bool, dpp::snowflake, double> viewUser = event.get_parameter("user");
                try{
                    dpp::snowflake temp = std::get<dpp::snowflake>(viewUser);
                    who = event.command.get_resolved_user(temp);
                }catch(const std::bad_variant_access& ex){
                    event.reply("Could not find that user");
                    return;
                }
                valType* valarray = userDict.getArray(who.username);
                double old_val = valarray[param];
                valarray[param] = amount;
                std::string response = "Changed " + who.get_mention() + "[" + to_string(param) + "]" + " from ";
                response += doub_to_str(old_val) + " to " + doub_to_str(amount);
                event.reply(response);
            }
            else{
                event.reply("You do not have sufficient permissions to use this.");
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
                add_choice(dpp::command_option_choice("Moonshine(L)", std::string("item_moonshine"))).
                add_choice(dpp::command_option_choice("Moonshine Still", std::string("item_still"))).
                add_choice(dpp::command_option_choice("Speakeasy", std::string("item_speaks"))).
                add_choice(dpp::command_option_choice("Casino", std::string("item_casino")))
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
                add_choice(dpp::command_option_choice("Casino (TIER3)", int(14)))
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
    

            std::vector<dpp::slashcommand> new_comms;
            new_comms.push_back(work);
            new_comms.push_back(buy);
            new_comms.push_back(upgrade);
            new_comms.push_back(bal);
            new_comms.push_back(inventory);
            new_comms.push_back(pay);
            new_comms.push_back(edit);
            new_comms.push_back(rob);
            new_comms.push_back(roulette);
            new_comms.push_back(crash_roulette);

            // bot.global_bulk_command_create(new_comms);
        }
    });
    
    Dictionary& dict = userDict;

    auto writeTaskWrapper = [&dict]() {
        writeTask(dict);
    };

    // cout << "The dict: \n" << userDict << endl;

    std::thread timerThread(writeTaskWrapper); // run something every x time, concurrently with the bot
    bot.start(dpp::st_wait);

    return 0;
}