// email: shiraba01@gmail.com
#pragma once

#include "Player.hpp"

namespace coup {

// The Governor class represents a special role in the game.
// Inherits from the Player base class.
class Governor : public Player {
public:
    // Constructor: initializes a Governor player with a reference to the game and a name.
    Governor(Game& game, const std::string& name);

    // Destructor: uses the default implementation from Player.
    virtual ~Governor() override = default;

    // Special ability:
    // Allows the Governor to block a "tax" action performed by another player.
    // Preconditions:
    // - The target must be alive.
    // - The target must not be the Governor themselves.
    // - The target’s last action must be "tax".
    // - The target must have at least 2 coins to remove.
    void blockTax(Player& target);
};

} // namespace coup
