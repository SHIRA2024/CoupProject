// email: shiraba01@gmail.com
#include "Spy.hpp"
#include "Game.hpp"
#include <iostream>
#include <stdexcept>

namespace coup {

/**
 * @brief Constructs a Spy player and registers them in the game.
 * @param game Reference to the game instance.
 * @param name The name of the player.
 */
Spy::Spy(Game& game, const std::string& name)
    : Player(game, name) {
    role = "Spy"; // Set the role of this player to "Spy"
}

/**
 * @brief Special ability of the Spy: block a target from using 'arrest' on their next turn.
 * 
 * Preconditions:
 * - It must be the Spy's turn.
 * - The target must be alive.
 * 
 * Behavior:
 * - Prevents the target from using the 'arrest' action in their next turn.
 * - Stores the target's coin count (reserved for potential use).
 * - Grants the Spy an extra turn.
 * 
 * @param target The player whose ability to arrest will be blocked.
 * @throws std::runtime_error if the action is performed out of turn or if the target is not alive.
 */
void Spy::blockArrestNextTurn(Player& target) {
    if (game.turn() != getName()) {
        throw std::runtime_error("It's not " + getName() + "'s turn.");
    }

    if (!target.isAlive()) {
        throw std::runtime_error("Cannot block an eliminated player.");
    }

    // Store the target's coin count for potential future use(commented out to avoid compiler warnings)
    //  int numberOfCoins = target.getCoins();

    // Disable the target's ability to arrest on their next turn
    target.disableArrest();

    // Record this action as the Spy's last action
    setLastAction("blockArrest");

    // Grant the Spy an additional turn
    grantExtraTurn();
}

} // namespace coup
