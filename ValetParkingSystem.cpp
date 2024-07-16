/*##########################################################################
System Requirements
--------------------
Valet parking system
--------------------

Design a system responsible for indicating to a valet, which spots to park cars in, issue tickets, and accept tickets to
inform a valet which parking spot a car is located in.

Details:

The valet parking system should be able to accept multiple types of cars, and indicate a spot to park them in. Spots are identified by numbers.
Each parking spot can cater to certain types of vehicles.

There are 2 types of cars

Sedan
SUV
Both types of cars can also be EV (electric vehicles).
There are 3 types of parking spots

Regular
EV
SUV
Each parking spot can accommodate certain types of vehicles:

SUVs can only be parked in “SUV”, or “EV” (if electric) parking spots.
Electric cars should be parked in EV vehicle parking spots,
but if none are available, they can be parked in other spots.
EV parking spots are big enough to accommodate SUVs and Sedan cars.
Sedan cars can fit in any spot
At the beginning of a night, the valet system will be initialized with data on the parking spots.
Every spot is numbered, and given a type (“SUV”, “EV” or “regular”). The valet system will be
initialized with an array of parking spots. Each spot has a spot number and type.

Eg. [(1, regular), (2, regular), (3, SUV), (4, EV)]

##########################################################################*/

#include <bits/stdc++.h>
using namespace std;

// Enum for car types
enum class CarType
{
    Sedan,
    SUV
};

// Enum for spot types
enum class SpotType
{
    Regular,
    EV,
    SUV
};

class Car
{
public:
    Car(CarType type, bool isElectric) : type(type), isElectric(isElectric) {}

    CarType getType() const { return type; }
    bool isElectricCar() const { return isElectric; }

private:
    CarType type;
    bool isElectric;
};

// Class representing a parking spot
class ParkingSpot
{
public:
    ParkingSpot(int number, SpotType type) : spotNumber(number), spotType(type), available(true) {}

    bool isAvailable() const
    {
        return available;
    }
    SpotType getSpotType() const
    {
        return spotType;
    }
    void occupySpot()
    {
        available = false;
    }
    void freeSpot()
    {
        available = true;
    }
    int getSpotNumber() const
    {
        return spotNumber;
    }

private:
    int spotNumber;
    SpotType spotType;
    bool available;
};

// Strategy interface for parking
class ParkingStrategy
{
public:
    virtual int findSpot(vector<ParkingSpot> &parkingSpots, const Car &car) const = 0;
    virtual ~ParkingStrategy() = default;
};

// Concrete strategy for SUV cars
class SUVCarParkingStrategy : public ParkingStrategy
{
public:
    int findSpot(vector<ParkingSpot> &parkingSpots, const Car &car) const override
    {
        for (auto &spot : parkingSpots)
        {
            if (spot.isAvailable() && spot.getSpotType() == SpotType::SUV)
            {
                spot.occupySpot();
                return spot.getSpotNumber();
            }
        }
        // Finally try EV spots
        for (auto &spot : parkingSpots)
        {
            if (spot.isAvailable() && spot.getSpotType() == SpotType::EV)
            {
                spot.occupySpot();
                return spot.getSpotNumber();
            }
        }
        return -1; // No available SUV spot found
    }
};

// Concrete strategy for sedan cars
class SedanCarParkingStrategy : public ParkingStrategy
{
public:
    int findSpot(vector<ParkingSpot> &parkingSpots, const Car &car) const override
    {
        // First try regular spots
        for (auto &spot : parkingSpots)
        {
            if (spot.isAvailable() && spot.getSpotType() == SpotType::Regular)
            {
                spot.occupySpot();
                return spot.getSpotNumber();
            }
        }

        // Then try SUV spots
        if (car.getType() == CarType::SUV)
        {
            for (auto &spot : parkingSpots)
            {
                if (spot.isAvailable() && spot.getSpotType() == SpotType::SUV)
                {
                    spot.occupySpot();
                    return spot.getSpotNumber();
                }
            }
        }

        // Finally try EV spots
        for (auto &spot : parkingSpots)
        {
            if (spot.isAvailable() && spot.getSpotType() == SpotType::EV)
            {
                spot.occupySpot();
                return spot.getSpotNumber();
            }
        }

        return -1; // No available spot found
    }
};

// Concrete strategy for electric cars
class ElectricCarParkingStrategy : public ParkingStrategy
{
public:
    int findSpot(vector<ParkingSpot> &parkingSpots, const Car &car) const override
    {
        // First try to find an available EV spot
        for (auto &spot : parkingSpots)
        {
            if (spot.isAvailable() && spot.getSpotType() == SpotType::EV)
            {
                spot.occupySpot();
                return spot.getSpotNumber();
            }
        }

        // If no EV spot available, delegate to SUV or Sedan strategy based on car type
        ParkingStrategy *fallbackStrategy = nullptr;
        if (car.getType() == CarType::SUV)
        {
            fallbackStrategy = new SUVCarParkingStrategy();
        }
        else
        {
            fallbackStrategy = new SedanCarParkingStrategy();
        }

        int spotNumber = fallbackStrategy->findSpot(parkingSpots, car);
        delete fallbackStrategy;
        return spotNumber;
    }
};

