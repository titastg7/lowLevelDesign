/*##########################################################################
System Requirements
--------------------
1. Event Management: Manage concert details including dates, venues, and artists.
2. User Account Management: Handle user registrations and profiles.
3. Ticket Booking Process: Enable users to book tickets and select seats.
4. Payment Processing: Handle ticket payments and issue receipts.
5. Ticket Cancellation and Refund: Manage cancellations and refunds.
---------------------------------------------------------------------------------
Core Use Cases
--------------
1. Creating and Managing Concert Events
2. Registering and Managing User Accounts
3. Booking and Canceling Tickets
4. Processing Payments and Issuing Tickets
5. Handling Refunds

##########################################################################*/

#include <bits/stdc++.h>
using namespace std;

// Payment Status Enum
enum class PaymentStatus
{
    PENDING,
    COMPLETED,
    FAILED
};

// Abstract Event interface
class Event
{
public:
    virtual std::string getName() const = 0;
    virtual std::string getVenue() const = 0;
    virtual std::string getDate() const = 0;
    virtual void displayDetails() const = 0;
    virtual int getMaxCapacity() const = 0;
    virtual double getPricePerTicket() const = 0;
    virtual ~Event() {}
};

// Concrete Concert class implementing Event interface
class Concert : public Event
{
    std::string name;
    std::string venue;
    std::string date;
    int maxCapacity;
    double pricePerTicket;

public:
    Concert(const std::string &name, const std::string &venue, const std::string &date, int maxCapacity)
        : name(name), venue(venue), date(date), maxCapacity(maxCapacity) {}

    std::string getName() const override { return name; }
    std::string getVenue() const override { return venue; }
    std::string getDate() const override { return date; }
    int getMaxCapacity() const override { return maxCapacity; }
    double getPricePerTicket() const override { return pricePerTicket; }

    void displayDetails() const override
    {
        std::cout << "Concert: " << name << ", Venue: " << venue << ", Date: " << date << ", Capacity: " << maxCapacity << std::endl;
    }
};

// Abstract User interface
class User
{
public:
    virtual std::string getUserId() const = 0;
    virtual std::string getName() const = 0;
    virtual std::string getEmail() const = 0;
    virtual ~User() {}
};

// Concrete User class implementing User interface
class ConcertUser : public User
{
    std::string userId;
    std::string name;
    std::string email;

public:
    ConcertUser(const std::string &userId, const std::string &name, const std::string &email)
        : userId(userId), name(name), email(email) {}

    std::string getUserId() const override { return userId; }
    std::string getName() const override { return name; }
    std::string getEmail() const override { return email; }
};

// Abstract Ticket interface
class Ticket
{
public:
    virtual Event *getEvent() const = 0;
    virtual User *getUser() const = 0;
    virtual int getNumTickets() const = 0;
    virtual void displayDetails() const = 0;
    virtual double getTotalPrice() const = 0;
    virtual PaymentStatus getStatus() const = 0;
    virtual ~Ticket() {}
};

// Concrete Ticket class implementing Ticket interface
class ConcertTicket : public Ticket
{
    Event *event;
    User *user;
    int numTickets; // Number of tickets booked
    PaymentStatus status;

public:
    ConcertTicket(Event *event, User *user, int numTickets, double price)
        : event(event), user(user), numTickets(numTickets), price(price) {}

    Event *getEvent() const override { return event; }
    User *getUser() const override { return user; }
    int getNumTickets() const override { return numTickets; }
    double getTotalPrice() const override { return numTickets * event->getPricePerTicket(); }
    PaymentStatus getStatus() const override { return status; }

    void displayDetails() const override
    {
        std::cout << "Ticket Details: ";
        event->displayDetails();
        std::cout << "User: " << user->getName() << ", Number of Tickets: " << numTickets << ", Price per Ticket: $" << price << std::endl;
    }
};

// PaymentStrategy interface
class PaymentStrategy
{
public:
    virtual bool processPayment(const std::string &userId, double amount) = 0;
    virtual ~PaymentStrategy() {}
};

// Concrete CreditCard class implementing PaymentStrategy
class CreditCard : public PaymentStrategy
{
    std::string creditCardDetails;

public:
    CreditCard(const std::string &creditCardDetails)
        : creditCardDetails(creditCardDetails) {}

    bool processPayment(const std::string &userId, double amount) override
    {
        // Simulate credit card payment processing
        if (amount > 0)
        {
            std::cout << "Credit Card Payment processed successfully for user " << userId << " amount $" << amount << std::endl;
            return true;
        }
        else
        {
            std::cout << "Credit Card Payment failed for user " << userId << " amount $" << amount << std::endl;
            return false;
        }
    }
};

// Concrete Netbanking class implementing PaymentStrategy
class Netbanking : public PaymentStrategy
{
    std::string accountDetails;

public:
    Netbanking(const std::string &accountDetails)
        : accountDetails(accountDetails) {}

    bool processPayment(const std::string &userId, double amount) override
    {
        // Simulate netbanking payment processing
        if (amount > 0)
        {
            std::cout << "Netbanking Payment processed successfully for user " << userId << " amount $" << amount << std::endl;
            return true;
        }
        else
        {
            std::cout << "Netbanking Payment failed for user " << userId << " amount $" << amount << std::endl;
            return false;
        }
    }
};

