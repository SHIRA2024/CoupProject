# Coup Game in C++ with SFML

This project is a simplified graphical version of the **Coup** board game, implemented in C++ using the **SFML** graphics library.

---

## Required Libraries

To compile and run the graphical version of the game, make sure the following are installed:

* **SFML 2.5.1 or later**
  Required modules:

  * `graphics`
  * `window`
  * `system`

* **C++17-compatible compiler** (e.g., `g++`, `clang++`, MSVC)

### Installing SFML on Ubuntu

```bash
sudo apt update
sudo apt install libsfml-dev
```

---

## GUI Instructions

To run and interact with the game GUI smoothly, follow these steps:

1. **Choose number of players**
   Use the **mouse** to click the desired number (from 2 to 6).

2. **Set keyboard language to English** and press **SPACE**.

3. **Enter player names**
   Type the name directly from the keyboard (no mouse needed).

4. Press **ENTER**.

5. **Enter the next player's name** and press **ENTER** again.

6. Repeat the process for all players.

7. When the screen displaying the player names and their roles appears, press **SPACE** to continue.

8. **Choose an action** from the action buttons shown.

9. If the selected action requires a target (like `arrest`, `bribe`, or `coup`), a prompt will appear:

   ```
   target:
   ```

   → Type the **name of the target player** in English.

10. Press **SPACE** to confirm.

11. The turn automatically switches to the next player.

12. **Repeat the process**: choose an action → press **SPACE**.

13. Continue until a **winner** is declared!

---

## File Overview

### Main Project Files

* `main.cpp`: Handles the main GUI loop, player input, role assignment, and turn logic.
* `Makefile`: Compilation instructions for the main executable and tests.
* `OpenSans-Regular.ttf`: Font used in the GUI for displaying text. Must be in the same directory as the executable.

### Core Game Logic

* `Game.cpp` / `Game.hpp`: Central class managing game state, bank coins, players list, and turn progression.
* `Player.cpp` / `Player.hpp`: Base class for all player types. Contains common behavior like gather, tax, bribe, etc.

### Roles

Each role inherits from `Player` and adds unique behavior:

* `Governor.cpp` / `Governor.hpp`: Can block the "tax" action.
* `Spy.cpp` / `Spy.hpp`: Can block arrest from happening next turn.
* `Baron.cpp` / `Baron.hpp`: Can use the `invest` action to earn coins.
* `General.cpp` / `General.hpp`: Can block the `coup` action.
* `Judge.cpp` / `Judge.hpp`: Can block bribes.
* `Merchant.cpp` / `Merchant.hpp`: Gains 1 coin automatically at the start of a turn if they have 3 or more.

### Tests

* `test_coup.cpp`: Contains unit tests using the `doctest` framework.
* `doctest.h`: Header-only unit testing framework.

---

## Additional Notes

### Font File

The file `OpenSans-Regular.ttf` is required by the SFML GUI to render text.
It must be present in the same directory as the executable.
If the font cannot be loaded, the program will terminate with an error message:

```
Could not load font.
```

---

## Build & Run

### 1. Build and Run GUI

```bash
make Main
```

### 2. Run Unit Tests

```bash
make test
```

### 3. Check for Memory Leaks (requires Valgrind)

```bash
make valgrind
```

### 4. Clean Build Files

```bash
make clean
```
