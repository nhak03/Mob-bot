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
const double min_wage = 0.50;
const double still_prod = 4.00;

// creates an entry for a new user
// param is dict and username
void createEntry(Dictionary& dict, std::string username);

// returns a pointer to the entry array
// corresponding to a username
valType* getEntry(Dictionary& dict, std::string username);

// takes a double and returns it in string up to 2 decimals
std::string doub_to_str(double x);

// takes a dict and username
// returns a response that contains the user's balance
// in both pocket and bank
std::string action_bal(Dictionary& dict, std::string username, std::string mention);

// takes a dict and a username
// returns a response that contains the user's inventory
// if admin requested, then also shows hidden stats
std::string action_inventory(Dictionary& dict, std::string username, std::string mention, bool admin);

// takes a dict, sender, reciever, and reciever mention
// attempts to send money from sender to reciever
// edits both inventories directly 
// returns a response saying if transaction went through or not
std::string action_pay(Dictionary& dict, std::string sender, std::string recipient, double amount, std::string recip_mention);

// sees if the user is able to retire
// if minimums are met, then reset all their stats, and increase
// [18] by 1
std::string action_retire(Dictionary& dict, std::string username, std::string mention);

/*
Not a real function, hover to recall indexes
0 is pocket, 1 is bank, 2 is gun, 3 is assoc
4 is stills, 5 is moonshine, 6-9 is speaks
10 is casino balance, 11-14 is casinos
15 is fronts, 16 is deposit count, 17 is deposit amount before audit
18 is wisdom
*/
void index_checker();

#endif