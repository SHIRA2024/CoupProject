// email: shiraba01@gmail.com
#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <random> 
#include <ctime>  
#include <memory>         
#include <algorithm>      

#include "Game.hpp"
#include "Player.hpp"
#include "Governor.hpp"
#include "Spy.hpp"
#include "Baron.hpp"
#include "General.hpp"
#include "Judge.hpp"
#include "Merchant.hpp"
using namespace coup;

/**
 * @brief Removes all spaces from the input string.
 * 
 * @param str The original string which may contain spaces.
 * @return std::string A new string with all space characters removed.
 */
std::string removeSpaces(const std::string& str) {
    std::string result;
    for (char c : str) {
    if (c != ' ') {
    result += c;
    }
    }
    return result;
    }

/**
 * @brief Converts all characters in the input string to lowercase.
 * 
 * @param str The original string.
 * @return std::string A new string with all characters converted to lowercase.
 */
std::string toLower(const std::string& str) {
    std::string result = str;
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}


/**
 * @brief Displays the assigned roles of players in a graphical window using SFML.
 *
 * This function renders a list of players and their assigned roles on the screen.
 * It also shows an instruction ("Press SPACE to continue") and waits for the user
 * to press the spacebar before continuing. If the window is closed, the function exits.
 * 
 * @param window Reference to the SFML RenderWindow where the text will be displayed.
 * @param font Reference to the SFML Font used to render the text.
 * @param playerRoles A vector of pairs, where each pair contains a player's name and their assigned role.
 */

void showAssignedRoles(sf::RenderWindow& window, sf::Font& font, const std::vector<std::pair<std::string, std::string>>& playerRoles) {
    std::vector<sf::Text> texts;
    int y = 100;

    for (const auto& [name, role] : playerRoles) {
        sf::Text text(name + " got role: " + role, font, 28);
        text.setFillColor(sf::Color::Black);
        text.setPosition(100, y);
        y += 40;
        texts.push_back(text);
    }

    sf::Text continueText("Press SPACE to continue", font, 24);
    continueText.setFillColor(sf::Color::Blue);
    continueText.setPosition(100, y + 40);
    texts.push_back(continueText);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
                return;
        }

        window.clear(sf::Color::White);
        for (auto& text : texts) window.draw(text);
        window.display();
    }
}


/**
 * @brief Displays a graphical interface for choosing the number of players (2–6) using SFML.
 *
 * This function shows a title and buttons for selecting between 2 and 6 players.
 * The user selects a number by clicking one of the options. The function returns
 * the chosen number once a valid selection is made, or exits if the window is closed.
 *
 * @param window Reference to the SFML RenderWindow used for displaying the interface.
 * @param font Reference to the SFML Font used for rendering the text.
 * @return int The selected number of players (between 2 and 6).
 */
int chooseNumberOfPlayers(sf::RenderWindow& window, sf::Font& font) {
    sf::Text title("Choose number of players (2-6):", font, 30);
    title.setPosition(150, 100);
    title.setFillColor(sf::Color::Black);

    std::vector<sf::Text> options;
    for (int i = 2; i <= 6; ++i) {
        sf::Text option(std::to_string(i), font, 24);
        option.setPosition(200 + (i - 2) * 80, 200);
        option.setFillColor(sf::Color::Blue);
        options.push_back(option);
    }

    int selectedPlayers = 0;
    while (window.isOpen() && selectedPlayers == 0) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) window.close();

            if (event.type == sf::Event::MouseButtonPressed) {
                for (size_t i = 0; i < options.size(); ++i) {
                    sf::FloatRect bounds = options[i].getGlobalBounds();
                    if (bounds.contains(event.mouseButton.x, event.mouseButton.y)) {
                        selectedPlayers = i + 2;
                    }
                }
            }
        }

        window.clear(sf::Color::White);
        window.draw(title);
        for (auto& opt : options) window.draw(opt);
        window.display();
    }

    return selectedPlayers;
}


/**
 * @brief Displays a message indicating the number of selected players and waits for user input.
 *
 * This function shows a message in the window stating how many players were selected,
 * and prompts the user to press the SPACE key to continue. It blocks until either
 * the SPACE key is pressed or the window is closed.
 *
 * @param window Reference to the SFML RenderWindow used for displaying the message.
 * @param font Reference to the SFML Font used for rendering the text.
 * @param numPlayers The number of players that were selected.
 */
