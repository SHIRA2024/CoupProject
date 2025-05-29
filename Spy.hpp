#pragma once

#include "Player.hpp"

namespace coup {

/**
 * @brief Spy - A player role that can block another player's ability to use 'arrest' on their next turn.
 * 
 * Special ability:
 * - On their turn, the Spy may choose a target player.
 * - That player will be blocked from using the 'arrest' action on their next turn.
 * - The Spy receives an extra turn immediately after using this ability.
 */
class Spy : public Player {
public:
    /**
     * @brief Constructs a Spy player and registers them in the game.
     * @param game Reference to the game instance.
     * @param name The name of the player.
     */
    Spy(Game& game, const std::string& name);

    /**
     * @brief Blocks the target player from using 'arrest' on their next turn.
     * 
     * Conditions:
     * - Must be the Spy's turn.
     * - The target player must be alive.
     * 
     * Effects:
     * - Prevents the target from performing an 'arrest' action next turn.
     * - Grants the Spy an extra turn.
     * 
     * @param target Reference to the player whose 'arrest' will be blocked.
     * @throws std::runtime_error if conditions are not met.
     */
    void blockArrestNextTurn(Player& target); 
};

} // namespace coup
