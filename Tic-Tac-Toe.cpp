/*##########################################################################
System Requirements
--------------------
1. Handle Player Moves: Allow two players to alternately place their marks (X or O) on a 3x3 grid.
2. Check for Win or Draw: Determine the outcome of the game – a win for one player, a draw, or continuation.
3. Reset the Game: Enable starting a new game after one round finishes.
---------------------------------------------------------------------------------
Core Use Cases
--------------
1. Making a Move: Players take turns to place their marks on the grid.
2. Checking Game Status: After each move, check if the game is won, drawn, or still ongoing.
3. Resetting the Game: Clear the board for a new round.

##########################################################################*/

#include <bits/stdc++.h>
using namespace std;

enum class Mark
{
    Empty,
    X,
    O
};

class Player
{
private:
    int playerId;
    std::string name;
    Mark mark;

public:
    Player(int id, const std::string &playerName, Mark playerMark)
        : playerId(id), name(playerName), mark(playerMark) {}

    int getPlayerId() const { return playerId; }
    std::string getName() const { return name; }
    Mark getMark() const { return mark; }
};

class Board
{
private:
    std::vector<std::vector<Mark>> grid;

public:
    Board() : grid(3, std::vector<Mark>(3, Mark::Empty)) {}

    void display() const
    {
        for (const auto &row : grid)
        {
            for (const auto &cell : row)
            {
                char mark = '-';
                if (cell == Mark::X)
                    mark = 'X';
                else if (cell == Mark::O)
                    mark = 'O';
                std::cout << mark << " ";
            }
            std::cout << std::endl;
        }
    }

    bool isValidMove(int row, int col) const
    {
        return row >= 0 && row < 3 && col >= 0 && col < 3 && grid[row][col] == Mark::Empty;
    }

    void placeMark(int row, int col, Mark mark)
    {
        grid[row][col] = mark;
    }

    bool isFull() const
    {
        for (const auto &row : grid)
        {
            for (const auto &cell : row)
            {
                if (cell == Mark::Empty)
                    return false;
            }
        }
        return true;
    }

    bool checkWinner(const Player &player) const
    {
        for (int i = 0; i < 3; ++i)
        {
            if (grid[i][0] == player.getMark() && grid[i][0] == grid[i][1] && grid[i][1] == grid[i][2])
                return true;
            if (grid[0][i] == player.getMark() && grid[0][i] == grid[1][i] && grid[1][i] == grid[2][i])
                return true;
        }
        if (grid[0][0] == player.getMark() && grid[0][0] == grid[1][1] && grid[1][1] == grid[2][2])
            return true;
        if (grid[0][2] == player.getMark() && grid[0][2] == grid[1][1] && grid[1][1] == grid[2][0])
            return true;
        return false;
    }

    void reset()
    {
        for (auto &row : grid)
        {
            for (auto &cell : row)
            {
                cell = Mark::Empty;
            }
        }
    }
};

class TicTacToe
{
private:
    Board board;
    Player player1;
    Player player2;
    Player currentPlayer;

public:
    TicTacToe(const Player &p1, const Player &p2) : player1(p1), player2(p2), currentPlayer(p1) {}

    void start()
    {
        std::cout << "Tic-Tac-Toe Game Started!" << std::endl;
        while (true)
        {
            board.display();
            std::cout << "Player " << currentPlayer.getName() << "'s turn. Enter row and column (0-2): ";
            int row, col;
            std::cin >> row >> col;
            if (!board.isValidMove(row, col))
            {
                std::cout << "Invalid move! Try again." << std::endl;
                continue;
            }
            board.placeMark(row, col, currentPlayer.getMark());
            if (board.checkWinner(currentPlayer))
            {
                std::cout << "Player " << currentPlayer.getName() << " wins!" << std::endl;
                break;
            }
            if (board.isFull())
            {
                std::cout << "It's a draw!" << std::endl;
                break;
            }
            currentPlayer = (currentPlayer.getPlayerId() == player1.getPlayerId()) ? player2 : player1;
        }
        board.reset();
    }
};

int main()
{
    Player playerX(1, "PlayerX", Mark::X);
    Player playerO(2, "PlayerO", Mark::O);
    TicTacToe game(playerX, playerO);
    game.start();
    return 0;
}

/*
 The design pattern used in the provided code is the Model-View-Controller (MVC) pattern.
 Here's how it maps to the code:

1. Model: The Board class represents the model. It encapsulates the state of the Tic-Tac-Toe board
   and provides methods to manipulate and query the board's state.
2. View: The output of the game, including the display of the board and messages to the players,
   is handled in the display() method of the Board class and through cout statements in the TicTacToe class.
3. Controller: The TicTacToe class acts as the controller. It coordinates the interaction between
   the players, the board, and the game flow. It prompts the players for moves, updates
   the board accordingly, and determines the game's outcome.
*/