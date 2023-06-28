#include <iostream>

// since the userData is loaded into a dictionary in memory
// and read/write operations are handled in mybot.cpp
// we can have dictionary based operations here (altering state of dictionary)
// such as buy, sleep, work (work already inside mybot.cpp)

const double min_wage = 0.50;
const double val_hold = -3.1415;
const double gun_price = 30.00;
const double assoc_price = 30.00;
const double still_price = 250.00;
const double still_prod = 4;

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
    return total_cost;
}

double assoc_check(std::string item, int64_t amount){
    double total_cost = 0;
    if(item == "item_still"){
        total_cost = 2 * amount;
    }

    return total_cost;
}
