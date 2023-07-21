#include "Dictionary.h"
#include "commons.h"

#ifndef ACTION_GAMBLE_H
#define ACTION_GAMBLE_H

/*
takes as input: dictionary, player name, bet amount, color guessed, and house name
returns a string indicating whether or not player won the roulette spin
edits the inventories of house and player directly
*/
std::string action_roulette(Dictionary& dict, std::string player, double bet, std::string color, std::string house);

/*
takes as input: dictionary, player name, bet amount, mult guessed, and house name
returns a string indicating whether or not player won the crash
edits the inventories of house and player directly
*/
std::string action_crash(Dictionary& dict, std::string player, double bet, double guess, std::string house);

/*
takes as input: dictionary, owner, and the action
acceptable actions: casino_view casino_cashout, casino_restock
returns a string indicating the status of the casino, or if any changes were made
if cashout/restock, will edit the owner inventory directly
*/
std::string action_casino(Dictionary& dict, std::string owner, std::string mention, std::string action);

#endif