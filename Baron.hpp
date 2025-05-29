#pragma once

#include "Player.hpp"

namespace coup {

/**
 * @class Baron
 * @brief A special role in the Coup game, derived from Player.
 * 
 * The Baron has a unique ability to "invest": spend 3 coins to receive 6 coins from the bank,
 * assuming the bank has enough coins and the player has fewer than 10 coins.
 */
class Baron : public Player {
public:
    /**
     * @brief Constructs a Baron player.
     * 
     * @param game Reference to the game instance.
     * @param name Name of the player.
     */
    Baron(Game& game, const std::string& name);

    /**
     * @brief Special ability: Invest 3 coins to receive 6 from the bank.
     * 
     * Can only be performed if:
     * - It's the player's turn.
     * - The player has fewer than 10 coins.
     * - The player has at least 3 coins.
     * - The bank has at least 6 coins available.
     * 
     * Throws an exception otherwise.
     */
    void invest();
};

} // namespace coup
