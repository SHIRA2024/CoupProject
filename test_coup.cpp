#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"
#include "Game.hpp"
#include "Spy.hpp"
#include "Governor.hpp"
#include "Baron.hpp"

using namespace coup;
using namespace std;

TEST_CASE("Simple flow") {
    Game game;
    Governor gov(game, "Alice");
    Spy spy(game, "Bob");
    Baron baron(game, "Charlie");

    // Round 1: Each player gathers 1 coin
    gov.gather();    // Alice's turn
    spy.gather();    // Bob's turn
    baron.gather();  // Charlie's turn

    // Round 2
    gov.gather();
    spy.gather();
    baron.gather();

    // Round 3
    gov.gather();
    spy.gather();
    baron.gather();

    // Check each player has 3 coins
    CHECK(gov.getCoins() == 3);
    CHECK(spy.getCoins() == 3);
    CHECK(baron.getCoins() == 3);

    // Prepare Baron to perform a sanction: he needs 6 coins
    gov.gather();     // Alice
    spy.gather();     // Bob
    baron.gather();   // Charlie now has 4 coins

    gov.gather();     // Alice
    spy.gather();     // Bob
    baron.gather();   // Charlie now has 5 coins

    gov.gather();     // Alice
    spy.gather();     // Bob
    baron.gather();   // Charlie now has 6 coins

    gov.gather();     // Alice
    spy.gather();     // Bob
    baron.sanction(spy); // Charlie performs sanction on Bob (Spy)

    CHECK(baron.getCoins() == 3); // Sanction should cost 3 coins

    // Next round - more actions
    gov.gather();          // Alice
    spy.arrest(baron);     // Bob arrests Charlie
    baron.gather();        // Charlie gathers again
    CHECK(baron.getCoins() == 3); // Check Charlie has 3 coins again
}
