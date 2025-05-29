#pragma once

#include "Player.hpp"

namespace coup {

// The General class represents a special role in the game,
// inheriting from the Player base class.
// A General has the ability to block a coup by paying 5 coins.
class General : public Player {
public:
    // Constructor: creates a General and registers them in the game.
    // Parameters:
    // - game: the game instance this player is part of
    // - name: the name of the player
    General(Game& game, const std::string& name);

    // Special ability: pay 5 coins to block a coup that was just executed.
    // The target player (who was eliminated) is brought back to life.
    // Throws an exception if:
    // - it's not the General's turn
    // - the General does not have at least 5 coins
    // Parameters:
    // - target: the player to revive
    void blockCoup(Player& target);
};

} // namespace coup
