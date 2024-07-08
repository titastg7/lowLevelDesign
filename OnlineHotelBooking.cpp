/*##########################################################################
System Requirements
--------------------
1. Room Booking Management: Manage bookings for various types of rooms.
2. Customer Management: Handle customer information and booking history.
3. Room Service Management: Manage orders for food and other services.
4. Billing: Generate bills for customers.
---------------------------------------------------------------------------------
Core Use Cases
--------------
1. Booking a Room: Customers can book different types of rooms.
2. Managing Customer Profiles: Storing and retrieving customer details.
3. Generating Bills: Calculating and producing bills for customers.
4. Managing Hotels: Storing details of hotel and room bookings
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
    Room(const std::string &roomNumber, const std::string &roomType, double pricePerDay)
        : roomNumber(roomNumber), roomType(roomType), isBooked(false), roomPricePerDay(pricePerDay) {}

    void bookRoom()
    {
        isBooked = true;
    }

    void vacateRoom()
    {
        isBooked = false;
    }
    double getPricePerDay() const
    {
        return roomPricePerDay;
    }

    // Getters...
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

    // Getters...
};

class Booking
{
private:
    Room *room;
    string bookingid;
    Customer *customer;
    std::tm checkInDate;
    std::tm checkOutDate;
    double totalPrice;

public:
    Booking(Room *room, Customer *customer, const std::tm &checkIn, const std::tm &checkOut)
        : room(room), customer(customer), checkInDate(checkIn), checkOutDate(checkOut)
    {
        generateId();
        room->bookRoom();
    }

    void generateId()
    {
        static int counter = 0;
        bookingId = "B" + std::to_string(++counter);
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
        return totalDays * room->getPricePerDay();
    }
    double getTotalPrice() const
    {
        return totalPrice;
    }
    // Getters...
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

        if (room && !room->getIsBooked())
        {
            Booking *booking = new Booking(room, customer, checkIn, checkOut);
            booking->totalPrice = booking->calculateTotalPrice();
            bookings.push_back(booking);
            return booking;
        }

        return nullptr;
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

    // Getters...
};

class OnlineHotelBookingSystem
{
private:
    std::vector<Hotel *> hotels;
    vector<Customer *> customers;

public:
    void addHotel(Hotel *hotel)
    {
        hotels.push_back(hotel);
    }

    void addCustomer(Customer *user)
    {
        customers.push_back(user);
    }

    Booking *bookRoom(const std::string &hotelName, const std::string &roomNumber, Customer *customer,
                      const std::tm &checkIn, const std::tm &checkOut)
    {
        Hotel *hotel = findHotelByName(hotelName);
        if (hotel)
        {
            return hotel->bookRoom(roomNumber, customer, checkIn, checkOut);
        }
        return nullptr;
    }

    string &getHotelName() const
    {
        return hotelName;
    }

private:
    Hotel *findHotelByName(const std::string &hotelName) const
    {
        for (auto hotel : hotels)
        {
            if (hotel->getHotelName() == hotelName)
                return hotel;
        }

        return (it != hotels.end()) ? *it : nullptr;
    }
};

int main()
{
    // Create instances of rooms, customers, and hotel
    Room room1("101", "Single");
    Room room2("102", "Double");
    Customer customer("1", "John Doe", "1234567890");
    Hotel hotel("Sample Hotel");

    // Add rooms to the hotel
    hotel.addRoom(&room1);
    hotel.addRoom(&room2);

    // Create OnlineHotelBookingSystem instance and add hotel
    OnlineHotelBookingSystem bookingSystem;
    bookingSystem.addHotel(&hotel);
    bookingSystem.addCustomer(customer);
    // Book a room
    std::tm checkIn = {};
    std::tm checkOut = {};
    std::string checkInStr = "2024-03-15";  // Example date
    std::string checkOutStr = "2024-03-18"; // Example date
    strptime(checkInStr.c_str(), "%Y-%m-%d", &checkIn);
    strptime(checkOutStr.c_str(), "%Y-%m-%d", &checkOut);
    Booking *booking = bookingSystem.bookRoom("Sample Hotel", "101", &customer, checkIn, checkOut);

    if (booking != nullptr)
    {
        std::cout << "Room booked successfully!" << std::endl;
        // Other actions...
    }
    else
    {
        std::cout << "Failed to book room." << std::endl;
    }

    return 0;
}
