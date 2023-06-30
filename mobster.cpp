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