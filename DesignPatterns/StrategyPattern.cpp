#include <bits/stdc++.h>

using namespace std;

// Interface
class DriveStrategy
{
public:
    virtual void drive() = 0;
};

class NormalStrategy : public DriveStrategy
{
public:
    void drive() override
    {
        cout << "Normal Strategy Capability\n";
    }
};

class SportsDriveStrategy : public DriveStrategy
{
public:
    void drive() override
    {
        cout << "Sports Strategy Capability\n";
    }
};

class Vehicle
{
private:
    DriveStrategy *driveObj;

public:
    Vehicle(DriveStrategy *driveObj)
    {
        this->driveObj = driveObj;
    }
    void drive()
    {
        driveObj->drive();
    }
};

class SportVehicle : public Vehicle
{
public:
    SportVehicle() : Vehicle(new SportsDriveStrategy())
    {
    }
};

class OffRoadVehicle : public Vehicle
{
public:
    OffRoadVehicle() : Vehicle(new SportsDriveStrategy())
    {
    }
};

class PassengerVehicle : public Vehicle
{
public:
    PassengerVehicle() : Vehicle(new NormalStrategy())
    {
    }
};

int main()
{
    Vehicle *v1 = new SportVehicle();
    cout << "Sports Vehicle: ";
    v1->drive();

    Vehicle *v2 = new PassengerVehicle();
    cout << "Passenger Vehicle: ";
    v2->drive();

    return 0;
}