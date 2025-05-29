#include "Game.hpp"
#include "Player.hpp"

using namespace std;

namespace coup {

/**
 * @brief Adds a new player to the game.
 * 
 * @param player Pointer to the Player to add.
 * @throws runtime_error if the maximum number of players (6) is exceeded.
 */
void Game::add_player(Player* player) {
    if (players_list.size() >= 6) {
        throw std::runtime_error("Maximum number of players reached.");
    }
    players_list.push_back(player);
}

/**
 * @brief Returns a list of names of all currently active (alive) players.
 * 
 * @return vector<string> Names of alive players.
 */
vector<string> Game::players() const {
    vector<string> active_names;
    for (Player* p : players_list) {
        if (p->isAlive()) {
            active_names.push_back(p->getName());
        }
    }
    return active_names;
}

/**
 * @brief Returns the name of the player whose turn it currently is.
 * 
 * @return string Name of the current player.
 * @throws runtime_error if there are no players or no active players.
 */
string Game::turn() const {
    if (players_list.empty()) {
        throw std::runtime_error("No players in the game.");
    }

    // Make sure current_turn_index points to a living player
    size_t idx = current_turn_index;
    size_t count = 0;
    while (!players_list[idx]->isAlive()) {
        idx = (idx + 1) % players_list.size();
        if (++count >= players_list.size()) {
            throw std::runtime_error("No active players.");
        }
    }

    return players_list[idx]->getName();
}

/**
 * @brief Marks a player as eliminated.
 * 
 * @param target Reference to the Player to eliminate.
 */
void Game::eliminate_player(Player& target) {
    target.eliminate();
}

/**
 * @brief Returns the name of the winner (if only one player is alive).
 * 
 * @return string Name of the winner.
 * @throws runtime_error if more than one player is still alive.
 */
string Game::winner() const {
    string winner_name;
    int alive_count = 0;

    for (Player* p : players_list) {
        if (p->isAlive()) {
            alive_count++;
            winner_name = p->getName();
        }
    }

    if (alive_count != 1) {
        throw std::runtime_error("The game is not over yet.");
    }

    return winner_name;
}

/**
 * @brief Returns the current number of coins in the bank.
 * 
 * @return int Number of coins in the bank.
 */
int Game::getBankCoins() const {
    return coinBank;
}

/**
 * @brief Deducts coins from the bank.
 * 
 * @param amount Number of coins to take.
 * @throws runtime_error if there are not enough coins in the bank.
 */
void Game::takeCoins(int amount) {
    if (amount > coinBank) {
        throw std::runtime_error("Not enough coins in the bank.");
    }
    coinBank -= amount;
}

/**
 * @brief Returns coins back to the bank.
 * 
 * @param amount Number of coins to return.
 */
void Game::returnCoins(int amount) {
    coinBank += amount;
}

/**
 * @brief Advances the turn to the next active player.
 * 
 * If the current player has a pending extra turn, they continue instead.
 * Otherwise, moves to the next alive player and starts their turn.
 */
void Game::advanceTurn() {
    if (players_list.empty()) {
        return;
    }

    Player* current = players_list[current_turn_index];

    // If the player has a pending extra turn, use it and stay on the same turn
    if (current->hasPendingExtraTurn()) {
        current->useExtraTurn(); // Consume the extra turn
        current->onTurnStart();
        return;
    }

    // Re-enable arrest at the start of the new player's turn
    current->enableArrest();

    // Move to the next alive player
    size_t count = 0;
    do {
        current_turn_index = (current_turn_index + 1) % players_list.size();
        count++;
    } while (!players_list[current_turn_index]->isAlive() && count <= players_list.size());

    players_list[current_turn_index]->onTurnStart();
}

/**
 * @brief Sets a player as the target of a pending coup.
 * 
 * @param target Pointer to the player who may be blocked from coup.
 */
void Game::setPendingCoup(Player* target) {
    pendingCoupTarget = target;
}

/**
 * @brief Clears any pending coup target.
 */
void Game::clearPendingCoup() {
    pendingCoupTarget = nullptr;
}

/**
 * @brief Checks whether the coup on a specific player is currently blocked.
 * 
 * @param target Pointer to the player being checked.
 * @return true if the player's coup is blocked; false otherwise.
 */
bool Game::isCoupBlocked(Player* target) const {
    return pendingCoupTarget == target;
}

} // namespace coup
