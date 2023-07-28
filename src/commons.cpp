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

dpp::embed action_inventory(Dictionary& dict, std::string username, std::string mention, bool admin){
    valType* valarray = getEntry(dict, username);
    dpp::embed response = dpp::embed();
    
    std::string pocket = "$" + doub_to_str(valarray[0]);
    std::string bank = "$" + doub_to_str(valarray[1]);
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

    std::string title = username + "'s Inventory \\👜";
    response.set_title(title);
    response.set_color(dpp::colors::cinnabar);
    response.add_field("Pocket 💵", pocket, true);
    response.add_field("Bank 🏦", bank, true);
    response.add_field("", "", false); // acts as a newline for embeds
    response.add_field("Guns", guns, true);
    response.add_field("Associates 🤝", associates, true);
    response.add_field("", "", false);
    response.add_field("Stills 🏺", stills, true);
    response.add_field("Moonshine(L) 🍾", moonshine, true);
    response.add_field("", "", false);
    response.add_field("Speakeasies 🍻", speaks, true);
    response.add_field("Casinos 🎰", casinos, true);
    response.add_field("Fronts 🍝", fronts, true);
    
    if(admin == true){
        std::string deposits = std::to_string(static_cast<int>(valarray[16]));
        std::string am_depod = doub_to_str(valarray[17]);
        std::string casino_bal = doub_to_str(valarray[10]);
        response.add_field("", "", false);
        response.add_field("Deposits", deposits, true);
        response.add_field("Amount Deposited", am_depod, true);
        response.add_field("Casino Balance", casino_bal, true);
    }
    return response;
}

std::string action_pay(Dictionary& dict, std::string sender, std::string recipient, double amount, std::string recip_mention){
    std::string msg;
    valType* senderArr = getEntry(dict, sender);
    valType* recpArr = getEntry(dict, recipient);

    if(senderArr[0] + senderArr[1] < amount){
        msg = "❌You don't have enough cash to send $`" + doub_to_str(amount) + "`";
        return msg;
    }
    // else
    recpArr[0] += amount;
    msg = "💸➡️🤝🏛️\n";
    msg += "You've sent $`" + doub_to_str(amount) + "` to " + recip_mention;

    if(senderArr[0] - amount >= 0){ // if can subtract from pocket fully, do so
        senderArr[0] -= amount;
    }else{
        amount -= senderArr[0];
        senderArr[0] = 0;
        senderArr[1] -= amount;
    }
    return msg;
}

void index_checker(){

}

const double retire_savings = 10000000.00;
const double retire_casino = 10;
const double retire_speaks = 20;
const double retire_fronts = 20;
std::string action_retire(Dictionary& dict, std::string username, std::string mention){
    valType* userArr = getEntry(dict, username);
    std::string msg;
    if(userArr[1] < retire_savings){
        msg = "Whoa there, " + mention + "! Can't exactly retire yet boss, need more dough for a cushy retirement!";
        return msg;
    }
    if(userArr[14] < retire_casino){
        msg = mention + ", come on! You gotta leave some great casinos in your name!";
        return msg;
    }
    if(userArr[10] < 0){
        msg = mention + "! Can't leave our gambling debts unpaid!";
        return msg; 
    }
    if(userArr[9] < retire_speaks){
        msg = mention + ", you gotta leave some legendary speakeasies before you leave!";
        return msg;
    }
    if(userArr[15] < retire_fronts){
        msg = mention + ", at least help the crew setup some fronts before you go!";
        return msg;
    }

    for(int i=0; i<18; i++){
        userArr[i] = 0.00; // reset all
    }
    userArr[18] += 1;
    msg = "✨🎉🎂🎆\n";
    msg += mention + ", the crew says that it's sad you're stepping down, but they wish you a happy retirement!\n";
    msg += "**Reset: gained 1 wisdom**";

    return msg;
}

