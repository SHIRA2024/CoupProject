// email: shiraba01@gmail.com
#include "Player.hpp"
#include "Game.hpp"

#include <stdexcept>

using namespace std;

namespace coup {

/**
 * @brief Constructs a new Player and registers them to the game.
 *
 * Initializes a new Player with the given name and sets their coin count to 0.
 * The player is automatically added to the game instance provided.
 *
 * @param game Reference to the Game object the player will participate in.
 * @param name The name of the player.
 */

Player::Player(Game& game, const std::string& name)
    : name(name), coins(0), game(game) {
   
    game.add_player(this);
}

/**
 * @brief Returns the name of the player.
 *
 * @return The player's name as a std::string.
 */

std::string Player::getName() const {
    return name;
}

/**
 * @brief Returns the role of the player (e.g., "Spy", "Governor").
 *
 * @return The player's role as a std::string.
 */

std::string Player::getRole() const {
    return role;
}

/**
 * @brief Returns the current number of coins the player has.
 *
 * @return The player's coin count as an integer.
 */

int Player::getCoins() const {
    return coins;
}

/**
 * @brief Adds a specified number of coins to the player and deducts the same amount from the game's bank.
 *
 * @param amount The number of coins to add. Must be non-negative and less than or equal to the number of coins available in the bank.
 * @throws std::invalid_argument if the amount is negative or if there are not enough coins in the bank.
 */

void Player::addCoins(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Cannot add negative amount of coins.");
    }
     if (game.getBankCoins()  < amount) {
        throw std::invalid_argument("there are not enough coins in the bank.");
    }
    coins += amount;
    game.takeCoins(amount);

}

/**
 * @brief Adds a specified number of coins directly to the player without affecting the bank.
 *
 * This method is used for internal adjustments where the bank is not involved (e.g., coin transfers between players).
 *
 * @param amount The number of coins to add. Must be non-negative.
 * @throws std::invalid_argument if the amount is negative.
 */

void Player::onlyAddCoinsToPlayer(int amount) {
    if (amount < 0) {
        throw std::invalid_argument("Cannot add negative amount of coins.");
    }
     
    coins += amount;
   
}


/**
 * @brief Removes a specified number of coins from the player and returns them to the bank.
 *
 * This function ensures that the player has enough coins before removing them.
 * The removed coins are returned to the central coin bank managed by the Game object.
 *
 * @param amount The number of coins to remove.
 * @throws std::runtime_error if the player does not have enough coins.
 */

void Player::removeCoins(int amount) {
    if (amount > coins) {
        throw std::runtime_error("Not enough coins.");
    }
    coins -= amount;
    game.returnCoins(amount);
}

/**
 * @brief Removes a specified number of coins from the player without interacting with the coin bank.
 *
 * This method is used when coins need to be deducted from the player without returning them to the bank.
 * It validates that the player has enough coins before performing the deduction.
 *
 * @param amount The number of coins to remove.
 * @throws std::runtime_error if the player does not have enough coins.
 */

void Player::onlyRemoveCoinsFromPlayer(int amount) {
    if (amount > coins) {
        throw std::runtime_error("Not enough coins.");
    }
    coins -= amount;
   
}

/**
 * @brief Executes the "gather" action for the player.
 *
 * This action allows the player to collect 1 coin from the bank.
 * It includes several validations:
 * - The action must be performed during the player's turn.
 * - The player cannot perform this action if they have 10 or more coins (must perform a coup instead).
 * - A player who has been sanctioned is not allowed to use the gather action.
 *
 * Upon successful execution, the player's coin count is increased by 1,
 * their last action is recorded as "gather", and the turn is advanced to the next player.
 *
 * @throws std::runtime_error if it is not the player's turn.
 * @throws std::runtime_error if the player has 10 or more coins.
 * @throws std::runtime_error if the player is sanctioned.
 */

void Player::gather() {

    if(game.turn() != getName()) {
        throw std::runtime_error("It's not " + getName() + "'s turn.");
    }
    if (coins >= 10) {
        throw std::runtime_error(getName() + " has 10 or more coins and must perform a coup.");
    }
     if (is_sanctioned == true) {
        throw std::runtime_error(getName() + " has been sanctioned and therefore can't use the gather action.");
    }
    addCoins(1);
    setLastAction("gather");
    game.advanceTurn();
}

