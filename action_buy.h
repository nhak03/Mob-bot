#include "Dictionary.h"
#include "commons.h"

#ifndef ACTION_BUY
#define ACTION_BUY

// inputs: dictionary, username, mention, item-name, amount to buy
// returns a string based on whether buy was successful
// edits the user inventory directly
std::string action_buy(Dictionary& dict, std::string username, std::string mention, std::string item, int amount);

#endif