#include "action_sell.h"

// example of an embed

// dpp::embed sample = dpp::embed();
// sample.set_title("Sample title");
// dpp::message hello;
// dpp::embed sample = action_sell(userDict, who.username, product, volume);
// hello.add_embed(sample); 

// dpp::embed action_sell(Dictionary& dict, std::string username, std::string product, double volume){
//     dpp::embed msg = dpp::embed();
//     msg.set_title("Sample from action_sell");
    
//     return msg; 
// }

const double shine_sell = 2.00;

std::string action_sell(Dictionary& dict, std::string username, std::string product, double volume){
    // index_checker();
    std::string msg;
    valType* userArr = getEntry(dict, username);
    int index = 5;
    double price;
    if(product == "item_moonshine"){
        index = 5;
        product = "liters of moonshine 🍾";
        price = shine_sell;
    }

    // check if they have the required amount
    if(userArr[index] < volume){
        msg = "❌ You don't have enough " + product + " to make that sale!";
        return msg;
    }

    // else they do
    double profit = volume * price;
    userArr[0] += profit;
    userArr[index] -= volume;
    msg = "✅🤝🏷️\n";
    msg += "Sale Successful! Sold " + doub_to_str(volume) + " " + product + " for $`" + doub_to_str(profit) + "`";
    
    return msg;
}