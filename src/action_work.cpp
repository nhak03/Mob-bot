#include <iostream>
#include <random>
#include <cstdlib>
#include "action_work.h"

const double speak_cost = 2.32; // liters
const double speak_rate = 25.00; 
// above is avg consumption per hour assuming base of 17 patrons
// per hour over 6 hours (~100 patrons per day)
// about ~$25 per liter sold
const double base_speak_rev = speak_cost * speak_rate;
// higher level speaks need more alc to run, but sell price
// is increased
const double t1_speak_rev = (speak_cost * 4) * (speak_rate + 10);
const double t2_speak_rev = (speak_cost * 7) * (speak_rate + 20);
const double t3_speak_rev = (speak_cost * 10) * (speak_rate + 30);

double speak_revenue(valType* userArr){
    // outputs a revenue -> edits balance and user_alc directly
    // returns 0 if no speakeasies owned
    // returns -1 if no alcohol to sell
    // returns -2 if not enough alc to operate
    double revenue = 0;
    // base is [6], t1 is [7], t2 is [8], t3 is [9]
    if(userArr[6] < 1 && userArr[7] < 1 && userArr[8] < 1 && userArr [9] < 1){
        // if they have none, return val of 0
        // client can handle appropriate error msg
        return revenue;
    }

    double user_alc = userArr[5];
    if(user_alc <= 0){ // if no alc, return -1
        revenue = -1;
        return revenue; 
    }

    // check to see if have enough alc
    double alc_cost = (userArr[6] * speak_cost) + (4 * userArr[7] * speak_cost);
    // t1 uses 4x more alc than base, t2 uses 7x more, t3 uses 10x more
    alc_cost += (7 * userArr[8] * speak_cost) + (10 * userArr[9] * speak_cost);

    if(user_alc < alc_cost){ // if not enough return -2
        return alc_cost;
    }

    // otherwise we have enough to operate;
    revenue = (userArr[6] * base_speak_rev) + (userArr[7] * t1_speak_rev);
    revenue += (userArr[8] * t2_speak_rev) + (userArr[9] * t3_speak_rev);
    userArr[5] -= alc_cost;
    userArr[0] += revenue;
    // calculate revenue and return it;
    return revenue;
}

const double base_casi_rev = 232.00;
const double t1_casi_rev = 1000.00;
const double t2_casi_rev = 1933.00;
const double t3_casi_rev = 3866.00;

const double base_casi_alc = 2;
const double t1_casi_alc = 10;
const double t2_casi_alc = 50;
const double t3_casi_alc = 75;

// returns 0 if player has no casinos
// returns revenue based on player casinos
// index 0 is revenue
// 1 is multiplier
// 2 is alc consumed for multiplier
// void type since inputted resArray is modified directly
void casino_revenue(valType* valarray, double* resArray){
    // return 0 if no casinos
    if(valarray[11] + valarray[12] + valarray[13] + valarray[14] <= 0){
        resArray[0] = 0;
        resArray[1] = 0;
        resArray[2] = 0;
        return;
    }
    double revenue = (valarray[11] * base_casi_rev) + (valarray[12] * t1_casi_rev);
    revenue += (valarray[13] * t2_casi_rev) + (valarray[14] * t3_casi_rev);
    
    // apply a multiplier if player has enough alcohol for all establishments
    double multiplier = 1.00;
    double required_alc = (valarray[11] * base_casi_alc) + (valarray[12] * t1_casi_alc);
    required_alc += (valarray[13] * t2_casi_alc) + (valarray[14] * t3_casi_alc);
    if(valarray[5] >= required_alc){
        // if they have enough alcohol apply a bonus;
        if(valarray[11] > 0){
            multiplier = 1.25;
        }
        if(valarray[12] > 0){
            multiplier = 1.50;
        }
        if(valarray[13] > 0){
            multiplier = 2.25;
        }
        if(valarray[14] > 0){
            multiplier = 3.00;
        }
        valarray[5] -= required_alc;
    }
    revenue = revenue * multiplier;
    resArray[0] = revenue;
    resArray[1] = multiplier;
    resArray[2] = required_alc;

}

