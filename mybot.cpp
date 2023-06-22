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
const double min_wage = 0.50;
 
// messy so far, just testing out certain methods before cleaning up and writing real functions
// https://dpp.dev/build-a-discord-bot-windows-wsl.html

void timerTask() {
    while (true) {
        // Do something every 15 seconds
        std::cout << "Timer task: wrote to file\n";

        // Sleep for 15 seconds
        std::this_thread::sleep_for(std::chrono::seconds(15));
    }
}

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
    std::string txtLine = "";

    ifstream inputFile;
    try{
        inputFile.open("test.txt", ios::in);
        getline(inputFile, txtLine);

        inputFile.close();
    }
    catch(logic_error& e){
        std::cout << "File not found" << endl;
    }

    cout << "From txt file: " << txtLine << endl;
    
    Dictionary userDict;
    // user dict will load in users from memory 


    dpp::cluster bot(BOT_TOKEN);
 
    bot.on_log(dpp::utility::cout_logger());
 
    bot.on_slashcommand([&userDict](const dpp::slashcommand_t& event) {
        if (event.command.get_command_name() == "ping") {
            event.reply("Pong!");
        }

        if(event.command.get_command_name() == "sup"){
            // event.reply("cuh!");
            dpp::user who = event.command.get_issuing_user(); // this is how to get a user obj
            std::string rep = who.username + " sent this cmd"; // this is username from user class
            event.reply(rep);
        }

        if(event.command.get_command_name() == "blep"){
            std::string param = std::get<std::string>(event.get_parameter("animal")); // reading parameter type
            std::int64_t amount = std::get<std::int64_t>(event.get_parameter("amount")); // reading parameter amount
            std::string am = std::to_string(amount); // converting to string so can output properly
            std::string rep = "You selected " + am;
            rep += " " + param;
            event.reply(rep);
        }

        if(event.command.get_command_name() == "work"){
            // first use will register user - add them to database
            // we can just setvalue since that already deals with nonexistent entry
            dpp::user who = event.command.get_issuing_user();
            std::string rep = who.username;
            try{ // check to see if token exists
                valType& value = userDict.getValue(rep);
                // if it does, increment value
                value += min_wage;
                std::string msg = "@" + rep + " you earned 0.50\n";
                msg += "you have " + std::to_string(value) + " in earnings"; 
                event.reply(msg);
            }catch(logic_error& e){
                // if can't find, add it
                userDict.setValue(rep, min_wage);
                std::string msg = "@" + rep + " you earned 0.50 from your first hour\n";
                event.reply(msg);
            }
        }
    });
 
    bot.on_ready([&bot](const dpp::ready_t& event) {
        // in this on_ready block, shows how to create commands,
        // thier descriptions, 
        if (dpp::run_once<struct register_bot_commands>()) {
            dpp::slashcommand ping("ping", "Ping pong!", bot.me.id);
            dpp::slashcommand sup("sup", "greeting", bot.me.id);
            dpp::slashcommand blep("blep", "request x animals", bot.me.id);
            blep.add_option(
                    dpp::command_option(dpp::co_string, "animal", "The type of animal", true).
                        add_choice(dpp::command_option_choice("Dog", std::string("animal_dog"))).
                        add_choice(dpp::command_option_choice("Cat", std::string("animal_cat"))).
                        add_choice(dpp::command_option_choice("Penguin", std::string("animal_penguin")
                    )
                )
            );
            // this adds an amount parameter: x from min-max_range
            blep.add_option(
                dpp::command_option(dpp::co_integer, "amount", "amount you want", true).
                    set_min_value(1). // set minimum
                    set_max_value(100) // set maximum
            );

            dpp::slashcommand work("work", "perform 1 hour of labor", bot.me.id);

            std::vector<dpp::slashcommand> new_comms;
            new_comms.push_back(ping);
            new_comms.push_back(sup);
            new_comms.push_back(blep);
            new_comms.push_back(work);

            bot.global_bulk_command_create(new_comms);

            /* Register the command */
            //bot.global_command_create(blep); // registers the command with discord
        }
    });
    
    Dictionary& dict = userDict;

    auto writeTaskWrapper = [&dict]() {
        writeTask(dict);
    };

    std::thread timerThread(writeTaskWrapper); // run something every x time, concurrently with the bot
    bot.start(dpp::st_wait);


    goodbye();

    return 0;
}