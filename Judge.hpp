// email: shiraba01@gmail.com
#pragma once

#include "Player.hpp"

namespace coup {

// The Judge class represents a special role in the game.
// A Judge can block a bribe action made by another player, causing that player
// to lose 4 coins and canceling the granted extra turn.
class Judge : public Player {
public:
    // Constructor:
    // Initializes a Judge with the given game reference and player name.
    Judge(Game& game, const std::string& name);

    // Special ability:
    // Cancels a bribe performed by another player.
    // Effects:
    // - The target player does NOT regain the 4 coins spent on the bribe.
    // - The target's extra turn (if granted) is canceled.
    // Preconditions:
    // - The target must be alive.
    // - The target's last action must be "bribe".
    // - The target must not be the Judge themselves.
    void blockBribe(Player& target);
};

} // namespace coup
