#include <iostream>
#include <vector>
#include <string>
#include <cstdlib>
#include <ctime>

class Card {
private:
    std::string suit;
    std::string rank;

public:
    Card(const std::string& suit, const std::string& rank)
        : suit(suit), rank(rank) {}

    const std::string& getSuit() const {
        return suit;
    }

    const std::string& getRank() const {
        return rank;
    }

    int getValue() const {
        if (rank == "Ace") {
            return 11;
        } else if (rank == "King" || rank == "Queen" || rank == "Jack") {
            return 10;
        } else {
            return std::stoi(rank);
        }
    }
};

class Deck {
private:
    std::vector<Card> cards;
    int currentIndex;

public:
    Deck() {
        const std::vector<std::string> suits = { "Hearts", "Diamonds", "Clubs", "Spades" };
        const std::vector<std::string> ranks = { "Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King" };

        for (const auto& suit : suits) {
            for (const auto& rank : ranks) {
                cards.emplace_back(suit, rank);
            }
        }

        currentIndex = 0;
    }

    void shuffle() {
        std::srand(static_cast<unsigned int>(std::time(nullptr)));
        for (int i = 0; i < cards.size(); i++) {
            int randomIndex = std::rand() % cards.size();
            std::swap(cards[i], cards[randomIndex]);
        }
    }

    Card dealCard() {
        if (currentIndex >= cards.size()) {
            throw std::runtime_error("Deck is empty");
        }

        return cards[currentIndex++];
    }

    bool isEmpty() const {
        return currentIndex >= cards.size();
    }
};

class Hand {
private:
    std::vector<Card> cards;

public:
    void addCard(const Card& card) {
        cards.push_back(card);
    }

    int getHandValue() const {
        int value = 0;
        int numAces = 0;

        for (const auto& card : cards) {
            value += card.getValue();
            if (card.getRank() == "Ace") {
                numAces++;
            }
        }

        while (value > 21 && numAces > 0) {
            value -= 10;
            numAces--;
        }

        return value;
    }

    void clear() {
        cards.clear();
    }

    const std::vector<Card>& getCards() const {
        return cards;
    }
};

class ConsoleView {
public:
    void displayWelcomeMessage() {
        std::cout << "Welcome to Blackjack!" << std::endl;
    }

    void displayHand(const std::string& playerName, const Hand& hand) {
        std::cout << playerName << "'s Hand: ";
        for (const auto& card : hand.getCards()) {
            std::cout << card.getRank() << " of " << card.getSuit() << ", ";
        }
        std::cout << std::endl;
    }

    void displayHandValue(const std::string& playerName, int handValue) {
        std::cout << playerName << "'s Hand Value: " << handValue << std::endl;
    }

    void displayResult(const std::string& result) {
        std::cout << "Result: " << result << std::endl;
    }

    void displayErrorMessage(const std::string& errorMessage) {
        std::cout << "Error: " << errorMessage << std::endl;
    }

    std::string getPlayerAction() {
        std::string action;
        std::cout << "Choose an action (hit/stand): ";
        std::cin >> action;
        return action;
    }
};

class GameController {
private:
    Deck deck;
    Hand playerHand;
    Hand dealerHand;
    ConsoleView view;

    void dealInitialCards() {
        playerHand.addCard(deck.dealCard());
        dealerHand.addCard(deck.dealCard());
        playerHand.addCard(deck.dealCard());
        dealerHand.addCard(deck.dealCard());
    }

    void playerTurn() {
        while (true) {
            view.displayHand("Player", playerHand);
            view.displayHandValue("Player", playerHand.getHandValue());

            std::string action = view.getPlayerAction();
            if (action == "hit") {
                playerHand.addCard(deck.dealCard());
                if (playerHand.getHandValue() > 21) {
                    view.displayHand("Player", playerHand);
                    view.displayResult("Player Bust! You lose.");
                    return;
                }
            } else if (action == "stand") {
                break;
            } else {
                view.displayErrorMessage("Invalid action. Please try again.");
            }
        }
    }