void showSelectedPlayersMessage(sf::RenderWindow& window, sf::Font& font, int numPlayers) {
    sf::Text message;
    message.setFont(font);
    message.setString(std::to_string(numPlayers) + " players selected!\nPress SPACE to start.");
    message.setCharacterSize(30);
    message.setFillColor(sf::Color::Black);
    message.setPosition(120, 200);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            

            if (event.type == sf::Event::Closed) {
                window.close();
                return;
            }
            if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space) {
                return; 
            }
        }

        window.clear(sf::Color::White);
        window.draw(message);
        window.display();
    }
}

/**
 * @brief Prompts the user to enter names for each player using a graphical input interface.
 *
 * This function iterates over the number of players and opens a text input prompt
 * for each one inside the given SFML window. The user can type each name and press
 * Enter to confirm. Names are collected and returned as a vector of strings.
 *
 * The input is shown inside a rectangular input box, and the interface handles
 * backspace for corrections and skips empty inputs.
 *
 * @param window Reference to the SFML RenderWindow used for input display.
 * @param font Reference to the SFML Font used to render the texts.
 * @param numPlayers Number of player names to collect.
 * @return std::vector<std::string> A vector containing the entered names.
 */

std::vector<std::string> getPlayerNames(sf::RenderWindow& window, sf::Font& font, int numPlayers) {
    std::vector<std::string> names;

    for (int i = 0; i < numPlayers; ++i) {
        std::string currentName;

        sf::Text title("Enter name for Player " + std::to_string(i + 1) + ":", font, 30);
        title.setFillColor(sf::Color::Black);
        title.setPosition(100, 100);

        sf::Text input("", font, 28);
        input.setFillColor(sf::Color::Blue);
        input.setPosition(110, 185);  

        
        sf::RectangleShape inputBox;
        inputBox.setSize(sf::Vector2f(400, 40));
        inputBox.setPosition(100, 180);
        inputBox.setFillColor(sf::Color(230, 230, 230));  
        inputBox.setOutlineColor(sf::Color::Black);
        inputBox.setOutlineThickness(2);

        while (window.isOpen()) {
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                    return names;
                }

                if (event.type == sf::Event::TextEntered) {
                    if (event.text.unicode == '\b' && !currentName.empty()) {
                        currentName.pop_back(); 
                    } else if (event.text.unicode < 128 && event.text.unicode != '\r') {
                        currentName += static_cast<char>(event.text.unicode); 
                    }
                }

                if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {
                    if (!currentName.empty()) {
                        names.push_back(currentName);
                        goto next_player;
                    }
                }
            }

            input.setString(currentName);

            window.clear(sf::Color::White);
            window.draw(title);
            window.draw(inputBox);   
            window.draw(input);      
            window.display();
        }
    next_player:
        continue;
    }

    return names;
}





/**
 * @brief Entry point for the Coup GUI game.
 *
 * This function sets up the graphical user interface and manages the entire game loop.
 * It performs the following main tasks:
 * 
 * 1. Initializes the SFML window and font resources.
 * 2. Prompts the user to select the number of players and enter their names.
 * 3. Randomly assigns roles to players and displays them.
 * 4. Initializes and displays action buttons according to the current player's role.
 * 5. Handles input events such as:
 *     - Mouse clicks on action buttons
 *     - Text input for target-based actions
 *     - Spacebar to proceed to the next turn
 *     - Enter key to confirm a target for targeted actions
 * 6. Executes the selected game actions (e.g., gather, tax, bribe, arrest, block, etc.).
 * 7. Displays output messages in a designated box and updates the game state accordingly.
 * 8. Handles game ending when a winner is declared.
 *
 * The function uses several helper classes and functions such as Game, Player, and their derived classes
 * (Governor, Spy, Baron, General, Judge, Merchant), along with utility functions like
 * chooseNumberOfPlayers, getPlayerNames, and showAssignedRoles.
 *
 * @return int 0 on successful completion, or 1 if an error occurs (e.g., font not loaded).
 */
