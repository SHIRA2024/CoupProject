// email: shiraba01@gmail.com
#include "Merchant.hpp"
#include <stdexcept>

namespace coup {

/**
 * @brief Constructor for the Merchant class.
 * Initializes a player with the "Merchant" role.
 *
 * @param game Reference to the Game object the player belongs to.
 * @param name Name of the player.
 */
Merchant::Merchant(Game& game, const std::string& name)
    : Player(game, name) {
    role = "Merchant";
}

/**
 * @brief Called at the start of the Merchant's turn.
 * If the Merchant has 3 or more coins at the beginning of their turn,
 * they receive 1 bonus coin.
 */
void Merchant::onTurnStart() {
    if (getCoins() >= 3) {
        addCoins(1);
    }
}

} // namespace coup
