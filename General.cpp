
// email: shiraba01@gmail.com
#include "General.hpp"
#include "Game.hpp"
#include <stdexcept>

namespace coup {

// Constructor for the General role
// Initializes a new General with a reference to the game and the player's name.
// Also sets the role name to "General".
General::General(Game& game, const std::string& name)
    : Player(game, name) {
    role = "General";
}

// Special ability: block a coup that was just executed against a player.
// This action restores the eliminated player (target) and costs the General 5 coins.
// It can only be performed on the General's turn.
void General::blockCoup(Player& target) {
    // Ensure it is the General's turn
    if (game.turn() != getName()) {
        throw std::runtime_error("It's not " + getName() + "'s turn.");
    }

    // Ensure the General has at least 5 coins to pay for the block
    if (getCoins() < 5) {
        throw std::runtime_error(getName() + " does not have enough coins to block the coup.");
    }

    // Deduct 5 coins from the General
    removeCoins(5);

    // Revive the player who was just eliminated in the coup
    target.enliven();

    // Record the action as the last action taken by the General
    setLastAction("blockCoup");
}

} // namespace coup
