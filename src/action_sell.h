#include <dpp/dpp.h>
#include "Dictionary.h"
#include "commons.h"

#ifndef ACTION_SELL
#define ACTION_SELL

// inputs: dictionary, username, product, volume
// returns a string indicating the sale
// edits the inventory directly
dpp::embed action_sell(Dictionary& dict, std::string username, std::string product, double volume);

#endif