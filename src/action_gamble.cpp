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
            msg += "\nApplied a `" + doub_to_str(player_house_bonus) + "`x multiplier for gambling at " + house + "'s casino";
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

std::string action_crash(Dictionary& dict, std::string player, double bet, double guess, std::string house){
    valType* playerArr = getEntry(dict, player);
    std::string msg;
    if(playerArr[0] + playerArr[1] < bet){
        msg = "❌You don't have enough money to place a bet of $`" + doub_to_str(bet) + "`";
        return msg;
    }

    double real_mult = crash();
    if(house == "bot"){
        if(guess > real_mult){
            // loss
            msg = "🚀~~~>💥🔥\n";
            msg += "You lost $`" + doub_to_str(bet) + "` in that crash bet.\n";
            if(playerArr[0] - bet >= 0){
                // if can subtract from pocket fully, do so
                playerArr[0] -= bet;
            }else{
                // otherwise, subtract from pocket, and then from bank
                double bet_orig = bet;
                bet -= playerArr[0];
                playerArr[0] = 0;
                playerArr[1] -= bet;
                bet = bet_orig;
            }
            msg += "Your guess: `" + doub_to_str(guess) + "` || ";
            msg += "Real multiplier: `" + doub_to_str(real_mult) + "`"; 
            return msg; 
        }else{
            // win
            playerArr[0] -= bet; // doesn't matter if neg or not since it will be given back
            double winnings = bet * guess;
            playerArr[0] += winnings;
            msg = "🚀~~~>✨🎆\n";
            msg += "You won $`" + doub_to_str(winnings) + "` in that crash bet!\n";
            msg += "Your guess: `" + doub_to_str(guess) + "` || ";
            msg += "Real multiplier: `" + doub_to_str(real_mult) + "`"; 
            return msg;
        }
    }else{
        // non-bot house
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

        if(guess > real_mult){ // player lost, house won
            // loss
            msg = "🚀~~~>💥🔥\n";
            msg += "You lost $`" + doub_to_str(bet) + "` in that crash bet.\n";
            if(playerArr[0] - bet >= 0){
                // if can subtract from pocket fully, do so
                playerArr[0] -= bet;
            }else{
                // otherwise, subtract from pocket, and then from bank
                double bet_holder = bet;
                bet -= playerArr[0];
                playerArr[0] = 0;
                playerArr[1] -= bet;
                bet = bet_holder;
            }
            msg += "Your guess: `" + doub_to_str(guess) + "` || ";
            msg += "Real multiplier: `" + doub_to_str(real_mult) + "`"; 
            if(house_check == 2){
                houseArr[0] += bet;
            }else{
                houseArr[10] += bet;
            }    
        }else{
            // win
            double winnings = bet * real_mult;
            playerArr[0] += (winnings * player_house_bonus);
            houseArr[10] -= winnings;
            msg = "🚀~~~>✨🎆\n";
            msg += "You won $`" + doub_to_str(winnings * player_house_bonus) + "` in that crash bet!\n";
            msg += "Your guess: `" + doub_to_str(guess) + "` || ";
            msg += "Real multiplier: `" + doub_to_str(real_mult) + "`\n"; 
            msg += "Applied a `" + doub_to_str(player_house_bonus) + "`x multiplier for gambling at " + house + "'s casino";
        }
    }
    return msg;
}

std::string action_casino(Dictionary& dict, std::string owner, std::string mention, std::string action){
    valType* ownerArr = getEntry(dict, owner);
    std::string msg;
    if(ownerArr[11] + ownerArr [12] + ownerArr[13] + ownerArr[14] < 1){
        msg = "❌ You do not own any casinos to manage.";
        return msg;
    }

    if(action == "casino_view"){
        int total_owned = static_cast<int>(ownerArr[11] + ownerArr[12] + ownerArr[13] + ownerArr[14]);
        msg = "🎰🎲 " + mention + "'s Casino Ledger: 👑🃏\n";
        msg += "🎰Owned: " + std::to_string(total_owned) + "\n";
        
        if(ownerArr[10] < 0){
            msg += "📉Current Deficit: $`" + doub_to_str(ownerArr[10]) + "`";
        }
        else{
            msg += "💵Current Vault: $`" + doub_to_str(ownerArr[10]) + "`";
        }

        double maxDef = (casino_base_def * ownerArr[11]) + (casino_t1_def * ownerArr[12]);
        maxDef += (casino_t2_def * ownerArr[13]) + (casino_t3_def * ownerArr[14]);
        double maxVault = (casino_base_vault * ownerArr[11]) + (casino_t1_vault * ownerArr[12]);
        maxVault += (casino_t2_vault * ownerArr[13]) + (casino_t3_vault * ownerArr[14]);

        msg += "\n📉Max Deficit: $`" + doub_to_str(maxDef) + "`";
        msg += "\n🔒Max Vault: $`" + doub_to_str(maxVault) + "`";
        return msg;
    }

    if(action == "casino_restock"){
        if(ownerArr[10] < 0){
            // attempt to clear deficit
            // deficit so [10] is negative
            if(ownerArr[0] + ownerArr[1] + ownerArr[10] < 0){
                msg = "You don't have enough funds to clear your casino deficit.";
                return msg;
            }
            // else:
            if(ownerArr[0] + ownerArr[10] >= 0){ // if can pay from pocket, do so
                ownerArr[0] += ownerArr[10];
                ownerArr[10] = 0;
            }else{
                ownerArr[10] += ownerArr[0]; // pay as much from pocket
                ownerArr[0] = 0;
                ownerArr[1] += ownerArr[10]; // pay rest from bank
                ownerArr[10] = 0;
            }
            msg = "✅Your casino debts have been successfully paid off.";
        }
        else{
            // attempt to fill the vault
            // [10] is positive here
            double maxVault = (casino_base_vault * ownerArr[11]) + (casino_t1_vault * ownerArr[12]);
            maxVault += (casino_t2_vault * ownerArr[13]) + (casino_t3_vault * ownerArr[14]);

            if(ownerArr[10] >= maxVault){
                msg = "❌Your casino vaults are already full.";
                return msg;
            }
            if(ownerArr[0] + ownerArr[1] < maxVault){
                double needed = maxVault - (ownerArr[0] + ownerArr[1]);
                msg = "❌You have insufficient funds to stock your casino vaults.\n";
                msg += "You need $`" + doub_to_str(needed) + "` to stock your vaults.";
                return msg;
            }
            // else:
            double maxVault_copy = maxVault;
            if(ownerArr[0] - maxVault >= 0){ // pay fully from pocket if possible
                ownerArr[0] -= maxVault;
                ownerArr[10] += maxVault;
            }else{
                maxVault -= ownerArr[0]; // pay what can from pocket
                ownerArr[0] = 0;
                ownerArr[1] -= maxVault; // pay rest from bank
                ownerArr[10] += maxVault_copy; 
            }
            msg = "✅👛-->🔒💵🔒\n";
            msg += "You have stocked your casino vaults with an additional $`" + doub_to_str(maxVault_copy) + "`"; 
        }
        return msg;
    }

    if(action == "casino_cashout"){
        // check to see if there's cash to take out;
        if(ownerArr[10] <= 0){
            msg = "❌You don't have any available cash to pull from your casino vaults.";
        }else{
            ownerArr[0] += ownerArr[10];
            msg = "🔒💵🔒-->👛\n";
            msg += "You have successfully cashed out $`" + doub_to_str(ownerArr[10]) + "` from your casino vaults.";
            ownerArr[10] = 0;
        }
    }
    return msg;
}