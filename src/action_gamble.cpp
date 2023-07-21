#include <random>
#include <cstdlib>
#include "action_gamble.h"

const double player_house_bonus = 1.10;

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

const double casino_base_def = -100000.00; const double casino_base_vault = 50000.00;
const double casino_t1_def = -150000.00; const double casino_t1_vault = 100000.00;
const double casino_t2_def = -300000.00; const double casino_t2_vault = 200000.00;
const double casino_t3_def = -1000000.00; const double casino_t3_vault = 1000000.00;
// returns 0 if operational
// returns -1 if user owns no casinos
// returns 1 if user has too much house debt
// returns 2 if user's vault is full (still operational)
int player_house_check(valType* houseArr){ 
    double houseBal = houseArr[10]; // index 10 is houseBalance
    // indexes 11, 12, 13, 14 are casinos
    double maxDef = (casino_base_def * houseArr[11]) + (casino_t1_def * houseArr[12]);
    maxDef += (casino_t2_def * houseArr[13]) + (casino_t3_def * houseArr[14]);
    double maxVault = (casino_base_vault * houseArr[11]) + (casino_t1_vault * houseArr[12]);
    maxVault += (casino_t2_vault * houseArr[13]) + (casino_t3_vault * houseArr[14]);

    if(houseArr[11] + houseArr[12] + houseArr[13] + houseArr[15] < 1){
        // check casino ownership
        return (-1);
    }
    if(houseBal < maxDef){
        // too much house debt
        return 1;
    }
    if(houseBal >= maxVault){
        // casino vault is full
        return 2;
    }
    return 0;
}

std::string action_roulette(Dictionary& dict, std::string player, double bet, std::string color, std::string house){
    valType* playerArr = getEntry(dict, player);
    std::string msg;
    if(playerArr[0] + playerArr[1] < bet){
        msg = "❌You don't have enough money to place a bet of $`" + doub_to_str(bet) + "`";
        return msg;
    }

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

    if(house == "bot"){
        if(color == fin_color){
            // user won
            if(fin_color == "green"){
                bet *= 36.00;
            }
            playerArr[0] += bet;
            msg = roulette_win_msg(color);
            msg += "\nYou won $`" + doub_to_str(bet) + "` in that roulette spin!";
        }
        else{
            msg = roulette_loss_msg(color);
            msg += "\nYou lost $`" + doub_to_str(bet) + "` in that roulette spin.";
            if(bet - playerArr[0] < 0){
                //pay from pocket and then bank
                bet -= playerArr[0];
                playerArr[0] = 0;
                playerArr[1] -= bet;
            }
            else{
                //or just from pocket
                playerArr[0] -= bet;
            }
        }
    }else{
        valType* houseArr = getEntry(dict, house);
        int house_check = player_house_check(houseArr);
        if(house_check == -1){
            msg = house + " does not own any casinos.";
            return msg;
        }
        // 2nd, check to see if the casino has enough money to pay bets
        if(house_check == 1){
            msg = house + "'s casino doesn't have enough funds to pay out winnings.";
            return msg;
        }
        // else, it's a 0 or 2 (operational or operational but full vault)
        if(color == fin_color){
            // user won
            if(fin_color == "green"){
                bet *= 36.00;
            }
            playerArr[0] += (bet * player_house_bonus);
            houseArr[10] -= bet;
            msg = roulette_win_msg(color);
            msg += "\nYou won $`" + doub_to_str(bet) + "` in that roulette spin!";
            msg += "\nApplied a `" + doub_to_str(player_house_bonus) + "`x multiplier for gambling at " + house;
        }
        else{
            msg = roulette_loss_msg(color);
            msg += "\nYou lost $`" + doub_to_str(bet) + "` in that roulette spin.";
            if(house_check == -2){
                // if vault is full, add cash to pocket
                houseArr[0] += bet;
            }
            else{
                // else add cash to vault
                houseArr[10] += bet;
            }
            if(bet - playerArr[0] < 0){
                //pay from pocket and then bank
                bet -= playerArr[0];
                playerArr[0] = 0;
                playerArr[1] -= bet;
            }
            else{
                //or just from pocket
                playerArr[0] -= bet;
            }
        }
    }

    return msg;
}