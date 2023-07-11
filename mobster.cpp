#include <iostream>
#include <random>
#include <cstdlib>

// since the userData is loaded into a dictionary in memory
// and read/write operations are handled in mybot.cpp
// we can have dictionary based operations here (altering state of dictionary)
// such as buy, sleep, work (work already inside mybot.cpp)
using namespace std;

const double min_wage = 0.50;
const double val_hold = -3.1415;
const double gun_price = 30.00;
const double assoc_price = 30.00;
const double still_price = 250.00;
const double shine_price = 5.00;
const double still_prod = 4;
const double speak_price = 2000.00;
const double casino_base_max = 50000; const double casino_base_def = 100000;
const double casino_t1_max = 100000; const double casino_t1_def = 150000;
const double casino_t2_max = 200000; const double casino_t2_def = 300000;
const double casino_t3_max = 1000000; const double casino_t3_def = 1000000;
const double casino_price = 15000.00;
const double player_house_bonus = 1.1; // 10% winnings bonus 


void hello(){
    std::cout << "Hello World\n";
}

void goodbye(){
    std::cout << "Goodbye world\n";
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

    return total_cost;
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

double bot_roulette_spin(std::string color, double bet){
    // returns a positive or negative number depending on whether player won or lost bet
    // on a roulette spin
    // usage: balance += roulette_spin(color, bet);
    // works in both cases where win and lose since roulette_spin() returns pos or neg, depending on win/loss
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(1, 38);
    int outcome = distribution(gen);
    std::string fin_color;
    if(outcome >= 1 && outcome <= 18){
        fin_color = "black";
    }
    if(outcome >= 19 && outcome <= 36){
        fin_color = "red";
    }
    if(outcome > 36){
        fin_color = "green";
    }
    // this is roulette for bot, red and black are more unfair, slightly higher chance
    // to hit green though, for regular roulette, do 18-18-1
    double winnings = bet;
    if(fin_color == color){
        if(fin_color == "green"){ // if hits green, bonus added
            winnings *= 35;
            return winnings;
        }
        return winnings; // otherwise return pos num
    }else{
        // player lost
        winnings *= -1; // make it negative
        return winnings;
    }
}

std::string roulette_win_msg(std::string color){
    std::random_device rd;
    std::mt19937 gen(rd());

    std::string win_msg = "...............||...............\n";
    // ...............||...............
    // 🟥 ⬛ 🟥 ⬛ 🟥
    // ...............^................
    if(color == "red"){
        std::string red1 = "🟥 ⬛ 🟥 ⬛ 🟥\n";
        std::string red2 = "🟥 ⬛ 🟥 ⬛ 🟩\n";
        std::string red3 = "🟩 ⬛ 🟥 ⬛ 🟥\n";
        std::vector<std::string> stringSet = {red1, red2, red3};
        std::uniform_int_distribution<int> distribution(0, stringSet.size() - 1);
        int randomIndex = distribution(gen);
        // Retrieve the randomly selected string
        std::string randomString = stringSet[randomIndex];
        win_msg += randomString;
    }
    if(color == "black"){
        std::string black1 = "⬛ 🟥 ⬛ 🟥 ⬛\n";
        std::string black2 = "⬛ 🟥 ⬛ 🟥 🟩\n";
        std::string black3 = "🟩 🟥 ⬛ 🟥 ⬛\n";
        std::vector<std::string> stringSet = {black1, black2, black3};
        std::uniform_int_distribution<int> distribution(0, stringSet.size() - 1);
        int randomIndex = distribution(gen);
        // Retrieve the randomly selected string
        std::string randomString = stringSet[randomIndex];
        win_msg += randomString;
    }
    if(color == "green"){
        win_msg += "🟥 ⬛ 🟩 🟥 ⬛\n";
    }
    win_msg += "...............^................";
    return win_msg;
}

std::string roulette_loss_msg(std::string color){
    std::random_device rd;
    std::mt19937 gen(rd());
    std::string lose_msg = "...............||...............\n";
    if(color == "red"){
        std::string black1 = "⬛ 🟥 ⬛ 🟥 ⬛\n";
        std::string black2 = "⬛ 🟥 ⬛ 🟥 🟩\n";
        std::string black3 = "🟩 🟥 ⬛ 🟥 ⬛\n";
        std::string n_red1 = "🟥 🟩 ⬛ 🟥 ⬛\n";
        std::string n_red2 = "🟥 ⬛ 🟩 🟥 ⬛\n";
        std::vector<std::string> stringSet = {black1, black2, black3, n_red1, n_red2};
        std::uniform_int_distribution<int> distribution(0, stringSet.size() - 1);
        int randomIndex = distribution(gen);
        // Retrieve the randomly selected string
        std::string randomString = stringSet[randomIndex];
        lose_msg += randomString;
    }
    if(color == "black"){
        std::string red1 = "🟥 ⬛ 🟥 ⬛ 🟥\n";
        std::string red2 = "🟥 ⬛ 🟥 ⬛ 🟩\n";
        std::string red3 = "🟩 ⬛ 🟥 ⬛ 🟥\n";
        std::string n_b1 = "🟥 ⬛ 🟩 🟥 ⬛\n";
        std::string n_b2 = "🟥 ⬛ 🟥 🟩 ⬛\n";
        std::vector<std::string> stringSet = {red1, red2, red3, n_b1, n_b2};
        std::uniform_int_distribution<int> distribution(0, stringSet.size() - 1);
        int randomIndex = distribution(gen);
        // Retrieve the randomly selected string
        std::string randomString = stringSet[randomIndex];
        lose_msg += randomString;
    }
    if(color == "green"){
        std::string n_b2 = "🟥 ⬛ 🟥 🟩 ⬛\n";
        std::string n_red1 = "🟥 🟩 ⬛ 🟥 ⬛\n";
        std::string black3 = "🟩 🟥 ⬛ 🟥 ⬛\n";
        std::string red2 = "🟥 ⬛ 🟥 ⬛ 🟩\n";
        std::string black1 = "⬛ 🟥 ⬛ 🟥 ⬛\n";
        std::string red1 = "🟥 ⬛ 🟥 ⬛ 🟥\n";
        std::vector<std::string> stringSet = {red1, red2, black3, n_red1, n_b2, black1};
        std::uniform_int_distribution<int> distribution(0, stringSet.size() - 1);
        int randomIndex = distribution(gen);
        // Retrieve the randomly selected string
        std::string randomString = stringSet[randomIndex];
        lose_msg += randomString;
    }

    lose_msg += "...............^................";
    return lose_msg;
}

bool player_house_check(valType* houseArr){ 
    // given that a player has a casino, return true or false if it's operational
    // based on deficit values
}

std::string roulette_player_house(valType* playerArr, valType* houseArr, std::string house_name){ 
    // this function is to handle operating roulette with another
    // player as the house
    // returns a string, that will be the message sent in event.reply()
    std::string response = "Player houses not implemented yet.";
    // first, check to see if that player has a house
    if(houseArr[11] < 1 && houseArr[12] < 1 && houseArr[13] < 1 && houseArr[14] < 1){
        // if each is less than 1, then no houses
        response = house_name + " does not have any casinos for you to gamble at.";
        return response;
    }else{
        // the player as house is valid
        // calculate gambling operations
    }

    return response;
}

const double max_mult = 1000000.00;
const int max_int = static_cast<int>(max_mult) - 1;

double crash(){ // returns a value that is the multiplier
    // using formula
    // mult = [(E*100-H)/(E-H)]/100
    
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> distribution(0, max_int);
    int some_H = distribution(gen);

    double mult = (max_mult*100 - some_H) / (max_mult-some_H);
    mult = mult / 100;

    return round(mult * 100) / 100;
}

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
