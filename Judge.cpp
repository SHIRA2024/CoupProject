#include "Judge.hpp"
#include "Game.hpp"
#include <stdexcept>

namespace coup {

// Constructor: initializes a Judge player with a reference to the game and a name.
Judge::Judge(Game& game, const std::string& name)
    : Player(game, name) {
    role = "Judge";
}

// Special ability:
// Cancels a bribe action performed by another player.
// Preconditions:
// - The target must be alive.
// - The target must not be the Judge themselves.
// - The target's last action must be "bribe".
//
// Behavior:
// - Does NOT refund the 4 coins used for the bribe.
// - Cancels the extra turn granted by the bribe.
// - Records "blockBribe" as the last action of the Judge.
void Judge::blockBribe(Player& target) {
    if (!target.isAlive()) {
        throw std::runtime_error("Cannot undo action of an eliminated player.");
    }

    if (&target == this) {
        throw std::runtime_error("Player cannot undo their own action.");
    }

    if (target.getLastAction() != "bribe") {
        throw std::runtime_error("Judge can only undo bribe actions.");
    }

    // Do NOT return the coins — the bribe is cancelled and coins are lost
    target.useExtraTurn();  // Cancel the extra turn (if granted)
    setLastAction("blockBribe");
}

}
