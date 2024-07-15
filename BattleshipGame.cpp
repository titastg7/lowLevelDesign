/*##########################################################################
System Requirements
--------------------
1. Design a high level interface for the guessing game BattleShip
---------------------------------------------------------------------------------
Core Use Cases
--------------
1. Start a New Game :The game initializes two players, sets up the board for each player, and gets ready for ship placement.
2. Place Ships : Players place their ships on their respective boards by specifying coordinates.
3. Make Moves : Players take turns specifying coordinates to attack on the opponent’s board.
4. Check for Hits and Misses : The system checks if the specified attack coordinates hit or miss a ship and updates the board.
5. Check for Sunk Ships :  The system checks if a ship is completely sunk after a hit.
6. End Game : The game checks if all ships of a player are sunk and declares the opponent as the winner.

##########################################################################*/

#include <iostream>

using namespace std;

const int BOARD_SIZE = 10;

// Enumerations
enum class ShipType
{
    Carrier,
    Battleship,
    Cruiser,
    Submarine,
    Destroyer
};

enum class AttackResult
{
    Hit,
    Miss,
    Sunk,
    Invalid
};

// Forward declaration
class Board;

// Ship class
class Ship
{
    ShipType type;
    int length;
    int hits;
    bool isSunk;

public:
    Ship(ShipType type) : type(type), length(getShipLength(type)), hits(0), isSunk(false) {}

    int getLength() const { return length; }
    bool getSunk() const { return isSunk; }
    void hit()
    {
        hits++;
        if (hits >= length)
        {
            isSunk = true;
        }
    }

private:
    int getShipLength(ShipType type) const
    {
        switch (type)
        {
        case ShipType::Carrier:
            return 5;
        case ShipType::Battleship:
            return 4;
        case ShipType::Cruiser:
            return 3;
        case ShipType::Submarine:
            return 3;
        case ShipType::Destroyer:
            return 2;
        default:
            throw invalid_argument("Unknown ship type");
        }
    }
};

// Board cell class
class BoardCell
{
    bool occupied;
    bool attacked;
    Ship *ship;

public:
    BoardCell() : occupied(false), attacked(false), ship(nullptr) {}

    bool isOccupied() const { return occupied; }
    bool isAttacked() const { return attacked; }
    Ship *getShip() const { return ship; }

    void placeShip(Ship *s)
    {
        if (occupied)
        {
            throw runtime_error("Cell already occupied");
        }
        occupied = true;
        ship = s;
    }

    AttackResult receiveAttack()
    {
        if (attacked)
        {
            return AttackResult::Invalid;
        }
        attacked = true;
        if (occupied)
        {
            ship->hit();
            if (ship->getSunk())
            {
                return AttackResult::Sunk;
            }
            else
            {
                return AttackResult::Hit;
            }
        }
        return AttackResult::Miss;
    }
};

// Board class
class Board
{
    vector<vector<BoardCell>> cells;

public:
    Board()
    {
        cells.resize(BOARD_SIZE, vector<BoardCell>(BOARD_SIZE));
    }

    bool isValidCoordinate(int x, int y) const
    {
        return !(x < 0 || x >= BOARD_SIZE || y < 0 || y >= BOARD_SIZE);
    }

    bool placeShip(Ship *ship, int x, int y, bool isVertical)
    {
        int length = ship->getLength();
        if (isVertical)
        {
            if (x + length > BOARD_SIZE)
            {
                return false;
            }
            for (int i = x; i < x + length; i++)
            {
                if (cells[i][y].isOccupied())
                {
                    return false;
                }
            }
            for (int i = x; i < x + length; i++)
            {
                cells[i][y].placeShip(ship);
            }
        }
        else
        {
            if (y + length > BOARD_SIZE)
            {
                return false;
            }
            for (int j = y; j < y + length; j++)
            {
                if (cells[x][j].isOccupied())
                {
                    return false;
                }
            }
            for (int j = y; j < y + length; j++)
            {
                cells[x][j].placeShip(ship);
            }
        }
        return true;
    }

