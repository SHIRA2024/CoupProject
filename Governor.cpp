// email: shiraba01@gmail.com
#include "Governor.hpp"
#include <iostream>
#include <stdexcept>
#include "Game.hpp"

namespace coup {

// Constructor for Governor.
// Initializes the player with the "Governor" role.
Governor::Governor(Game& game, const std::string& name)
    : Player(game, name) {
    role = "Governor";
}

// Special ability: block the "tax" action performed by another player.
// If the last action of the target was "tax", this method removes 2 coins
// from the target player as a way to undo the benefit they gained from taxing.
// Throws an exception if:
// - the target player is not alive
// - the target is the same as the Governor (self-blocking is not allowed)
// - the target's last action was not "tax"
// - the target does not have at least 2 coins to remove
void Governor::blockTax(Player& target) {
    if (!target.isAlive()) {
        throw std::runtime_error("Cannot undo an eliminated player.");
    }

    if (&target == this) {
        throw std::runtime_error("Player cannot undo his own action.");
    }

    if (target.getLastAction() != "tax") {
        throw std::runtime_error("Governor can only undo tax actions.");
    }

    if (target.getCoins() < 2) {
        throw std::runtime_error("Target does not have enough coins to undo tax.");
    }

    target.removeCoins(2);            
                
    setLastAction("blockTax");          
}

} // namespace coup
