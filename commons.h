/*
This file contains common access functions for the bot
The bot loads a dictionary obj from a text file
Each action handler (bal, buy, rob, etc) is handled by a function
that accepts respective params, the dict obj, and any required usernames

will also contain basic response handlers such as /bal /inventory
*/
#include <iostream>
#include "Dictionary.h"

#ifndef COMMONS_H
#define COMMONS_H
const double val_hold = -3.1415;

// creates an entry for a new user
// param is dict and username
void createEntry(Dictionary& dict, std::string username);

// returns a pointer to the entry array
// corresponding to a username
valType* getEntry(Dictionary& dict, std::string username);

// takes a double and returns it in string up to 2 decimals
// std::string doub_to_str(double x);

// takes a dict and username
// returns a response that contains the user's balance
// in both pocket and bank
std::string action_bal(Dictionary& dict, std::string username, std::string mention);

std::string action_inventory(Dictionary& dict, std::string username, std::string mention);


#endif