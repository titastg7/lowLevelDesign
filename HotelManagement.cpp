/*##########################################################################
System Requirements
--------------------
1. Room Booking Management: Manage bookings for various types of rooms.
2. Customer Management: Handle customer information and booking history.
3. Room Service Management: Manage orders for food and other services.
4. Billing: Generate bills for customers based on their usage of services.
---------------------------------------------------------------------------------
Core Use Cases
--------------
1. Booking a Room: Customers can book different types of rooms.
2. Managing Customer Profiles: Storing and retrieving customer details.
3. Ordering Room Services: Placing orders for room-related services.
4. Generating Bills: Calculating and producing bills for customers.
##########################################################################*/

#include <bits/stdc++.h>
using namespace std;

class Room
{
private:
    std::string roomNumber;
    std::string roomType;
    bool isBooked;
    double roomPricePerDay;

public:
    Room(const std::string &roomNumber, const std::string &roomType, double price)
        : roomNumber(roomNumber), roomType(roomType), isBooked(false), roomPricePerDay(price) {}

    void bookRoom()
    {
        isBooked = true;
    }
    double getPrice()
    {
        return roomPricePerDay;
    }
    void vacateRoom()
    {
        isBooked = false;
    }

    // Getters and setters...
    const std::string &getRoomNumber() const
    {
        return roomNumber;
    }

    bool getIsBooked() const
    {
        return isBooked;
    }
};

class Customer
{
private:
    std::string customerId;
    std::string name;
    std::string phone;

public:
    Customer(const std::string &customerId, const std::string &name, const std::string &phone)
        : customerId(customerId), name(name), phone(phone) {}

    // Getters and setters...
};

class Booking
{
private:
    Room *room;
    Customer *customer;
    std::tm checkInDate;
    std::tm checkOutDate;
    double totalPrice;

public:
    Booking(Room *room, Customer *customer, const std::tm &checkIn, const std::tm &checkOut)
        : room(room), customer(customer), checkInDate(checkIn), checkOutDate(checkOut), totalPrice(0.0)
    {
        room->bookRoom();
    }

    // Method to complete booking
    void completeBooking()
    {
        totalPrice = calculateTotalPrice();
        room->vacateRoom();
    }

    double calculateTotalPrice() const
    {
        double totalDays = difftime(mktime(&checkOutDate), mktime(&checkInDate)) / (60 * 60 * 24);
        return totalDays * room->getPrice();
    }
    // Getters and setters...
};

class Hotel
{
private:
    std::string hotelName;
    std::vector<Room *> rooms;
    std::vector<Booking *> bookings;

public:
    Hotel(const std::string &hotelName)
        : hotelName(hotelName) {}

    void addRoom(Room *room)
    {
        rooms.push_back(room);
    }

    Booking *bookRoom(const std::string &roomNumber, Customer *customer, const std::tm &checkIn, const std::tm &checkOut)
    {
        Room *room = findRoomByNumber(roomNumber);

        if (!room && !room->bookRoom())
        {
            return nullptr;
        }

        Booking *booking = new Booking(room, customer, checkIn, checkOut);
        bookings.push_back(booking);
        return booking;
    }
    Room *findRoomByNumber(const std::string &roomNumber) const
    {
        for (auto room : rooms)
        {
            if (room->getRoomNumber() == roomNumber)
                return room;
        }
        return nullptr;
    }

    // Getters and setters...
};

int main()
{
    // Create instances of rooms, customers, and hotel
    Room room1("101", "Single", 100.0); // Example price
    Room room2("102", "Double", 150.0); // Example price
    Customer customer("1", "John Doe", "1234567890");
    Hotel hotel("Sample Hotel");

    // Add rooms to the hotel
    hotel.addRoom(&room1);
    hotel.addRoom(&room2);

    // Book a room
    std::tm checkIn = {};
    std::tm checkOut = {};
    std::string checkInStr = "2024-03-15";  // Example date
    std::string checkOutStr = "2024-03-18"; // Example date
    strptime(checkInStr.c_str(), "%Y-%m-%d", &checkIn);
    strptime(checkOutStr.c_str(), "%Y-%m-%d", &checkOut);
    Booking *booking = hotel.bookRoom("101", &customer, checkIn, checkOut);

    if (booking != nullptr)
    {
        std::cout << "Room booked successfully!" << std::endl;
        // Other actions...

        // Complete booking
        booking->completeBooking();
        std::cout << "Booking completed." << std::endl;
        std::cout << "Total price: $" << booking->getTotalPrice() << std::endl;
    }
    else
    {
        std::cout << "Failed to book room." << std::endl;
    }

    return 0;
}