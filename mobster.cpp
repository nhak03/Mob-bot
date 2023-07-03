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
const double still_prod = 4;
const double speak_price = 2000.00;
const double casino_base_max = 50000; const double casino_base_def = 100000;
const double casino_t1_max = 100000; const double casino_t1_def = 150000;
const double casino_t2_max = 200000; const double casino_t2_def = 300000;
const double casino_t3_max = 1000000; const double casino_t3_def = 1000000;


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
    if(item == "item_speaks"){
        total_cost = speak_price * amount;
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
