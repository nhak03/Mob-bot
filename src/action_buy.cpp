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

dpp::embed action_shop(){
    dpp::embed response = dpp::embed();

    response.set_title("What Cash Buys 💸");
    response.set_color(dpp::colors::cinnabar);
    
    std::string gun = "$" + doub_to_str(gun_price);
    response.add_field("Guns", gun, true);
    std::string assoc = "$" + doub_to_str(assoc_price) + " & " + " 1 Gun";
    response.add_field("Associates 🤝", assoc, true);

    response.add_field("", "", false);
    std::string still = "$" + doub_to_str(still_price) + " & " + "2 Associates";
    response.add_field("Stills 🏺", still, true);
    std::string shine = "$" + doub_to_str(shine_price);
    response.add_field("Moonshine 🍾", shine, true);

    response.add_field("", "", false);
    std::string speak = "$" + doub_to_str(speak_price) + " & " + "25 Associates";
    std::string casino = "$" + doub_to_str(casino_price) + " & " + "25 Associates";
    response.add_field("Speakeasies 🍻", speak, true);
    response.add_field("Casinos 🎰", casino, true);

    response.add_field("", "", false);
    std::string front = "$" + doub_to_str(front_price) + " & " + "10 Associates";
    response.add_field("Fronts 🍝", front, true);
    
    return response;
}

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

std::string param_to_item(std::string param){
    if(param == "item_gun"){
        param = "Gun";
    }
    if(param == "item_assoc"){
        param = "Associate";
    }
    if(param == "item_moonshine"){
        param = "Moonshine";
    }
    if(param == "item_still"){
        param = "Still";
    }
    if(param == "item_speaks"){
        param = "Speakeasy";
    }
    if(param == "item_casino"){
        param = "Casino";
    }
    return param;
}

std::string action_buy(Dictionary& dict, std::string username, std::string mention, std::string item, int amount){
    valType* valarray = getEntry(dict, username);
    double total_cost = calc_cost(item, amount);
    double assoc_cost = assoc_check(item, amount);
    std::string response;
    if((valarray[0] + valarray[1]) - total_cost < 0){ // if you can't afford
        response = "❌" + mention + " you could not afford that purchase\n";
        response += "Ordered: " + std::to_string(amount) + " " + param_to_item(item) + " || Cost: $" + doub_to_str(total_cost);
        return response;
    }
    if((valarray[3] - assoc_cost) < 0){ // if insufficient associates
        response = "❌" + mention + " you need more associates to run that\n";
        response += "Ordered: " + std::to_string(amount) + " " + param_to_item(item) + " || Need " + std::to_string(static_cast<int>(assoc_cost)) + " associates";
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
            if(valarray[3] >= 5000){
                response = "🤫" + mention + ", we run a tight ship here, we can't just hire people willy nilly!";
                return response;
            }
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
        if(item == "item_front"){
            valarray[3] -= assoc_cost;
            valarray[15] += amount;
            if(amount > 1){
                item = "fronts";
            }else{
                item = "front";
            }
            // [15] is front
        }

        // cout << "testing if event.reply is like a return\n";
        response = "✅" + mention + " you bought " + std::to_string(amount) + " " + item + " for $`" + doub_to_str(total_cost) + "`"; 
        return response;
    }
    return response;
}



