/*##########################################################################
System Requirements
--------------------
1. Authentication: Verifying user identity using a card number and PIN.
2. Account Management: Managing different account types.
3. Balance Inquiry: Checking the account balance.
4. Cash Withdrawal and Deposit: Handling money transactions.
5. Transaction History: Providing a record of past transactions.
---------------------------------------------------------------------------------
Core Use Cases
--------------
1. User Authentication
2. Performing Transactions: Withdrawals, deposits, balance inquiries.
3. Printing Transaction Receipts (optional for this implementation)

##########################################################################*/
#include <bits/stdc++.h>
using namespace std;

// TransactionType enum
enum class TransactionType
{
    WITHDRAWAL,
    DEPOSIT
};

// ATM class
class ATM
{
private:
    static ATM *instance = nullptr;
    CardReader *cardReader;
    CashDispenser *cashDispenser;
    Display *display;
    Keyboard *keyboard;
    string currentCardNumber;

    ATM()
    {
        cardReader = new CardReader();
        cashDispenser = new CashDispenser();
        display = new Display();
        keyboard = new Keyboard();
    }

    bool authenticateWithBank(string cardNumber, string pin)
    {
        // Placeholder implementation
        cout << "Authenticating card with bank's backend..." << endl;
        return true; // Placeholder success
    }

    float getBalanceFromBank(string cardNumber)
    {
        // Placeholder implementation
        cout << "Fetching balance from bank's backend..." << endl;
        return 1000.00; // Placeholder balance
    }

    bool withdrawFromBank(string cardNumber, float amount)
    {
        // Placeholder implementation
        cout << "Withdrawing $" << amount << " from bank's backend..." << endl;
        return true; // Placeholder success
    }

    bool depositToBank(string cardNumber, float amount)
    {
        // Placeholder implementation
        cout << "Depositing $" << amount << " to bank's backend..." << endl;
        return true; // Placeholder success
    }

    vector<string> getTransactionHistoryFromBank(string cardNumber)
    {
        // Placeholder implementation
        cout << "Fetching transaction history from bank's backend..." << endl;
        return {}; // Placeholder transaction history
    }

    void logTransaction(TransactionType type, float amount)
    {
        // Placeholder implementation
        string typeName = (type == TransactionType::WITHDRAWAL) ? "Withdrawal" : "Deposit";
        cout << "Logging " << typeName << " transaction: $" << amount << endl;
    }

public:
    static ATM *getInstance()
    {
        if (!instance)
            instance = new ATM();
        return instance;
    }

    ~ATM()
    {
        delete cardReader;
        delete cashDispenser;
        delete display;
        delete keyboard;
    }
    void authenticate()
    {
        display->showMessage("Please insert your card.");
        string cardNumber = cardReader->readCard();
        display->showMessage("Please enter your PIN:");
        string pin = keyboard->getInput();

        bool authenticated = authenticateWithBank(cardNumber, pin);
        if (authenticated)
        {
            currentCardNumber = cardNumber;
            display->showMessage("Authentication successful.");
        }
        else
        {
            display->showMessage("Authentication failed. Please try again.");
        }
    }

    void checkBalance()
    {
        if (currentCardNumber.empty())
        {
            display->showMessage("Please authenticate yourself first.");
            return;
        }

        float balance = getBalanceFromBank(currentCardNumber);
        display->showMessage("Your account balance is: $" + to_string(balance));
    }
    void withdraw(float amount)
    {
        if (currentCardNumber.empty())
        {
            display->showMessage("Please authenticate yourself first.");
            return;
        }

        display->showMessage("Please enter the amount to withdraw:");
        float amount = stof(keyboard->getInput());
        bool success = withdrawFromBank(currentCardNumber, amount);

        if (success)
        {
            cashDispenser->dispenseCash(amount);
            logTransaction(TransactionType::WITHDRAWAL, amount);
        }
        else
        {
            display->showMessage("Failed to withdraw. Please try again later.");
        }
    }
    void deposit(float amount)
    {
        if (currentCardNumber.empty())
        {
            display->showMessage("Please authenticate yourself first.");
            return;
        }

        display->showMessage("Please enter the amount to deposit:");
        float amount = stof(keyboard->getInput());
        bool success = depositToBank(currentCardNumber, amount);
        if (success)
        {
            logTransaction(TransactionType::DEPOSIT, amount);
        }
        else
        {
            display->showMessage("Failed to deposit. Please try again later.");
        }
    }

    void viewTransactionHistory()
    {
        if (currentCardNumber.empty())
        {
            display->showMessage("Please authenticate yourself first.");
            return;
        }

        vector<string> history = getTransactionHistoryFromBank(currentCardNumber);
        display->showMessage("Transaction History:");
        for (const auto &transaction : history)
        {
            display->showMessage(transaction);
        }
    }
};

// CardReader class
class CardReader
{
public:
    string readCard()
    {
        // Placeholder implementation
        cout << "Card inserted. Please enter card number: ";
        string cardNumber;
        cin >> cardNumber;
        return cardNumber;
    }
};

// CashDispenser class
class CashDispenser
{
public:
    void dispenseCash(float amount)
    {
        // Simulate cash dispensing
        cout << "Dispensing $" << amount << endl;
    }
};

// Display class
class Display
{
public:
    void showMessage(string message)
    {
        cout << message << endl;
    }
};

// Keyboard class
class Keyboard
{
public:
    string getInput()
    {
        string input;
        cin >> input;
        return input;
    }
};

// Main function
int main()
{
    ATM *atm = ATM::getInstance();

    atm->authenticate();
    atm->checkBalance();
    atm->withdraw();
    atm->deposit();
    atm->viewTransactionHistory();
}

/*
The Facade pattern provides a unified interface to a set of interfaces in a subsystem. It defines a higher-level
interface that makes the subsystem easier to use.

In this implementation:

1.  The ATM class acts as a facade, providing a simplified interface for interacting with the ATM system. It encapsulates
    the complexity of interactions with various subsystems such as card reading, cash dispensing, displaying messages,
    and managing transactions.
2.  The CardReader, CashDispenser, Display, Keyboard, and TransactionHistory classes represent the subsystems of the ATM system,
    each responsible for a specific aspect of functionality.
3.  The ATM class delegates the work to these subsystems through their respective interfaces without exposing their internal
    details to the client code.
*/