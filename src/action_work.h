#include "Dictionary.h"
#include "commons.h"

#ifndef ACTION_WORK_H
#define ACTION_WORK_H

// takes input dictionary, username, and labor type
// returns a response depending on the labor type and the outcome of that
// if inventory is edited, this function does that as well
std::string action_work(Dictionary& dict, std::string username, std::string mention, std::string labor_type);

// what is robbing but another kind of work?
// takes input: dictionary, robber name, victim name, victim mention, and steal type
// returns a response depending on whether robbery was successful
// edits inventories directly
std::string action_rob(Dictionary& dict, std::string robber, std::string victim, std::string victim_mention, int steal_type);

#endif