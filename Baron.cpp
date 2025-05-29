// email: shiraba01@gmail.com
#include "Baron.hpp"
#include <stdexcept>
#include "Game.hpp"

namespace coup {

/**
 * @brief Constructs a Baron player.
 * 
 * Initializes a player with the role of "Baron".
 * 
 * @param game Reference to the game the player is participating in.
 * @param name The name of the player.
 */
Baron::Baron(Game& game, const std::string& name)
    : Player(game, name) {
    role = "Baron";
}

/**
 * @brief Performs the "invest" special action for the Baron.
 * 
 * This action allows the Baron to pay 3 coins and receive 6 coins from the bank,
 * provided the following conditions are met:
 * - It is the Baron's turn.
 * - The Baron has fewer than 10 coins.
 * - The Baron has at least 3 coins to invest.
 * - The bank has at least 6 coins available.
 * 
 * After performing the action, the turn advances to the next player.
 * 
 * @throws std::runtime_error If it's not the Baron's turn.
 * @throws std::runtime_error If the Baron has 10 or more coins (must coup).
 * @throws std::runtime_error If the Baron has fewer than 3 coins.
 * @throws std::runtime_error If the bank has fewer than 6 coins.
 */
void Baron::invest() {
    if(game.turn() != getName()) {
        throw std::runtime_error("It's not " + getName() + "'s turn.");
    }
    if (getCoins() >= 10) {
        throw std::runtime_error(getName() + " has 10 or more coins and must perform a coup.");
    }

    if (getCoins() < 3) {
        throw std::runtime_error(getName() + " does not have enough coins to invest.");
    }

    removeCoins(3);

    if (game.getBankCoins() < 6) {
        throw std::runtime_error("Not enough coins in the bank to complete the investment.");
    }

    addCoins(6);

    setLastAction("invest");
    game.advanceTurn();
}

} // namespace coup
