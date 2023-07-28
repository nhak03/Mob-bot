#include "Dictionary.h"
#include "commons.h"

#ifndef ACTION_BUY
#define ACTION_BUY

// returns an embed showing a shop
// takes no parameters, as no data needs to be edited
dpp::embed action_shop();

// inputs: dictionary, username, mention, item-name, amount to buy
// returns a string based on whether buy was successful
// edits the user inventory directly
std::string action_buy(Dictionary& dict, std::string username, std::string mention, std::string item, int amount);

// inputs: dictionary, username, business, and tier
// tries to upgrade a business to the tier specified (edits inventory directly)
// returns string based on successful upgrade or not
std::string action_upgrade(Dictionary& dict, std::string username, std::string business, int tier);

#endif