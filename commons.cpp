#include <iostream>
#include <iomanip>
// #include "Dictionary.h"
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

std::string action_inventory(Dictionary& dict, std::string username, std::string mention){
    valType* valarray = getEntry(dict, username);
    std::ostringstream output;
    std::string pocket = doub_to_str(valarray[0]);
    std::string bank = doub_to_str(valarray[1]);
    std::string guns = std::to_string(static_cast<int>(valarray[2]));
    std::string associates = std::to_string(static_cast<int>(valarray[3]));
    std::string stills = std::to_string(static_cast<int>(valarray[4]));
    std::string moonshine = doub_to_str(valarray[5]);
    double speak_tot = valarray[6] + valarray[7];
    speak_tot += valarray[8]; speak_tot+= valarray[9];
    std::string speaks = std::to_string(static_cast<int>(speak_tot));
    double casino_tot = valarray[11]; casino_tot += valarray[12];
    casino_tot += valarray[13]; casino_tot += valarray[14];
    std::string casinos = std::to_string(static_cast<int>(casino_tot));
    std::string fronts = std::to_string(static_cast<int>(valarray[15]));
    std::string response = mention + "'s Inventory: ```";
    output << std::left << std::setw(12) << "Cash: " << std::setw(15) << pocket << std::setw(15) << "Stills: " << stills << std::endl;
    output << std::left << std::setw(12) << "Bank: " << std::setw(15) << bank << std::setw(15) << "Moonshine(L): " << moonshine << std::endl;
    output << std::left << std::setw(12) << "Guns: " << std::setw(15) << guns << std::setw(15) << "Speaks': " << speaks << std::endl;
    output << std::left << std::setw(12) << "Associates: " << std::setw(15) << associates << std::setw(15) << "Casinos: " << casinos << std::endl;
    output << std::left << std::setw(12) << "Fronts: " << std::setw(15) << fronts << std::setw(15) << std::endl;
    response += output.str() + "```";
    return response;
}