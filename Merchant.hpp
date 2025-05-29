#pragma once

#include "Player.hpp"

namespace coup {

/**
 * @class Merchant
 * @brief A special role in the game. If the Merchant has 3 or more coins
 * at the start of their turn, they automatically gain 1 bonus coin.
 */
class Merchant : public Player {
public:
    /**
     * @brief Constructor for the Merchant class.
     * Initializes a player with the "Merchant" role.
     *
     * @param game Reference to the game instance.
     * @param name The name of the player.
     */
    Merchant(Game& game, const std::string& name);

    /**
     * @brief Called at the start of the player's turn.
     * If the Merchant has 3 or more coins, they receive 1 extra coin.
     */
    void onTurnStart() override;
};

} // namespace coup
