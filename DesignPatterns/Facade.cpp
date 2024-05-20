#include <iostream>
#include <string>

using namespace std;

class User
{
private:
    string name, email;
    int phoneNumber; // Added phoneNumber for completeness

public:
    User(string _name, string _email, int _phoneNumber) : name(_name), email(_email), phoneNumber(_phoneNumber) {}
    string getClass() const { return "User"; }
    string getName()
    {
        return name;
        
    }
};

class TicketSystem
{
private:
    int ticketNumber;

public:
    void createTicket(int ticketNumber, User user, string movie)
    {
        // Assuming some logic here to generate a unique ticket number
        this->ticketNumber = ticketNumber;
        cout << "Ticket created for " << user.getClass() << " " << user.getName() << " Ticket Number: " << ticketNumber << " Movie: " << movie << endl;
    }

    int getTicketNumber()
    {
        return ticketNumber;
    }

    bool validateAvailability(string movie)
    {
        // Some logic to check availability, for simplicity always return true
        return true;
    }
};

class PaymentSystem
{
public:
    void chargeCard()
    {
        // Logic to charge the user's card
        cout << "Card charged successfully!" << endl;
    }
};

class NotificationSystem
{
public:
    void sendEmail(User user, int ticketNumber)
    {
        cout << "Email sent to " << user.getClass() << " for Ticket Number: " << ticketNumber << endl;
    }

    void sendSms(User user, int ticketNumber)
    {
        cout << "SMS sent to " << user.getClass() << " for Ticket Number: " << ticketNumber << endl;
    }
};

class BookingFacade
{
public:
    void createBooking(User user)
    {
        TicketSystem ts;
        PaymentSystem ps;
        NotificationSystem ns;

        bool isBookingPossible = ts.validateAvailability("Avenger");
        if (isBookingPossible)
        {
            ts.createTicket(100, user, "Avenger");
            ps.chargeCard();
            ns.sendEmail(user, ts.getTicketNumber());
            ns.sendSms(user, ts.getTicketNumber());
        }
    }
};

int main()
{
    User user("Titas", "titas@example.com", 9987092513);
    BookingFacade bookingFacade;
    bookingFacade.createBooking(user);

    return 0;
}