/**
 * @brief Executes the "tax" action for the player.
 *
 * The "tax" action allows the player to collect coins from the bank:
 * - Normally, the player receives 2 coins.
 * - If the player is a "Governor", they receive 3 coins.
 *
 * Conditions for using the action:
 * - It must be the player's turn.
 * - The player cannot have 10 or more coins (must perform a coup instead).
 * - A sanctioned player is not allowed to perform the tax action.
 *
 * Upon success, the appropriate number of coins is added to the player,
 * the action is recorded as "tax", and the turn proceeds to the next player.
 *
 * @throws std::runtime_error if it is not the player's turn.
 * @throws std::runtime_error if the player has 10 or more coins.
 * @throws std::runtime_error if the player is sanctioned.
 */

void Player::tax() {
    if(game.turn() != getName()) {
        throw std::runtime_error("It's not " + getName() + "'s turn.");
    }
    if (coins >= 10) {
        throw std::runtime_error(getName() + " has 10 or more coins and must perform a coup.");
    }
    if (is_sanctioned == true) {
        throw std::runtime_error(getName() + " has been sanctioned and therefore can't use the tax action.");
    }
     int cost = (getRole() == "Governor") ? 3 : 2;
    


    addCoins(cost);
    setLastAction("tax");
    game.advanceTurn();
}

/**
 * @brief Executes the "bribe" action for the player.
 *
 * The "bribe" action allows the player to pay 4 coins in order to receive
 * an immediate extra turn instead of passing the turn to the next player.
 *
 * Conditions for using the action:
 * - It must be the player's turn.
 * - The player cannot have 10 or more coins (must perform a coup instead).
 * - The player must have at least 4 coins.
 *
 * Upon success, 4 coins are removed from the player, the action is recorded
 * as "bribe", and the player receives an additional turn (grantExtraTurn).
 *
 * @throws std::runtime_error if it is not the player's turn.
 * @throws std::runtime_error if the player has 10 or more coins.
 * @throws std::runtime_error if the player has fewer than 4 coins.
 */

void Player::bribe() {
    if(game.turn() != getName()) {
        throw std::runtime_error("It's not " + getName() + "'s turn.");
    }
    if (coins >= 10) {
        throw std::runtime_error(getName() + " has 10 or more coins and must perform a coup.");
    }
    if (coins < 4) {
        throw std::runtime_error("Not enough coins to bribe.");
    }
    removeCoins(4);
    setLastAction("bribe");
    grantExtraTurn();
}

/**
 * @brief Performs the "arrest" action on a target player.
 *
 * This action allows a player to steal 1 coin from a target player (or 2 coins if the target is a Merchant),
 * under specific conditions. The arrested player loses coins, and the acting player gains 1 coin.
 *
 * Special cases:
 * - If the target is a Merchant, they lose 2 coins (returned to the bank), and the acting player gains nothing.
 * - If the target is a General, they gain 1 bonus coin in return.
 *
 * Conditions for using this action:
 * - It must be the player's turn.
 * - The player must have fewer than 10 coins.
 * - The target must be a different, alive player.
 * - The same player cannot be arrested two turns in a row.
 * - The target must have at least 1 coin (2 if they are a Merchant).
 * - The player must not be blocked from using "arrest" this turn.
 *
 * Effects:
 * - Coins are transferred or removed according to the roles.
 * - Action is recorded as "arrest".
 * - Turn advances to the next player.
 *
 * @param target The player to arrest.
 * @throws std::runtime_error if any game rule or condition is violated.
 */

void Player::arrest(Player& target) {
    if(game.turn() != getName()) {
        throw std::runtime_error("It's not " + getName() + "'s turn.");
    }
    if (coins >= 10) {
        throw std::runtime_error(getName() + " has 10 or more coins and must perform a coup.");
    }
    if (&target == this) {
        throw std::runtime_error("Cannot arrest yourself.");
    }
    if (last_arrested == &target) {
        throw std::runtime_error("Cannot arrest the same player twice in a row.");
    }
    if (!target.isAlive()) {
        throw std::runtime_error("Cannot arrest an eliminated player.");
    }
    if (target.getCoins() < 1) {
        throw std::runtime_error("Target has no coins to take.");
    }
    if (!isArrestEnabled()) {
        throw std::runtime_error(getName() + " is blocked from using arrest this turn.");
    }

     if (target.getRole() == "Merchant") {
        if (target.getCoins() < 2) {
            throw std::runtime_error("Merchant does not have 2 coins to pay after arrest.");
        }
        target.removeCoins(2);
        
    }
    else {

    target.onlyRemoveCoinsFromPlayer(1);
    onlyAddCoinsToPlayer(1);

        if (target.getRole() == "General") {
            target.addCoins(1);
            }

      }
    last_arrested = &target;
    setLastAction("arrest");
    
    game.advanceTurn();
}

