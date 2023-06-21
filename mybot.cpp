#include <dpp/dpp.h>
#include "mobster.cpp"
using namespace std;

const std::string    BOT_TOKEN    = "NjkyMDc0OTI3MjQ2OTM0MDM3.XnpPAA.ArATND3efgNt0OSJysPlnE94l2g";
 
// messy so far, just testing out certain methods before cleaning up and writing real functions
// https://dpp.dev/build-a-discord-bot-windows-wsl.html

int main() {

    hello();
    goodbye();

    dpp::cluster bot(BOT_TOKEN);
 
    bot.on_log(dpp::utility::cout_logger());
 
    bot.on_slashcommand([](const dpp::slashcommand_t& event) {
        if (event.command.get_command_name() == "ping") {
            event.reply("Pong!");
        }

        if(event.command.get_command_name() == "sup"){
            event.reply("cuh!");
        }

        if(event.command.get_command_name() == "blep"){
            string param = std::get<std::string>(event.get_parameter("animal"));
            event.reply(param);
        }
    });
 
    bot.on_ready([&bot](const dpp::ready_t& event) {
        if (dpp::run_once<struct register_bot_commands>()) {
            bot.global_command_create(dpp::slashcommand ("ping", "Ping pong!", bot.me.id));
            bot.global_command_create(dpp::slashcommand ("sup", "greeting", bot.me.id));
            
            dpp::slashcommand newcommand("blep", "Send a random adorable animal photo", bot.me.id);
            newcommand.add_option(
                    dpp::command_option(dpp::co_string, "animal", "The type of animal", true).
                        add_choice(dpp::command_option_choice("Dog", std::string("animal_dog"))).
                        add_choice(dpp::command_option_choice("Cat", std::string("animal_cat"))).
                        add_choice(dpp::command_option_choice("Penguin", std::string("animal_penguin")
                    )
                )
            );

 
            /* Register the command */
            bot.global_command_create(newcommand);
        }
    });
 
    bot.start(dpp::st_wait);
}