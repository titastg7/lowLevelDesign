/*##########################################################################
System Requirements
--------------------
1. User Account Management: Manage user account creation and maintenance.
2. Wallet Management: Allow users to add, withdraw, and check balances.
3. Transaction Processing: Handle transactions and maintain a history.
4. Security and Authentication: Ensure secure access and transaction integrity.
---------------------------------------------------------------------------------
Core Use Cases
--------------
1. Creating and Managing User Accounts
2. Adding and Withdrawing Funds
3. Making and Receiving Payments
4. Viewing Transaction History
##########################################################################*/

#include <bits/stdc++.h>
using namespace std;

class Transaction
{
private:
    string type;
    double amount;
    time_t date;
    double postTransactionBalance;

public:
    Transaction(const string &type, double amount, double postTransactionBalance)
        : type(type), amount(amount), postTransactionBalance(postTransactionBalance)
    {
        date = time(nullptr);
    }

    // Getters...
};

class Wallet
{
private:
    User *user;
    double balance;
    vector<Transaction> transactionHistory;

public:
    Wallet(User *user) : user(user), balance(0.0) {}

    bool addFunds(double amount)
    {
        if (amount <= 0)
        {
            return false;
        }
        balance += amount;
        transactionHistory.push_back(Transaction("Deposit", amount, balance));
        return true;
    }

    bool withdrawFunds(double amount)
    {
        if (amount > balance)
        {
            return false;
        }
        balance -= amount;
        transactionHistory.push_back(Transaction("Withdrawal", amount, balance));
        return true;
    }

    double getBalance() const
    {
        return balance;
    }

    const vector<Transaction> &getTransactionHistory() const
    {
        return transactionHistory;
    }
};

class User
{
private:
    string userId;
    string name;
    Wallet *wallet;

public:
    User(const string &userId, const string &name) : userId(userId), name(name)
    {
        wallet = new Wallet(this);
    }

    bool addFunds(double amount)
    {
        return wallet->addFunds(amount);
    }

    bool withdrawFunds(double amount)
    {
        return wallet->withdrawFunds(amount);
    }

    double getBalance() const
    {
        return wallet->getBalance();
    }

    const vector<Transaction> &getTransactionHistory() const
    {
        return wallet->getTransactionHistory();
    }

    const string &getUserId() const
    {
        return userId;
    }

    // Other necessary methods...
};

class DigitalWalletSystem
{
private:
    vector<User> users;

public:
    DigitalWalletSystem() {}

    void addUser(const User &user)
    {
        users.push_back(user);
    }

    bool transferFunds(const string &senderUserId, const string &receiverUserId, double amount)
    {
        User *sender = nullptr;
        User *receiver = nullptr;

        for (auto &user : users)
        {
            if (user.getUserId() == senderUserId)
            {
                sender = &user;
            }
            else if (user.getUserId() == receiverUserId)
            {
                receiver = &user;
            }
        }

        if (sender && receiver)
        {
            if (sender->withdrawFunds(amount))
            {
                receiver->addFunds(amount);
                return true;
            }
        }
        return false;
    }

    // Other necessary methods...
};

int main()
{
    DigitalWalletSystem digitalWalletSystem;

    User user1("123", "Alice");
    User user2("456", "Bob");

    digitalWalletSystem.addUser(user1);
    digitalWalletSystem.addUser(user2);

    digitalWalletSystem.transferFunds("123", "456", 50);

    return 0;
}