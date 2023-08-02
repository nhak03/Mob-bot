#include <iostream>
#include <random>
#include <cstdlib>
#include "action_work.h"

// inclusion of wisdom boost

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
    double boost;
    boost = userArr[18] * 0.10 + 1;
    if(boost < 1){
        boost = 1;
    }
    revenue = revenue * boost;
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

// returns "pass" or "some descriptive fail msg"
// based on whether the still gets raided, or blows up
// will be used for still random events
std::string still_raid(valType* valarray){
    index_checker();
    if(valarray[4] <= 5){
        return "pass";
    }
}

std::string action_work(Dictionary& dict, std::string username, std::string mention, std::string labor_type){
    valType* valarray = getEntry(dict, username);
    std::string msg;
    if(labor_type == "default"){
        double check = (min_wage * apply_wisdom(valarray))
        valarray[0] += check;
        msg = "✅ You've earned $`" + doub_to_str(check) + "`\n";
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
        double boost = apply_wisdom(valarray);
        msg = "💸🎰🎲👑🃏💸\n";
        msg += "You've earned $`" + doub_to_str((resArray[0] * boost)) + "` in revenue from your casinos!";
        valarray[0] += (resArray[0] * boost);
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

const double front_income = 6000; // $3000 per week, for 2 weeks (26 * 6000) --> yearly $156,000
const double bracket1 = 154; // (26 * ) --> yearly $4,000
const double bracket2 = 231; // (26 * ) --> yearly $6,000
const double bracket3 = 385; // (26 * ) --> yearly $10,000
const double bracket4 = 770; // (26 * ) --> yearly $20,000
const double bracket5 = 1154; // (26* ) --> yearly $30,000
const double bracket6 = 1539; // (26* ) --> yearly $40,000
const double bracket7 = 1923; // (26* ) --> yearly $50,000
const double bracket8 = 2308; // (26* ) --> yearly $60,000
const double bracket9 = 2692; // yearly $70k
const double bracket10 = 3846; // yearly $100k
const double bracket12 = 5769; // yearly $150k
const double bracket13 = 7692; // yearly $200k
const double bracket14 = 11538; // 300k
const double bracket15 = 15384; // 400k
const double bracket16 = 19230; // 500k
const double bracket17 = 28846; // 750k
const double bracket18 = 38462; // 1 million

// returns a percentage of tax that'll be taken
double tax_rate(double income){
    double rate = 0.08;

    if(income >= bracket2){
        rate = 0.09;
    }
    if(income >= bracket3){
        rate = 0.10;
    }
    if(income >= bracket4){
        rate = 0.16;
    }
    if(income >= bracket5){
        rate = 0.21;
    }
    if(income >= bracket6){
        rate = 0.26;
    }
    if(income >= bracket7){
        rate = 0.31;
    }
    if(income >= bracket8){
        rate = 0.36;
    }
    if(income >= bracket9){
        rate = 0.41;
    }
    if(income >= bracket10){
        rate = 0.56;
    }
    if(income >= bracket12){
        rate = 0.57;
    }
    if(income >= bracket13){
        rate = 0.58;
    }
    if(income >= bracket14){
        rate = 0.59;
    }
    if(income >= bracket15){
        rate = 0.60;
    }
    if(income >= bracket16){
        rate = 0.61;
    }
    if(income >= bracket17){
        rate = 0.62;
    }
    if(income >= bracket18){
        rate = 0.63;
    }
    return rate; 
}



std::string action_deposit(Dictionary& dict, std::string username, std::string mention, double amount){
    std::string msg;
    valType* userArr = getEntry(dict, username);

    if(amount > userArr[0]){
        msg = mention + " you don't have enough funds for that deposit.";
        return msg;
    }
    
    // step 1 audit
    // low income b1 and b2 don't need audits
    if(amount > bracket2 || userArr[16] >= 26){
        // 50-50 to audit
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<int> distr(1, 10);
        int outcome = distr(gen);
        if((outcome <= 3)  || (userArr[16] >= 26)){
            // we audit
            // audit every 26 deposits or randomly
            if(userArr[16] >= 26){
                // regular audit
                msg = "Your account has been selected for a regular audit.\n";
                userArr[16] = 0; // reset the counter

                if((userArr[17] + amount) >= (26 * 1.50 * bracket2)){
                    if(userArr[17] <= (26 * front_income * userArr[15])){
                    // pass if they have appropriate fronts as sources
                    msg += "✅Your deposit and bank account have passed the audit.\n";
                    userArr[17] = 0.00; // reset the amount counter
                    }else{
                        // fail
                        msg += "❌You have failed this audit.\n";
                        double fine = 0.50 * (userArr[17] + amount);
                        msg += "👮The IRS seized your deposit of: $`" + doub_to_str(amount) + "`\n";
                        msg += "🔒They also siezed $`" + doub_to_str(userArr[17]) + "` in illict revenue\n";
                        msg += "🧾You are forced to pay a fine of $`" + doub_to_str(fine) + "`\n";
                        userArr[0] -= (amount + fine + userArr[17]);
                        // subtract the deposit, fine, and illicit money
                        if(userArr[0] < 0){
                            // pull from bank (since bank should have negatives)
                            userArr[0] = 0.00;
                            userArr[1] -= (amount + fine + userArr[17]);
                        }
                        userArr[17] = 0.00; // reset the amount counter
                        return msg;
                    }
                }else{
                    // pass
                    msg += "✅Your account has passed the audit.\n";
                    userArr[17] = 0.00;
                }
            }else{
                // random chance audit
                msg = "Your deposit has been randomly selected for an audit.\n";
                if((amount >= (0.25) * userArr[0]) || amount >= (2 * bracket2)){
                    // if the amount trying to deposit is more than 25% of your pocket
                    // or >= double bracket2
                    // look to see if have fronts, index 15
                    if(amount <= (front_income * userArr[15])){
                        // pass if they have appropriate fronts as sources
                        msg += "✅Your deposit has passed the audit.\n";
                    }else{
                        // fail (dirty money)
                        msg += "❌You have failed this audit.\n";
                        double fine = 0.50 * amount;
                        msg += "👮The IRS seized your deposit of: $`" + doub_to_str(amount) + "`\n";
                        msg += "🧾You are forced to pay a fine of $`" + doub_to_str(fine) + "`\n";
                        userArr[0] -= (amount + fine);
                        if(userArr[0] < 0){
                            // pull from bank (since bank should have negatives)
                            userArr[0] = 0.00;
                            userArr[1] -= (amount + fine);
                        }
                        return msg;
                    }
                }else{
                    // pass 
                    msg += "✅Your deposit has passed the audit.\n";
                    userArr[17] = 0.00;
                }
            }
        }
    }

    // assuming here, you passed audit or didn't get one, now pay tax on that deposit
    double rate = tax_rate(amount);
    double taxed = rate * amount;
    userArr[0] -= amount;
    amount -= taxed;
    msg += "🧾$`" + doub_to_str(taxed) + "` of your deposit has been taken as income tax.\n";
    userArr[1] += amount;
    msg += "🏦$`" + doub_to_str(amount) + "` has been added to your bank account.";

    // userArr[16] is the deposit counter
    // userArr[17] is the amount tracker
    userArr[16] += 1;
    userArr[17] += amount;

    return msg;
}