// Facade to manage ticket booking system
class TicketBookingFacade
{
    std::vector<Event *> events;
    std::vector<User *> users;
    std::unordered_map<std::string, std::vector<Ticket *>> bookings;

public:
    TicketBookingFacade() {}
    ~TicketBookingFacade()
    {
        for (auto event : events)
            delete event;
        for (auto user : users)
            delete user;
        for (auto &pair : bookings)
        {
            for (auto ticket : pair.second)
                delete ticket;
        }
    }
    Event *createConcert(string &name, string &venue, string &date, int maxCapacity,double pricePerTicket)
    {
        Event* concert = new Concert(name, venue, date, maxCapacity, pricePerTicket);        
        events.push_back(concert);
        return concert;
    }

    User *registerUser(string &userId, string &name, string &email)
    {
        User *user = new ConcertUser(userId, name, email);
        users.push_back(user);
        return user;
    }

    Ticket *bookTicket(Event *event, User *user, int numTickets, PaymentStrategy *paymentStrategy)
    {
        // Check if tickets are available
        if (!areTicketsAvailable(event, numTickets))
        {
            std::cout << "Tickets are not available for this event." << std::endl;
            return nullptr;
        }

        // Check if user can book the number of tickets requested
        if (numTickets > 10)
        {
            std::cout << "User cannot book more than 10 tickets at a time." << std::endl;
            return nullptr;
        }

        PaymentStatus paymentStatus = paymentStrategy->processPayment(user->getUserId(), numTickets * event->getPricePerTicket()) ? PaymentStatus::COMPLETED : PaymentStatus::FAILED;
        Ticket *ticket = new ConcertTicket(event, user, numTickets, paymentStatus);
        bookings[user->getUserId()].push_back(ticket);
        return ticket;
    }

    void cancelTicket(Ticket *ticket)
    {
        auto &userTickets = bookings[ticket->getUser()->getUserId()];
        auto it = std::find(userTickets.begin(), userTickets.end(), ticket);
        if (it != userTickets.end())
        {
            userTickets.erase(it);
            delete ticket;
        }
    }

    void displayAllEvents() const
    {
        std::cout << "---- All Concert Events ----" << std::endl;
        for (auto event : events)
        {
            event->displayDetails();
        }
        std::cout << "-----------------------------" << std::endl;
    }

    bool areTicketsAvailable(Event *event, int numTickets) const
    {
        // Count booked tickets for the event
        int bookedTickets = 0;
        for (const auto &pair : bookings)
        {
            for (auto ticket : pair.second)
            {
                if (ticket->getEvent() == event && ticket->getStatus() == PaymentStatus::COMPLETED)
                {
                    bookedTickets += dynamic_cast<ConcertTicket *>(ticket)->getNumTickets();
                }
            }
        }

        // Check if available tickets are less than requested tickets
        return (bookedTickets + numTickets <= event->getMaxCapacity());
    }
};

int main()
{
    // Initialize facade
    TicketBookingFacade facade;

    // Create events
    Event *concert1 = facade.createConcert("Rock Fest", "Main Arena", "2024-08-15", 100,50.0);
    Event *concert2 = facade.createConcert("Pop Night", "City Hall", "2024-09-20", 150, 60.0);
    // Register users
    User *user1 = facade.registerUser("USER001", "Alice", "alice@example.com");
    User *user2 = facade.registerUser("USER002", "Bob", "bob@example.com");

    // Create payment processors
    PaymentStrategy* creditCardPayment = new CreditCard("1234 5678 9876 5432");
    PaymentStrategy* netbankingPayment = new Netbanking("user@example.com");

    // Book tickets with different payment methods
    Ticket *ticket1 = facade.bookTicket(concert1, user1, 2, creditCardPayment); // User 1 books 2 tickets
    Ticket *ticket2 = facade.bookTicket(concert2, user2, 4, netbankingPayment); // User 2 books 4 tickets

    // Check if tickets are available
    std::cout << "Tickets available for Rock Fest: " << (facade.areTicketsAvailable(concert1, 1) ? "Yes" : "No") << std::endl;
    std::cout << "Tickets available for Pop Night: " << (facade.areTicketsAvailable(concert2, 1) ? "Yes" : "No") << std::endl;

    // Display all events
    facade.displayAllEvents();

    // Cleanup
    delete creditCardPayment;
    delete netbankingPayment;
    delete ticket1;
    delete ticket2;

    return 0;
}

/*
Facade Design Pattern:

TicketBookingFacade: Acts as a facade to simplify the interface for interacting with complex subsystems like events (Event), users (User),
tickets (Ticket), and payment strategies (PaymentStrategy).
Clients interact with the facade to perform operations without needing to understand the inner workings of these subsystems.

Strategy Design Pattern:

PaymentStrategy: This interface defines a family of algorithms (CreditCard, Netbanking classes) encapsulated in concrete strategy classes.
It allows selecting a payment strategy at runtime and decouples the client (TicketBookingFacade) from the specific payment implementation.

*/