/**
 * @brief Performs the "sanction" action on a target player.
 *
 * This action allows a player to apply a sanction on another player, preventing them from using certain actions
 * (e.g., gather or tax) on their next turn. The cost of the action varies based on the target's role.
 *
 * Special cases:
 * - If the target is a Judge, the sanction costs 4 coins.
 * - If the target is a Baron, they receive 1 bonus coin after being sanctioned.
 *
 * Conditions for using this action:
 * - It must be the player's turn.
 * - The player must have fewer than 10 coins.
 * - The player cannot sanction themselves.
 * - The target must be alive.
 * - The player must have enough coins to cover the sanction cost.
 *
 * Effects:
 * - Deducts coins from the acting player.
 * - Marks the target as sanctioned.
 * - Action is recorded as "sanction".
 * - Advances the turn to the next player.
 *
 * @param target The player to sanction.
 * @throws std::runtime_error if any rule or condition is violated.
 */

void Player::sanction(Player& target) {
    if(game.turn() != getName()) {
        throw std::runtime_error("It's not " + getName() + "'s turn.");
    }
    if (coins >= 10) {
        throw std::runtime_error(getName() + " has 10 or more coins and must perform a coup.");
    }
  
    if (&target == this) {
        throw std::runtime_error("Cannot sanction yourself.");
    }
    if (!target.isAlive()) {
        throw std::runtime_error("Cannot sanction an eliminated player.");
    }
     int cost = (target.getRole() == "Judge") ? 4 : 3;
     if (coins < cost) {
        throw std::runtime_error("Not enough coins to apply sanction.");
    }

    removeCoins(cost);
    target.setSanction(true);
    setLastAction("sanction");
     if (target.getRole() == "Baron") {
        target.addCoins(1);
    }
    game.advanceTurn();
}

/**
 * @brief Performs the "coup" action on a target player.
 *
 * The coup action permanently eliminates another player from the game.
 * It is an irreversible action and costs exactly 7 coins.
 *
 * Conditions for using this action:
 * - The target must not be the same as the acting player.
 * - The target must still be alive.
 * - The acting player must have at least 7 coins.
 *
 * Effects:
 * - Deducts 7 coins from the acting player.
 * - Eliminates the target player from the game.
 * - Sets the last action to "coup".
 * - Records the target in `pendingCoupTarget` for potential blocking.
 * - Advances the game turn to the next player.
 *
 * @param target The player to be eliminated.
 * @throws std::runtime_error if any rule or condition is violated.
 */

void Player::coup(Player& target) {
    
    if (&target == this) {
        throw std::runtime_error("Cannot coup yourself.");
    }
    if (!target.isAlive()) {
        throw std::runtime_error("Target already eliminated.");
    }
    if (coins < 7) {
        throw std::runtime_error("Not enough coins to perform a coup.");
    }
    removeCoins(7);
    
    game.eliminate_player(target);
    setLastAction("coup");
    game.setPendingCoup(&target);
    game.advanceTurn();
}



/**
 * @brief Check if the player is still active in the game.
 *
 * @return true if the player has not been eliminated; false otherwise.
 */
bool Player::isAlive() const {
    return alive;
}

/**
 * @brief Mark the player as eliminated.
 *
 * Sets the player's alive status to false, indicating they are no longer active in the game.
 */
void Player::eliminate() {
    alive = false;
}

/**
 * @brief Revive the player.
 *
 * Sets the player's alive status to true, indicating they are active again in the game.
 */
void Player::enliven() {
    alive = false;
}

/**
 * @brief Checks whether the player is currently sanctioned.
 *
 * A sanctioned player is restricted from performing certain actions like `gather` or `tax`.
 *
 * @return true if the player is sanctioned, false otherwise.
 */
bool Player::isSanctioned() const {
    return is_sanctioned;
}

/**
 * @brief Sets the sanction status of the player.
 *
 * If set to true, the player is considered sanctioned and may be restricted from performing
 * specific actions such as `gather` or `tax`. If false, the player is no longer under sanction.
 *
 * @param value True to apply sanction, false to remove it.
 */
void Player::setSanction(bool value) {
    is_sanctioned = value;
}




} // namespace coup
