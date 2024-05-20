/*##########################################################################
System Requirements
--------------------
1. Vehicle Management: Handle various types of vehicles available for rent.
2. User Account Management: Manage customer registrations and profiles.
3. Rental Process: Enable users to rent and return vehicles.
4. Pricing and Payment: Calculate rental charges and process payments.
5. Reservation System: Support advance booking of vehicles.
---------------------------------------------------------------------------------
Core Use Cases
--------------
1. Adding and Managing Vehicles
2. Registering and Managing User Accounts
3. Renting and Returning Vehicles
4. Calculating Rental Charges
5. Handling Reservations
##########################################################################*/

#include <bits/stdc++.h>
using namespace std;

// Enum representing vehicle types
enum class VehicleType
{
    TWOWHEELER,
    CAR,
    TRUCK,
    LIMO,
    BUS
};

// Abstract base class for vehicles
class Vehicle
{
protected:
    std::string vehicleId;
    double ratePerDay;
    VehicleType type;

public:
    Vehicle(const std::string &vehicleId, double ratePerDay, VehicleType type)
        : vehicleId(vehicleId), ratePerDay(ratePerDay), type(type) {}

    virtual ~Vehicle() {}

    virtual std::string getVehicleId() const { return vehicleId; }
    virtual double getRatePerDay() const { return ratePerDay; }
    virtual VehicleType getType() const { return type; }

    // Common implementation of calculateRentalCharge method
    virtual double calculateRentalCharge(std::time_t rentalDate, std::time_t returnDate) const
    {
        double rentalDays = difftime(returnDate, rentalDate) / (60 * 60 * 24);
        return rentalDays * getRatePerDay();
    }
};

// Derived class for cars
class Car : public Vehicle
{
public:
    Car(const std::string &vehicleId, double ratePerDay)
        : Vehicle(vehicleId, ratePerDay, VehicleType::CAR) {}
};

// Derived class for trucks
class Truck : public Vehicle
{
public:
    Truck(const std::string &vehicleId, double ratePerDay)
        : Vehicle(vehicleId, ratePerDay, VehicleType::TRUCK) {}
};

// Rental class to manage rental details
class Rental
{
private:
    std::string rentalId;
    std::shared_ptr<User> user;
    std::shared_ptr<Vehicle> vehicle;
    std::time_t rentalDate;
    std::time_t returnDate;
    double totalCharge;

public:
    Rental(const std::string &rentalId, std::shared_ptr<User> user, std::shared_ptr<Vehicle> vehicle, std::time_t rentalDate)
        : rentalId(rentalId), user(user), vehicle(vehicle), rentalDate(rentalDate), returnDate(0), totalCharge(0.0) {}

    void completeRental(std::time_t returnDate)
    {
        this->returnDate = returnDate;
        this->totalCharge = calculateTotalCharge();
    }

    double calculateTotalCharge() const
    {
        return vehicle->calculateRentalCharge(rentalDate, returnDate);
    }

    std::string getRentalId() const { return rentalId; }
};

// Rental manager class to handle rental operations
class RentalManager
{
private:
    std::vector<std::shared_ptr<Rental>> rentals;

public:
    std::shared_ptr<Rental> rentVehicle(const std::shared_ptr<User> &user, const std::shared_ptr<Vehicle> &vehicle, std::time_t rentalDate)
    {
        auto rental = std::make_shared<Rental>(generateRentalId(), user, vehicle, rentalDate);
        rentals.push_back(rental);
        return rental;
    }

    void returnVehicle(const std::shared_ptr<Rental> &rental, std::time_t returnDate)
    {
        rental->completeRental(returnDate);
    }

private:
    std::string generateRentalId()
    {
        return "RENTAL_" + std::to_string(std::time(nullptr));
    }
};

// User class
class User
{
private:
    std::string userId;
    std::string name;
    std::string driverLicenseNumber;

public:
    User(const std::string &userId, const std::string &name, const std::string &driverLicenseNumber)
        : userId(userId), name(name), driverLicenseNumber(driverLicenseNumber) {}

    std::string getUserId() const { return userId; }
    std::string getName() const { return name; }
    std::string getDriverLicenseNumber() const { return driverLicenseNumber; }
};

// Vehicle rental system class to manage users, vehicles, and rentals
class VehicleRentalSystem
{
private:
    std::vector<std::shared_ptr<Vehicle>> vehicles;
    std::vector<std::shared_ptr<User>> users;
    RentalManager rentalManager;

public:
    void addUser(const std::shared_ptr<User> &user)
    {
        users.push_back(user);
    }

    void addVehicle(const std::shared_ptr<Vehicle> &vehicle)
    {
        vehicles.push_back(vehicle);
    }

    std::shared_ptr<Rental> rentVehicle(const std::shared_ptr<User> &user, const std::shared_ptr<Vehicle> &vehicle, std::time_t rentalDate)
    {
        return rentalManager.rentVehicle(user, vehicle, rentalDate);
    }

    void returnVehicle(const std::shared_ptr<Rental> &rental, std::time_t returnDate)
    {
        rentalManager.returnVehicle(rental, returnDate);
    }
};

int main()
{
    VehicleRentalSystem system;

    auto user1 = std::make_shared<User>("USR001", "John Doe", "DL123456");
    system.addUser(user1);

    auto car1 = std::make_shared<Car>("CAR001", 50.0);
    system.addVehicle(car1);

    auto truck1 = std::make_shared<Truck>("TRUCK001", 100.0);
    system.addVehicle(truck1);

    std::time_t rentalDate = std::time(nullptr);
    auto rentalCar = system.rentVehicle(user1, car1, rentalDate);
    auto rentalTruck = system.rentVehicle(user1, truck1, rentalDate);

    if (rentalCar != nullptr)
    {
        std::cout << "Car rented successfully!" << std::endl;
        // Other actions for car rental...
    }
    else
    {
        std::cout << "Failed to rent car." << std::endl;
    }

    if (rentalTruck != nullptr)
    {
        std::cout << "Truck rented successfully!" << std::endl;
        // Other actions for truck rental...
    }
    else
    {
        std::cout << "Failed to rent truck." << std::endl;
    }

    return 0;
}