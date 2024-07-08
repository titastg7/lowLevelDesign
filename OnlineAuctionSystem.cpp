/*##########################################################################
System Requirements
--------------------
1. Auction Management: Create and manage auctions with item details, starting prices, and durations.
2. User Account Management: Handle user registrations for sellers and bidders.
3. Bidding Process: Allow users to place and track bids.
4. Auction Monitoring: Enable users to view ongoing auctions and status.
5. Transaction Processing: Handle winning bid transactions.
---------------------------------------------------------------------------------
Core Use Cases
--------------
1. Creating and Managing Auctions
2. Registering and Managing User Accounts
3. Placing and Tracking Bids
4. Monitoring Auction Progress
5. Processing Transactions

##########################################################################*/

#include <bits/stdc++.h>
using namespace std;

// Observer interface
class AuctionObserver
{
public:
    virtual void update(const string &message) = 0;
};

// Observable interface
class Publisher
{
public:
    virtual void registerObserver(AuctionObserver *observer) = 0;
    virtual void unregisterObserver(AuctionObserver *observer) = 0;

    virtual void notifyObservers(const string &message) = 0;
};

// User class
class User
{
protected:
    string userId;
    string name;
    string email;
    string password;

public:
    User(const string &userId, const string &name, const string &email, const string &password)
        : userId(userId), name(name), email(email), password(password) {}

    bool authenticate(const string &enteredPassword) const
    {
        return enteredPassword == password;
    }

    const string &getUserId() const
    {
        return userId;
    }

    // Other user methods...
};

// Buyer class
class Buyer : public User, public AuctionObserver
{
private:
    unordered_map<string, double> subscribedAuctions;

public:
    Buyer(const string &userId, const string &name, const string &email, const string &password)
        : User(userId, name, email, password) {}

    void subscribeToAuction(const string &auctionId, double initialBid)
    {
        subscribedAuctions[auctionId] = initialBid;
    }

    virtual void update(const string &message) override
    {
        cout << "Buyer Update: " << message << endl;
    }

    // Other buyer methods...
};

// Seller class
class Seller : public User, public AuctionObserver
{
private:
    vector<string> auctions;

public:
    Seller(const string &userId, const string &name, const string &email, const string &password)
        : User(userId, name, email, password) {}

    void addAuction(const string &auctionId)
    {
        auctions.push_back(auctionId);
    }

    virtual void update(const string &message) override
    {
        cout << "Seller Update: " << message << endl;
    }

    // Other seller methods...
};

// Product class
class Product
{
private:
    string productId;
    // Other product details...

public:
    Product(const string &productId) : productId(productId) {}

    // Getters and setters for product details...
};

// Enum for Auction state
enum class AuctionState
{
    ACTIVE,
    CLOSED,
    PENDING_APPROVAL
};

// Auction class
class Auction : public Publisher
{
private:
    string auctionId;
    string sellerId;
    Product product;
    double startingPrice;
    double highestBid;
    AuctionState state;
    vector<AuctionObserver *> observers;

public:
    Auction(const string &auctionId, const string &sellerId, const Product &product, double startingPrice)
        : auctionId(auctionId), sellerId(sellerId), product(product), startingPrice(startingPrice), highestBid(startingPrice) {}

    void registerObserver(AuctionObserver *observer) override
    {
        observers.push_back(observer);
    }

    void unregisterObserver(AuctionObserver *observer) override
    {
        observers.erase(remove(observers.begin(), observers.end(), observer), observers.end());
    }

    void notifyObservers(const string &message) override
    {
        for (AuctionObserver *observer : observers)
        {
            observer->update(message);
        }
    }

    void updateState(AuctionState newState)
    {
        state = newState;
        string message = auctionId + " state update to " + to_string(static_cast<int>(newState));
        notifyObservers(message);
    }

    void placeBid(User *bidder, double bidAmount)
    {
        if (bidAmount > highestBid)
        {
            highestBid = bidAmount;
            string message = bidder->getUserId() + " placed a bid of $" + to_string(bidAmount) + " on Auction ID: " + auctionId;
            notifyObservers(message);
        }
        else
        {
            string message = "Bid not placed. Auction is closed or bid amount is too low.";
            notifyObservers(message);
        }
    }

    string getSellerId() const
    {
        return sellerId;
    }
    string &getAuctionId() const
    {
        return auctionId;
    }

    double getHighestBid() const
    {
        return highestBid;
    }
};

// Online Auction System class
class OnlineAuctionSystem
{
private:
    vector<Auction *> auctions;
    vector<User *> users;

public:
    void addAuction(Auction *auction)
    {
        auctions.push_back(auction);
        string sellerId = auction->getSellerId();

        for (auto user : users)
        {
            if (user->getUserId() == sellerId)
            {
                dynamic_cast<Seller *>(user)->addAuction(auction->getAuctionId());
            }
        }
    }

    void addUser(User *user)
    {
        users.push_back(user);
    }

    void registerEntityToAuction(const string &entityId, const string &auctionId, double initialBid,
                                 bool isBuyer, AuctionObserver *observer)
    {
        for (User *user : users)
        {
            if (user->getUserId() == entityId)
            {
                if (isBuyer)
                {
                    dynamic_cast<Buyer *>(user)->subscribeToAuction(auctionId, initialBid);
                }
                dynamic_cast<Auction *>(auctionId)->registerObserver(observer);
                break;
            }
        }
    }

    void updateAuctionState(const string &auctionId, AuctionState newState)
    {
        for (Auction *auction : auctions)
        {
            if (auction->getAuctionId() == auctionId)
            {
                auction->updateState(newState);
                break;
            }
        }
    }

    void placeBidOnAuction(const string &auctionId, User *bidder, double bidAmount)
    {
        for (Auction *auction : auctions)
        {
            if (auction->getAuctionId() == auctionId)
            {
                auction->placeBid(bidder, bidAmount);
                break;
            }
        }
    }
};

int main()
{
    // Usage example
    OnlineAuctionSystem auctionSystem;

    Buyer buyer("1", "John", "john@example.com", "password123");
    Seller seller("2", "Alice", "alice@example.com", "password456");
    Product product("12345");
    Auction auction("A1", seller.getUserId(), product, 100.0);

    auctionSystem.addUser(&buyer);
    auctionSystem.addUser(&seller);
    auctionSystem.addAuction(&auction);

    auctionSystem.registerEntityToAuction(buyer.getUserId(), auction.getAuctionId(), 100.0, true, &buyer);
    auctionSystem.registerEntityToAuction(seller.getUserId(), auction.getAuctionId(), 0.0, false, &seller);

    auctionSystem.updateAuctionState(auction.getAuctionId(), AuctionState::ACTIVE);

    auctionSystem.placeBidOnAuction(auction.getAuctionId(), &buyer, 120.0);
    auctionSystem.placeBidOnAuction(auction.getAuctionId(), &seller, 110.0);

    return 0;
}

/*
Observer pattern:

    AuctionObserver interface and concrete classes Buyer and Seller implementing the update method.
    Auction class acts as the subject (Publisher) that maintains a list of observers and notifies them of changes.

Facade Pattern:

    OnlineAuctionSystem class provides a simplified interface for the client to interact with various subsystems like auctions,
    users, and observers.
*/
