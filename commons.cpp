#include <iostream>
#include <iomanip>
#include "Dictionary.h"
#include "commons.h"

void createEntry(Dictionary& dict, std::string username){
    // if a new user is being added, create entry for them
    // and initialize all inventory elems to 0
    dict.setValue(username, val_hold);
    valType* inventory = dict.getArray(username);
    // inventory at getArray call is an array of x size,
    // with each index uninitialized
    for(int i=0; i<15; i++){
        inventory[i] = 0;
    }
    // after this for loop, the player array is now able 
    // to be operated on
}

// this function will return a user entry,
// if doesn't exist, will make one
// returns the pointer to the array when done
valType* getEntry(Dictionary& dict, std::string username){
    valType* valarray;
    try{
        valarray = dict.getArray(username);
    }catch(std::logic_error& e){
        createEntry(dict, username);
        valarray = dict.getArray(username);
    }
    return valarray;
}

std::string doub_to_str(double x){
    std::ostringstream p;
    p << std::fixed << std::setprecision(2) << x;
    std::string pock = p.str();
    return pock;
}

std::string action_bal(Dictionary& dict, std::string username, std::string mention){
    valType* userArr = getEntry(dict, username);
    double pocket = userArr[0];
    double bank = userArr[1];
    std::string response = mention + " you have:\n";
    response += "👛Pocket: $`" + doub_to_str(pocket) + "`\n";
    response += "🏦Bank:   $`" + doub_to_str(bank) + "`";

    return response;
}