std::string action_work(Dictionary& dict, std::string username, std::string mention, std::string labor_type){
    valType* valarray = getEntry(dict, username);
    std::string msg;
    if(labor_type == "default"){
        valarray[0] += min_wage;
        msg = "✅ You've earned $`" + doub_to_str(min_wage) + "`\n";
        msg += "💵 You have $`" + doub_to_str(valarray[0]) + "` in earnings!";
    }
    if(labor_type == "distill"){
        if(valarray[4] < 1){ // if i have no stills, then I can't distill
            msg = mention + " you need stills to be able to distill moonshine";
        }else{
            double product = valarray[4] * still_prod;
            valarray[5] += product;
            msg = mention + " you made " + doub_to_str(product) + " liters of moonshine";
        }
    }
    if(labor_type == "speak_op"){
        double result = speak_revenue(valarray);
        if(result == 0){
            msg = "You have no speakeasies to operate.";
            return msg;
        }
        if(result == -1){
            msg = "You have no alcohol sell at your speakeasies.";
            return msg;
        }
        if(result < -1){
            msg = "You don't have enough alcohol to operate your speakeasies.\n";
            msg += "You need `" + doub_to_str(result) + "` liters minimum.";
            return msg;
        }
        msg = "🍾🥃🥴\n";
        msg += "You've earned $`" + doub_to_str(result) + "` in revenue from your speakeasies!";
    }
    if(labor_type == "casino_op"){
        double resArray[] = {0, 0, 0}; // revenue, multiplier, alc consumed
        double* ptrArr = resArray;
        casino_revenue(valarray, ptrArr);
        if(resArray[0] == 0){
            msg = "You have no casinos to operate.";
            return msg;
        }
        msg = "💸🎰🎲👑🃏💸\n";
        msg += "You've earned $`" + doub_to_str(resArray[0]) + "` in revenue from your casinos!";
        valarray[0] += resArray[0];
        if(resArray[1] > 1.00){
            msg += "\nRevenue multiplier: `" + doub_to_str(resArray[1]) + "` || ";
            msg += "Alcohol consumed(L): `" + doub_to_str(resArray[2]) + "`";
        }
    }
    return msg;
}

bool rob_odds(double robber, double victim){
    // 1-5 and 5-6
    std::random_device rd;
    std::mt19937 gen(rd());

    double x = 0.5;
    double ratio = robber/victim;
    if(ratio < 1){ // for when victim has more
      ratio = victim/robber;
      x = ratio/2;
       std::bernoulli_distribution successChance(x);

      if(successChance(gen)){
          // cout << "Hit the " << x << " chance to fail" << endl;
          return false;
      }else{
          // cout << "Hit the " << (1-x) << " chance to succeed" << endl;
          return true;
      }
    }

    // when robber has more
    x = ratio/2;

    std::bernoulli_distribution successChance(x);

    if(successChance(gen)){
        // cout << "Hit the " << x << " chance to succeed" << endl;
        return true;
    }else{
        // cout << "Hit the " << (1-x) << " chance to fail" << endl;
        return false;
    }
}

int assoc_loss(double assocs){
    // helper function that determines how many assocs the robber will
    // lose on a robbery fail
    // usage: userArray[3] -= assoc_loss(userArray[3]);
    if(assocs <= 10){
        return assocs;
    }
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(20, 50);
    int percent_loss = distribution(gen);

    double percent_to_take = percent_loss/100.00;
    // we want to only subtract integer amounts (can't have fractions of assocs)
    int dead_assocs = percent_to_take * assocs;
    return dead_assocs;
}

std::string action_rob(Dictionary& dict, std::string robber, std::string victim, std::string victim_mention, int steal_type){
    valType* robberArray = getEntry(dict, robber);
    valType* victimArray = getEntry(dict, victim);
    std::string response;

    if(rob_odds(robberArray[3], victimArray[3]) == true){ //robbery success
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distribution(10, 20);
        int percent_taken = distribution(gen);
        double percent_to_take = percent_taken/100.00; // returns some decimal from .10 to .20
        double amount_stole = victimArray[steal_type] * percent_to_take;
        victimArray[steal_type] -= amount_stole;
        robberArray[steal_type] += amount_stole;
        std::string item;
        if(steal_type == 0){
            item = " dollars💸";
        }else{
            item = " liters of moonshine🍾";
        }
        response = "✅You've robbed " + victim_mention + " and stole `" + doub_to_str(amount_stole) + "`" + item;
        // implement later, random chance to lose associates even when success
    }else{
        // false, robbery failed
        int assocs_lost = assoc_loss(robberArray[3]);
        robberArray[3] -= assocs_lost;
        response = "❌You lost " + std::to_string(assocs_lost) + " associates 💀 in a botched robbery on " + victim_mention;
    }
    return response;
}