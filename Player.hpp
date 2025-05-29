// email: shiraba01@gmail.com
#pragma once
#include <string>
#include <stdexcept>

namespace coup {

class Game;

class Player {
protected:
    std::string name;                  
    std::string role;                 
    int coins;                         // Number of coins the player has
    Game& game;                        // Reference to the game this player belongs to
    Player* last_arrested = nullptr;  // Pointer to the last player arrested by this player
    bool alive = true;                // Is the player still in the game
    bool is_sanctioned = false;       // Is the player blocked from economic actions
    std::string lastAction = "";      // The last action performed by the player
    bool hasExtraTurn = false;        // Does the player have an extra turn this round
    bool canUseArrest = true;         // True if the player is allowed to use arrest this turn

public:
    // Constructor
    Player(Game& game, const std::string& name);

    // Virtual destructor
    virtual ~Player() = default;

    // Getters
    std::string getName() const;      // Returns the player's name
    std::string getRole() const;      // Returns the player's role
    int getCoins() const;             // Returns the number of coins the player has

    // Coin operations
    void addCoins(int amount);        // Adds coins to the player and removes from the bank
    void onlyAddCoinsToPlayer(int amount);        // Only adds coins to the player
    void removeCoins(int amount);     // Removes coins from the player and adds to the bank.
    void onlyRemoveCoinsFromPlayer(int amount);     // Onlt removes coins from the player 


    // Main actions (can be overridden by derived classes)
    virtual void gather();                         // +1 coin from the bank
    virtual void tax();                            // +2 coins (or 3 for Governor)
    virtual void bribe();                          // Pay 4 coins to get another action this turn
    virtual void arrest(Player& target);           // Take 1 coin from another player
    virtual void sanction(Player& target);         // Block another player's economic actions
    virtual void coup(Player& target);             // Eliminate another player

    // Status checkers
    bool isAlive() const;                          // Returns whether the player is still active
    void eliminate();                              // Marks the player as eliminated
    void enliven();                              // Marks the player as alive
    bool isSanctioned() const;                     // Checks if the player is under sanction
    void setSanction(bool value);                  // Sets sanction status

    // Last action tracking
    const std::string& getLastAction() const { return lastAction; }  // Get last action
    void setLastAction(const std::string& action) { lastAction = action; } // Set last action

   

    // Extra turn control
    bool hasPendingExtraTurn() const { return hasExtraTurn; } // Check if the player has an extra turn that they haven't used yet
    void grantExtraTurn() { hasExtraTurn = true; }            // Grant extra turn
    void useExtraTurn() { hasExtraTurn = false; }             // Consume extra turn
    void disableArrest() { canUseArrest = false; }            // Disables the player's ability to use arrest (used by Spy)
    void enableArrest() { canUseArrest = true; }               // Enables the player's ability to use arrest (used at the start of their turn)
    bool isArrestEnabled() const { return canUseArrest; }     // Checks if the player is currently allowed to use arrest

    // Called at the start of a player's turn — default does nothing
    virtual void onTurnStart() {}


};

} // namespace coup