    AttackResult receiveAttack(int x, int y)
    {
        if (!isValidCoordinate(x, y) || cells[x][y].isAttacked())
        {
            return AttackResult::Invalid;
        }
        return cells[x][y].receiveAttack();
    }

    bool allShipsSunk() const
    {
        for (const auto &row : cells)
        {
            for (const auto &cell : row)
            {
                if (cell.isOccupied() && !cell.getShip()->getSunk())
                {
                    return false;
                }
            }
        }
        return true;
    }

    void display() const
    {
        cout << "   A B C D E F G H I J" << endl;
        for (int i = 0; i < BOARD_SIZE; i++)
        {
            cout << i << "  ";
            for (int j = 0; j < BOARD_SIZE; j++)
            {
                if (cells[i][j].isAttacked())
                {
                    if (cells[i][j].isOccupied())
                    {
                        cout << "X ";
                    }
                    else
                    {
                        cout << "O ";
                    }
                }
                else
                {
                    cout << "- ";
                }
            }
            cout << endl;
        }
    }
};

// Player class
class Player
{
    string name;
    Board board;

public:
    Player(const string &name) : name(name) {}

    bool placeShip(Ship *ship, int x, int y, bool isVertical)
    {
        return board.placeShip(ship, x, y, isVertical);
    }

    Board &getBoard()
    {
        return board;
    }

    const string &getName() const { return name; }
};

// Game class
class Game
{
    Player player1;
    Player player2;
    Player *currentPlayer;
    Player *opponent;

public:
    Game(const string &name1, const string &name2) : player1(name1), player2(name2)
    {
        currentPlayer = &player1;
        opponent = &player2;
    }

    void setup()
    {
        // Place ships for player 1
        cout << player1.getName() << ", place your ships:" << endl;
        placeShips(player1);

        // Place ships for player 2
        cout << player2.getName() << ", place your ships:" << endl;
        placeShips(player2);
    }

    void start()
    {
        setup();

        while (true)
        {
            cout << currentPlayer->getName() << "'s board:" << endl;
            currentPlayer->getBoard().display();
            cout << opponent->getName() << "'s board:" << endl;
            opponent->getBoard().display();

            takeTurn();

            if (opponent->getBoard().allShipsSunk())
            {
                cout << currentPlayer->getName() << " wins!" << endl;
                break;
            }

            swapPlayers();
        }
    }

private:
    void placeShips(Player &player)
    {
        for (int i = 0; i < 5; i++)
        {
            ShipType type = static_cast<ShipType>(i);
            int x, y;
            char orientation;
            cout << "Enter coordinates for " << (i + 1) << ". ship (Type " << i << "): ";
            cin >> x >> y;
            cout << "Enter orientation (V for Vertical, H for Horizontal): ";
            cin >> orientation;
            bool isVertical = (orientation == 'V' || orientation == 'v');

            if (!player.placeShip(new Ship(type), x, y, isVertical))
            {
                cout << "Invalid placement. Try again." << endl;
                i--;
            }
            else
            {
                player.getBoard().display();
            }
        }
    }

    void takeTurn()
    {
        cout << currentPlayer->getName() << ", enter attack coordinates (e.g., A 0): ";
        int x, y;
        char col;
        cin >> col >> y;
        x = charToCoordinate(col);

        AttackResult result = opponent->getBoard().receiveAttack(x, y);

        switch (result)
        {
        case AttackResult::Hit:
            cout << "Hit!" << endl;
            break;
        case AttackResult::Miss:
            cout << "Miss!" << endl;
            break;
        case AttackResult::Sunk:
            cout << "Sunk!" << endl;
            break;
        case AttackResult::Invalid:
            cout << "Invalid attack!" << endl;
            break;
        }
    }

    int charToCoordinate(char c)
    {
        return c - 'A';
    }

    void swapPlayers()
    {
        // Swap current player and opponent
        Player *temp = currentPlayer;
        currentPlayer = opponent;
        opponent = temp;
    }
};

// Main function
int main()
{
    srand(static_cast<unsigned int>(time(nullptr)));

    // Create and play the game
    Game game("Player 1", "Player 2");
    game.start();

    return 0;
}