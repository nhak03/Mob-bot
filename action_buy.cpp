#include <iostream>
#include <string>
#include "action_buy.h"
const double gun_price = 30.00;
const double assoc_price = 30.00;
const double still_price = 250.00;
const double shine_price = 5.00;
const double speak_price = 2000.00;
const double casino_price = 15000.00;
const double front_price = 20000.00;

double calc_cost(std::string item, int64_t amount){
    double total_cost = 0;
    if(item == "item_gun"){
        total_cost = gun_price * amount;
    }
    if(item == "item_assoc"){
        total_cost = assoc_price * amount;
    }
    if(item == "item_still"){
        total_cost = still_price * amount;
    }
    if(item == "item_moonshine"){
        total_cost = shine_price * amount;
    }
    if(item == "item_speaks"){
        total_cost = speak_price * amount;
    }
    if(item == "item_casino"){
        total_cost = casino_price * amount;
    }
    if(item == "item_front"){
        total_cost = front_price * amount;
    }
    return total_cost;
}

double assoc_check(std::string item, int64_t amount){
    double total_cost = 0;
    if(item == "item_still"){
        total_cost = 2 * amount;
    }
    if(item == "item_speaks"){
        total_cost = 25 * amount;
    }
    if(item == "item_casino"){
        total_cost = 25 * amount;
    }
    if(item == "item_front"){
        total_cost = 10 * amount;
    }
    return total_cost;
}

std::string action_buy(Dictionary& dict, std::string username, std::string mention, std::string item, int amount){
    valType* valarray = getEntry(dict, username);
    double total_cost = calc_cost(item, amount);
    double assoc_cost = assoc_check(item, amount);
    std::string response;
    if((valarray[0] + valarray[1]) - total_cost < 0){ // if you can't afford
        response = "❌" + mention + " you could not afford that purchase\n";
        response += "Ordered: " + std::to_string(amount) + " " + item + " || Cost: $" + doub_to_str(total_cost);
        return response;
    }
    if((valarray[3] - assoc_cost) < 0){ // if insufficient associates
        response = "❌" + mention + " you need more associates to run that\n";
        response += "Ordered: " + std::to_string(amount) + " " + item + " || Need " + std::to_string(static_cast<int>(assoc_cost)) + " associates";
        return response;
    }else{
        if((valarray[0]-total_cost) >= 0){
            // if cost can be subbed from pocket without negative, do it
            valarray[0] -= total_cost;
        }
        else{
            // else, sub what I can using pocket, pay the rest with bank
            total_cost -= valarray[0];
            valarray[0] = 0;
            valarray[1] -= total_cost;
        } // end of money payment methods

        if(item == "item_gun"){
            valarray[2] += amount;
            if(amount > 1){
                item = "guns";
            }else{
                item = "gun";
            }
        }
        if(item == "item_assoc"){
            // check to see if I can buy, need minimum one gun per assoc
            if(valarray[2] < amount){
                response = "❌" + mention + " you have the cash, but you need x guns to hire x associates.\n";
                response += "You have " + std::to_string(static_cast<int>(valarray[2])) + " guns.";
                return response;
            }
            else{ // you do have the guns
                valarray[2] -= amount;
                valarray[3] += amount;
                response = "🤝" + mention + " you've successfully hired " + std::to_string(amount) + " associates.";
                return response;
            }
        }
        if(item == "item_still"){
            valarray[3] -= assoc_cost; //
            valarray[4] += amount;
            if(amount > 1){
                item = "moonshine stills";
            }else{
                item = "moonshine still";
            }
        }
        if(item == "item_moonshine"){
            valarray[5] += amount;
            if(amount > 1){
                item = "liters of moonshine";
            }else{
                item = "liter of moonshine";
            }
        }
        if(item == "item_speaks"){
            valarray[3] -= assoc_cost;
            valarray[6] += amount;
            if(amount > 1){
                item = "speakeasies";
            }else{
                item = "speakeasy";
            }
            // [6] for base, [7] for tier 1, [8] for tier 2, [9] for tier 3
        }
        if(item == "item_casino"){
            valarray[3] -= assoc_cost;
            valarray[11] += amount;
            if(amount > 1){
                item = "casinos";
            }else{
                item = "casino";
            }
            // [11] for base, [12] for t1, [13] for t2, [14] for t3
        }

        // cout << "testing if event.reply is like a return\n";
        response = "✅" + mention + " you bought " + std::to_string(amount) + " " + item + " for $`" + doub_to_str(total_cost) + "`"; 
        return response;
    }
    return response;
}