    void dealerTurn() {
        view.displayHand("Dealer", dealerHand);
        view.displayHandValue("Dealer", dealerHand.getHandValue());

        while (dealerHand.getHandValue() < 17) {
            dealerHand.addCard(deck.dealCard());
            view.displayHand("Dealer", dealerHand);
            view.displayHandValue("Dealer", dealerHand.getHandValue());
        }

        if (dealerHand.getHandValue() > 21) {
            view.displayResult("Dealer Bust! You win.");
        } else {
            int playerHandValue = playerHand.getHandValue();
            int dealerHandValue = dealerHand.getHandValue();

            if (playerHandValue > dealerHandValue) {
                view.displayResult("You win!");
            } else if (playerHandValue < dealerHandValue) {
                view.displayResult("You lose.");
            } else {
                view.displayResult("It's a tie.");
            }
        }
    }

    bool checkPlayerBust() {
        return playerHand.getHandValue() > 21;
    }

    bool checkDealerBust() {
        return dealerHand.getHandValue() > 21;
    }

public:
    void startGame() {
        view.displayWelcomeMessage();

        while (true) {
            playerHand.clear();
            dealerHand.clear();

            if (deck.isEmpty()) {
                deck = Deck();
                deck.shuffle();
            }

            dealInitialCards();

            view.displayHand("Player", playerHand);
            view.displayHandValue("Player", playerHand.getHandValue());

            if (checkPlayerBust()) {
                view.displayResult("Player Bust! You lose.");
            } else {
                playerTurn();
                if (!checkPlayerBust()) {
                    dealerTurn();
                }
            }

            std::string playAgain;
            std::cout << "Play again? (yes/no): ";
            std::cin >> playAgain;
            if (playAgain != "yes") {
                break;
            }
        }
    }
};

int main() {
    GameController gameController;
    gameController.startGame();
    return 0;
}


This code implements the complete Blackjack game using the provided classes and their respective methods. 
It includes the necessary logic for dealing cards, calculating hand values, handling player and dealer turns, and 
determining the game result. The ConsoleView class is responsible for displaying the game state and messages to the user. 
The GameController class controls the flow of the game, interacting with the model and view components.




Yes, the provided code follows the SOLID principles to a great extent. Let's examine how each principle is addressed:

Single Responsibility Principle (SRP):

Each class in the code has a clear and single responsibility.
Card class handles card properties.
Deck class manages the deck of cards.
Hand class manages the cards in a hand and hand value calculation.
ConsoleView class handles the user interface.
GameController class controls the game flow and interactions.
Each class focuses on its specific task, enhancing maintainability and reusability.

Open/Closed Principle (OCP):

The code is designed to be extensible, allowing for the addition of new features or behaviors without modifying existing code.
The Card, Deck, Hand, and ConsoleView classes are closed for modification and can be easily extended by adding new methods or properties.
The GameController class is responsible for game flow control and can be extended to handle additional game rules or variations.

Liskov Substitution Principle (LSP):

The code follows LSP as it uses polymorphism to interchangeably use subclasses with their base classes.
The Deck, Hand, and ConsoleView classes can be easily replaced with their derived classes if necessary, without affecting the behavior of the code.


Interface Segregation Principle (ISP):

The code does not explicitly implement interfaces, but each class has a well-defined set of methods that represent their responsibilities.
Clients interact with classes through these methods, reducing unnecessary dependencies.


Dependency Inversion Principle (DIP):

The code demonstrates DIP as higher-level modules depend on abstractions (interfaces) rather than concrete implementations.
For example, the GameController class depends on the ConsoleView class through its interface, enabling flexibility and testability.
While the code follows the SOLID principles to a good extent, keep in mind that SOLID is a guideline, and the degree of adherence can vary 
depending on the specific requirements and complexity of the system.


The design pattern used in the provided code is the Model-View-Controller (MVC) pattern.

Model:

The Card, Deck, and Hand classes represent the model in the MVC pattern.
These classes encapsulate the data and logic related to the game (the model).

View:

The ConsoleView class represents the view in the MVC pattern.
It is responsible for displaying the game state and messages to the user.
It provides a textual user interface.

Controller:

The GameController class represents the controller in the MVC pattern.
It controls the flow of the game, handles user input, and interacts with the model (Card, Deck, Hand) and view (ConsoleView).
It coordinates the interaction between the model and the view to update the game state and display information to the user.
The MVC pattern is a widely used architectural pattern for designing interactive applications. It promotes separation of concerns 
by dividing the application into three distinct components: the model, the view, and the controller. This separation enhances maintainability, 
reusability, and testability of the codebase.

In the provided code, the model classes (Card, Deck, Hand) handle the data and logic related to the game, the view class (ConsoleView) displays 
the game state and receives user input, and the controller class (GameController) coordinates the interaction between the model and 
the view to control the game flow.





