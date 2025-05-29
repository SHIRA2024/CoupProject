
// email: shiraba01@gmail.com
#pragma once
#include <vector>
#include <string>
#include <stdexcept>

namespace coup {

class Player;

class Game {
private:
    // List of all players in the game (only active ones are returned via players())
    std::vector<Player*> players_list;

    // Index of the player whose turn it is
    size_t current_turn_index = 0;

    // Number of coins available in the central bank of the game
    int coinBank = 100;

    // Pointer to the player who is currently the target of a pending coup (for blocking logic)
    Player* pendingCoupTarget = nullptr;

public:
    /**
     * @brief Adds a player to the game.
     * 
     * @param player Pointer to the player to be added.
     * @throws std::runtime_error if too many players are added (checked in implementation).
     */
    void add_player(Player* player);

    /**
     * @brief Returns the names of all currently active (alive) players in the game.
     * 
     * @return std::vector<std::string> List of player names.
     */
    std::vector<std::string> players() const;

    /**
     * @brief Gets the name of the player whose turn it currently is.
     * 
     * @return std::string The current player's name.
     * @throws std::runtime_error if there are no players or no active players.
     */
    std::string turn() const;

    /**
     * @brief Eliminates the specified player from the game.
     * 
     * @param target Reference to the player to eliminate.
     */
    void eliminate_player(Player& target);

    /**
     * @brief Returns the name of the winning player.
     * 
     * @return std::string The name of the only remaining player.
     * @throws std::runtime_error if the game is not yet over.
     */
    std::string winner() const;

    /**
     * @brief Returns the current number of coins in the bank.
     * 
     * @return int Number of coins left in the bank.
     */
    int getBankCoins() const;

    /**
     * @brief Takes coins from the bank.
     * 
     * @param amount Number of coins to take.
     * @throws std::runtime_error if there are not enough coins in the bank.
     */
    void takeCoins(int amount);

    /**
     * @brief Returns coins to the bank.
     * 
     * @param amount Number of coins to return.
     */
    void returnCoins(int amount);

    /**
     * @brief Advances the turn to the next active player.
     * 
     * Handles skipped players and special turn logic.
     */
    void advanceTurn(); 

    /**
     * @brief Marks a player as the current target of a coup action (used for blocking).
     * 
     * @param target Pointer to the player targeted by the coup.
     */
    void setPendingCoup(Player* target);

    /**
     * @brief Clears the pending coup target.
     */
    void clearPendingCoup();
    
    /**
     * @brief Checks if a coup against the given player has been blocked.
     * 
     * @param target Pointer to the player to check.
     * @return true if the coup is blocked; false otherwise.
     */
    bool isCoupBlocked(Player* target) const;
};

} // namespace coup