std::string action_upgrade(Dictionary& dict, std::string username, std::string business, int tier){
    int base;
    std::string msg;
    valType* userArr = getEntry(dict, username);
    if(business == "speaks"){
        business = "speakeasy";
        base = 6; // 6, 7, 8, 9
    }
    if(business == "casino"){
        base = 11; // 11, 12, 13, 14
    }

    int t1 = base + 1; // tier 1 could be more seating
    int t2 = base + 2; // tier 2 increased quality of liquor
    int t3 = base + 3; // tier 3 - more personel means ability to serve more people, more money security

    double t1_cost; double t2_cost; double t3_cost;
    if(base == 6){
        t1_cost = 3000;
        t2_cost = 3000;
        t3_cost = 6000;
    }
    if(base == 11){
        t1_cost = 22500;
        t2_cost = 22500;
        t3_cost = 45000;
    }

    if(tier == 1){
        if(userArr[base] < 1){ // check for base business
            msg = "❌You need a base " + business + " to get that upgrade";
            return msg;
        }
        if(userArr[3] < 50){ // check for adequate assocs
            int assoc_count = static_cast<int>(userArr[3]);
            msg = "❌You need to have 50 spare asscoiates to handle upgraded operations at your " + business;
            msg += "\nYou currently have `" + std::to_string(assoc_count) + "` to spare.";
            return msg;
        }
        if(userArr[0] + userArr[1] >= t1_cost){ // check for adequate funds
            if(userArr[0] - t1_cost >= 0){
                userArr[0] -= t1_cost;
            }else{
                t1_cost -= userArr[0];
                userArr[0] = 0;
                userArr[1] -= t1_cost;
            }
            userArr[t1] += 1; userArr[base] -= 1;
            msg = "✅You have successfully added increased seating to this " + business;
            return msg; 
        }else{
            double funds_needed = t1_cost - (userArr[0] + userArr[1]);
            msg = "❌You do not have enough funds to upgrade your " + business;
            msg += "\nYou need $`" + doub_to_str(funds_needed) + "` more.";
            return msg;
        }
    }
    if(tier == 2){
        if(userArr[t1] < 1){ // check for base business
            msg = "❌You need a tier 1 " + business + " to get that upgrade";
            return msg;
        }
        if(userArr[3] < 50){ // check for adequate assocs
            int assoc_count = static_cast<int>(userArr[3]);
            msg = "❌You need to have 50 spare asscoiates to handle upgraded operations at your " + business;
            msg += "\nYou currently have `" + std::to_string(assoc_count) + "` to spare.";
            return msg;
        }
        if(userArr[0] + userArr[1] >= t2_cost){ // check for adequate funds
            if(userArr[0] - t2_cost >= 0){
                userArr[0] -= t2_cost;
            }else{
                t2_cost -= userArr[0];
                userArr[0] = 0;
                userArr[1] -= t2_cost;
            }
            userArr[t2] += 1; userArr[t1] -= 1;
            msg = "✅You now serve higher quality liquor at this " + business;
            return msg; 
        }else{
            double funds_needed = t2_cost - (userArr[0] + userArr[1]);
            msg = "❌You do not have enough funds to upgrade your " + business;
            msg += "\nYou need $`" + doub_to_str(funds_needed) + "` more.";
            return msg;
        }
    }
    if(tier == 3){
        if(userArr[t2] < 1){ // check for base business
            msg = "❌You need a tier 2 " + business + " to get that upgrade";
            return msg;
        }
        if(userArr[3] < 50){ // check for adequate assocs
            int assoc_count = static_cast<int>(userArr[3]);
            msg = "❌You need to have 50 spare asscoiates to handle upgraded operations at your " + business;
            msg += "\nYou currently have `" + std::to_string(assoc_count) + "` to spare.";
            return msg;
        }
        if(userArr[0] + userArr[1] >= t3_cost){ // check for adequate funds
            if(userArr[0] - t3_cost >= 0){
                userArr[0] -= t3_cost;
            }else{
                t3_cost -= userArr[0];
                userArr[0] = 0;
                userArr[1] -= t3_cost;
            }
            userArr[t3] += 1; userArr[t2] -=1;
            msg = "✅With extra guards stationed here, you can handle more revenue at this " + business;
            return msg; 
        }else{
            double funds_needed = t3_cost - (userArr[0] + userArr[1]);
            msg = "❌You do not have enough funds to upgrade your " + business;
            msg += "\nYou need $`" + doub_to_str(funds_needed) + "` more.";
            return msg;
        }
    }
    return msg;
}