// Singleton class to manage parking strategies
class StrategyManager
{
    static StrategyManager *instance;
    static mutex mtx;

    StrategyManager() {} // Private constructor to prevent instantiation

public:
    static StrategyManager *getInstance()
    {
        if (instance == nullptr)
        {
            mtx.lock();
            if (instance == nullptr)
            {
                instance = new StrategyManager();
            }
            mtx.unlock();
        }
        return instance;
    }

    StrategyManager(const StrategyManager &) = delete;
    void operator=(const StrategyManager &) = delete;

    ParkingStrategy *getStrategy(const Car &car)
    {
        if (car.isElectricCar())
        {
            return new ElectricCarParkingStrategy();
        }
        else if (car.getType() == CarType::SUV)
        {
            return new SUVCarParkingStrategy();
        }
        else
        {
            return new SedanCarParkingStrategy();
        }
    }
};

StrategyManager *StrategyManager::instance = nullptr;
mutex StrategyManager::mtx;

// Singleton Class representing the valet parking system
class ValetParkingSystem
{
    static ValetParkingSystem *valetParkingSystem;
    vector<ParkingSpot> parkingSpots;
    static mutex mtx;

    ValetParkingSystem() {} // Private constructor to prevent instantiation

public:
    // Static method to get the singleton instance
    static ValetParkingSystem *getInstance()
    {
        if (valetParkingSystem == nullptr)
        {
            mtx.lock();
            if (valetParkingSystem == nullptr)
            {
                valetParkingSystem = new ValetParkingSystem();
            }
            mtx.unlock();
        }
        return valetParkingSystem;
    }

    // Delete copy constructor and assignment operator
    ValetParkingSystem(const ValetParkingSystem &) = delete;
    void operator=(const ValetParkingSystem &) = delete;

    void initializeParkingSpots(const vector<pair<int, SpotType>> &spotsData)
    {
        for (const auto &data : spotsData)
        {
            parkingSpots.push_back(ParkingSpot(data.first, data.second));
        }
    }

    int issueTicket(const Car &car)
    {
        StrategyManager *strategyManager = StrategyManager::getInstance();
        ParkingStrategy *strategy = strategyManager->getStrategy(car);
        int spotNumber = strategy->findSpot(parkingSpots, car);
        delete strategy;
        return spotNumber;
    }

    void acceptTicket(int ticketNumber)
    {
        for (auto &spot : parkingSpots)
        {
            if (spot.getSpotNumber() == ticketNumber)
            {
                spot.freeSpot();
                return;
            }
        }
    }
};

ValetParkingSystem *ValetParkingSystem::valetParkingSystem = nullptr;
mutex ValetParkingSystem::mtx;

int main()
{
    ValetParkingSystem *valetSystem = ValetParkingSystem::getInstance();

    // Initialize parking spots
    vector<pair<int, SpotType>> spotsData = {{1, SpotType::Regular}, {2, SpotType::Regular}, {3, SpotType::Regular}, {4, SpotType::EV}};
    valetSystem->initializeParkingSpots(spotsData);

    // Create cars
    Car sedanCar(CarType::Sedan, false); // Non-electric Sedan
    Car suvEvCar(CarType::SUV, true);    // Electric SUV
    Car suvCar(CarType::SUV, false);

    // Issue tickets
    int ticket1 = valetSystem->issueTicket(sedanCar);
    int ticket2 = valetSystem->issueTicket(suvEvCar);
    int ticket3 = valetSystem->issueTicket(suvCar);

    cout << "Ticket 1: " << ticket1 << endl; // Expected output: Ticket 1: 1
    cout << "Ticket 2: " << ticket2 << endl; // Expected output: Ticket 2: 4
    cout << "Ticket 3: " << ticket3 << endl; // Expected output: Ticket 3: -1

    // Accept tickets
    valetSystem->acceptTicket(ticket1);
    valetSystem->acceptTicket(ticket2);
    ticket3 = valetSystem->issueTicket(suvCar);
    cout << "Ticket 3: " << ticket3 << endl; // Expected output: Ticket 3: 4

    valetSystem->acceptTicket(ticket3);
    return 0;
}

/*
Notes:
 Strategy Pattern used.

Application in the Code:

1. Context (ValetParkingSystem):
    Acts as a client that needs to perform parking based on different strategies depending on the type of car.

2. Strategies (ParkingStrategy, ElectricCarParkingStrategy, SUVCarParkingStrategy, SedanCarParkingStrategy):

    Each strategy (ParkingStrategy and its derivatives) encapsulates a parking algorithm (findSpot) tailored
    for different types of cars (Electric, SUV, Sedan).
    Strategies are interchangeable at runtime, allowing the ValetParkingSystem to select the appropriate
    parking strategy dynamically based on the car type.

3. Strategy Manager (StrategyManager):

    Provides a centralized place (getStrategy) to instantiate and provide strategies (ParkingStrategy)
    to the ValetParkingSystem.
    Encapsulates the creation of strategies, promoting the Single Responsibility Principle by separating
    strategy selection logic from the ValetParkingSystem.
*/