int main() {
    sf::RenderWindow window(sf::VideoMode(800, 700), "Coup GUI - Setup");
    sf::Font font;
    if (!font.loadFromFile("OpenSans-Regular.ttf")) {
        std::cerr << "Could not load font.\n";
        return 1;
    }

    
    bool isAwaitingTargetInput = false;
    std::string currentActionNeedingTarget = "";
    std::string targetName = "";
    bool waitingForSpace = false;

    sf::Text inputPromptText("Enter target player's name:", font, 20);
    inputPromptText.setFillColor(sf::Color::White);
    inputPromptText.setPosition(50, 550);

    sf::Text targetInputText("Target: ", font, 20);
    targetInputText.setFillColor(sf::Color::Red);
    targetInputText.setPosition(300, 550);

    
    Game game;
    int currentTurn = 0;
    int numPlayers = chooseNumberOfPlayers(window, font);
    showSelectedPlayersMessage(window, font, numPlayers);
    std::vector<std::string> playerNames = getPlayerNames(window, font, numPlayers);

    std::vector<std::string> availableRoles = { "Governor", "Spy", "Baron", "General", "Judge", "Merchant" };
    std::shuffle(availableRoles.begin(), availableRoles.end(), std::default_random_engine(static_cast<unsigned>(std::time(nullptr))));

    std::vector<std::shared_ptr<Player>> players;
    std::vector<std::pair<std::string, std::string>> playerRoles;
    std::vector<sf::RectangleShape> actionButtonBoxes;
    std::vector<sf::Text> actionButtons;

    for (int i = 0; i < numPlayers; ++i) {
        std::string name = playerNames[i];
        std::string role = availableRoles[i];
        playerRoles.emplace_back(name, role);

        if (role == "Governor") players.push_back(std::make_shared<Governor>(game, name));
        else if (role == "Spy") players.push_back(std::make_shared<Spy>(game, name));
        else if (role == "Baron") players.push_back(std::make_shared<Baron>(game, name));
        else if (role == "General") players.push_back(std::make_shared<General>(game, name));
        else if (role == "Judge") players.push_back(std::make_shared<Judge>(game, name));
        else if (role == "Merchant") players.push_back(std::make_shared<Merchant>(game, name));
    }
    showAssignedRoles(window, font, playerRoles);

    sf::Text currentPlayerText("Current Player: " + playerNames[0], font, 30);
    currentPlayerText.setFillColor(sf::Color::Black);
    currentPlayerText.setPosition(100, 100);

    sf::Text roleText("Role: " + playerRoles[0].second, font, 30);
    roleText.setFillColor(sf::Color::Black);
    roleText.setPosition(100, 150);

    sf::Text coinText("Coins: 0", font, 30);
    sf::Text bankText("Bank: " + std::to_string(game.getBankCoins()), font, 30);
    bankText.setFillColor(sf::Color::Black);
    bankText.setPosition(500, 150);
    coinText.setFillColor(sf::Color::Black);
    coinText.setPosition(500, 100);

    sf::RectangleShape outputBox;
    outputBox.setSize(sf::Vector2f(600, 40));
    outputBox.setPosition(100, 600);
    outputBox.setFillColor(sf::Color(220, 220, 220));
    outputBox.setOutlineColor(sf::Color::Black);
    outputBox.setOutlineThickness(2);

    sf::Text outputText("", font, 20);
    outputText.setFillColor(sf::Color::Black);
    outputText.setPosition(100, 600);

    std::vector<std::string> baseActions = { "Gather", "Tax", "Bribe", "Arrest", "Sanction", "Coup" };
    std::string currentRole = playerRoles[0].second;
    std::vector<std::string> roleSpecificActions;
    if (currentRole == "Governor") roleSpecificActions.push_back("Block Tax");
    if (currentRole == "Spy") roleSpecificActions.push_back("Block Arrest");
    if (currentRole == "General") roleSpecificActions.push_back("Block Coup");
    if (currentRole == "Baron") roleSpecificActions.push_back("Invest");
    if (currentRole == "Judge") roleSpecificActions.push_back("Block Bribe");

    std::vector<std::string> allActions = baseActions;
    allActions.insert(allActions.end(), roleSpecificActions.begin(), roleSpecificActions.end());

    int buttonY = 200;
    for (const std::string& action : allActions) {
        sf::Text text(action, font, 24);
        text.setFillColor(sf::Color::Blue);
        text.setPosition(120, buttonY + 10);

        sf::RectangleShape box;
        box.setSize(sf::Vector2f(200, 40));
        box.setPosition(100, buttonY);
        box.setFillColor(sf::Color(220, 220, 220));
        box.setOutlineColor(sf::Color::Black);
        box.setOutlineThickness(2);

        actionButtons.push_back(text);
        actionButtonBoxes.push_back(box);
        buttonY += 50;
    }

    


while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
        if (event.type == sf::Event::Closed) {
            window.close();
            return 0;
        }

        
        if (isAwaitingTargetInput && event.type == sf::Event::TextEntered) {
            if (event.text.unicode == '\b' && !targetName.empty()) {
                targetName.pop_back();
            } else if (event.text.unicode < 128 && event.text.unicode != '\r') {
                targetName += static_cast<char>(event.text.unicode);
            }
            targetInputText.setString("Target: " + targetName);
        }

        if (isAwaitingTargetInput && event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Enter) {

            
            
            isAwaitingTargetInput = false;

            
            Player* target = nullptr;
            for (auto& p : players) {
            std::string cleanedName = removeSpaces(p->getName());
            std::string cleanedTarget = removeSpaces(targetName);

           

            if (cleanedName == cleanedTarget) {
                
                target = p.get();
                break;
            }
        }

        if (target == nullptr) {
            outputText.setString("No player found with name: " + targetName);
        } else {
            auto currentPlayer = players[currentTurn];
            try {

               
                if (currentActionNeedingTarget == "arrest") {
                    
                    currentPlayer->arrest(*target);
                    outputText.setString(currentPlayer->getName() + " arrested " + target->getName());
                    bankText.setString("Bank: " + std::to_string(game.getBankCoins()));
                    waitingForSpace = true;
                } else if (currentActionNeedingTarget == "sanction") {
                    currentPlayer->sanction(*target);
                    outputText.setString(currentPlayer->getName() + " sanctioned " + target->getName());
                    bankText.setString("Bank: " + std::to_string(game.getBankCoins()));
                    waitingForSpace = true;
                } else if (currentActionNeedingTarget == "coup") {
                    currentPlayer->coup(*target);
                    outputText.setString(currentPlayer->getName() + " performed a coup on " + target->getName());
                    bankText.setString("Bank: " + std::to_string(game.getBankCoins()));
                    waitingForSpace = true;
                try {
                    std::string winnerName = game.winner();
                    outputText.setString(winnerName + " wins!");
                    window.clear(sf::Color::White);
                    window.draw(outputBox);
                    window.draw(outputText);
                    window.display();
                    sf::sleep(sf::seconds(5));
                    window.close();
                    return 0;
                    } catch (const std::exception& e) {
                    
                    }

                } else if (currentActionNeedingTarget == "blockTax") {
                    auto gov = dynamic_cast<Governor*>(currentPlayer.get());
                    if (!gov) throw std::runtime_error("Only a Governor can block tax.");
                    gov->blockTax(*target);
                    outputText.setString(currentPlayer->getName() + " blocked tax from " + target->getName());
                    bankText.setString("Bank: " + std::to_string(game.getBankCoins()));
                    waitingForSpace = true;  
                    
                } else if (currentActionNeedingTarget == "blockBribe") {
                    auto judge = dynamic_cast<Judge*>(currentPlayer.get());
                    if (!judge) throw std::runtime_error("Only a Judge can block bribe.");
                    judge->blockBribe(*target);
                    outputText.setString(currentPlayer->getName() + " blocked bribe from " + target->getName());
                    bankText.setString("Bank: " + std::to_string(game.getBankCoins()));
                    waitingForSpace = true;
                } else if (currentActionNeedingTarget == "blockArrestNextTurn") {
                    
                    auto spy = dynamic_cast<Spy*>(currentPlayer.get());
                    if (!spy) throw std::runtime_error("Only a Spy can block arrest.");
                    spy->blockArrestNextTurn(*target);
                    outputText.setString(currentPlayer->getName() + " blocked arrest from " + target->getName());
                    bankText.setString("Bank: " + std::to_string(game.getBankCoins()));
                    waitingForSpace = true;
                } else if (currentActionNeedingTarget == "blockCoup") {
                    
                    auto general = dynamic_cast<General*>(currentPlayer.get());
                    if (!general) throw std::runtime_error("Only a General can block coup.");
                    general->blockCoup(*target);
                    outputText.setString(currentPlayer->getName() + " blocked coup from " + target->getName());
                    bankText.setString("Bank: " + std::to_string(game.getBankCoins()));
                    waitingForSpace = true;
                } else if (currentActionNeedingTarget == "invest") {
                    auto baron = dynamic_cast<Baron*>(currentPlayer.get());
                    if (!baron) throw std::runtime_error("Only a Baron can invest.");
                    baron->invest();
                    outputText.setString(currentPlayer->getName() + " invested " + target->getName());
                    bankText.setString("Bank: " + std::to_string(game.getBankCoins()));
                    waitingForSpace = true;
                }

                coinText.setString("Coins: " + std::to_string(currentPlayer->getCoins()));
                waitingForSpace = true;
            } catch (const std::exception& e) {
                outputText.setString(e.what());
            }
        }

        }


if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space && waitingForSpace) {
    game.advanceTurn();  
    currentTurn = (currentTurn + 1) % players.size();

    auto currentPlayer = players[currentTurn];
    std::string currentRole = currentPlayer->getRole();

    currentPlayerText.setString("Current Player: " + currentPlayer->getName());
    roleText.setString("Role: " + currentRole);
    coinText.setString("Coins: " + std::to_string(currentPlayer->getCoins()));
    bankText.setString("Bank: " + std::to_string(game.getBankCoins()));

   
    actionButtons.clear();
    actionButtonBoxes.clear();

    std::vector<std::string> baseActions = { "Gather", "Tax", "Bribe", "Arrest", "Sanction", "Coup" };
    std::vector<std::string> roleSpecificActions;
    if (currentRole == "Governor") roleSpecificActions.push_back("Block Tax");
    if (currentRole == "Spy") roleSpecificActions.push_back("Block Arrest");
    if (currentRole == "General") roleSpecificActions.push_back("Block Coup");
    if (currentRole == "Baron") roleSpecificActions.push_back("Invest");
    if (currentRole == "Judge") roleSpecificActions.push_back("Block Bribe");

    std::vector<std::string> allActions = baseActions;
    allActions.insert(allActions.end(), roleSpecificActions.begin(), roleSpecificActions.end());

    int buttonY = 200;
    for (const std::string& action : allActions) {
        sf::Text text(action, font, 24);
        text.setFillColor(sf::Color::Blue);
        text.setPosition(120, buttonY + 10);

        sf::RectangleShape box;
        box.setSize(sf::Vector2f(200, 40));
        box.setPosition(100, buttonY);
        box.setFillColor(sf::Color(220, 220, 220));
        box.setOutlineColor(sf::Color::Black);
        box.setOutlineThickness(2);

        actionButtons.push_back(text);
        actionButtonBoxes.push_back(box);
        buttonY += 50;
    }

    waitingForSpace = false;
}



        if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left) {
            sf::Vector2f mousePos(event.mouseButton.x, event.mouseButton.y);
            for (size_t i = 0; i < actionButtonBoxes.size(); ++i) {
                if (actionButtonBoxes[i].getGlobalBounds().contains(mousePos)) {
                    std::string selectedAction = toLower(actionButtons[i].getString());
                    std::transform(selectedAction.begin(), selectedAction.end(), selectedAction.begin(), ::tolower);
                    auto currentPlayer = players[currentTurn];

                    try {
                        if (selectedAction == "gather") {
                            currentPlayer->gather();
                            outputText.setString(currentPlayer->getName() + " gathered 1 coin.");
                            bankText.setString("Bank: " + std::to_string(game.getBankCoins()));
                            waitingForSpace = true;
                        } else if (selectedAction == "tax") {
                            currentPlayer->tax();
                            outputText.setString(currentPlayer->getName() + " received 2 coins from tax.");
                            bankText.setString("Bank: " + std::to_string(game.getBankCoins()));
                            waitingForSpace = true;
                        } else if (selectedAction == "bribe") {
                            currentPlayer->bribe();
                            outputText.setString(currentPlayer->getName() + " bribed and lost 4 coins.");
                            bankText.setString("Bank: " + std::to_string(game.getBankCoins()));
                            waitingForSpace = true;
                        } else if (
                            selectedAction == "arrest" ||
                            selectedAction == "sanction" ||
                            selectedAction == "coup" ||
                            selectedAction == "blockTax" ||
                            selectedAction == "blockBribe") {

                            isAwaitingTargetInput = true;
                            currentActionNeedingTarget = selectedAction;
                            targetName.clear();
                            targetInputText.setString("Target: ");
                            continue;  
                        }

                        coinText.setString("Coins: " + std::to_string(currentPlayer->getCoins()));
                        waitingForSpace = true;
                    } catch (const std::exception& e) {
                        outputText.setString(e.what());
                    }
                }
            }
        }
    }

        window.clear(sf::Color::White);
        window.draw(currentPlayerText);
        window.draw(roleText);
        window.draw(coinText);
        window.draw(bankText);
        
        if (isAwaitingTargetInput) window.draw(targetInputText);
        for (size_t i = 0; i < actionButtons.size(); ++i) {
            window.draw(actionButtonBoxes[i]);
            window.draw(actionButtons[i]);
        }
        window.draw(outputBox);
        window.draw(outputText);
        window.display();
    }


    return 0;
    }


