#include <bits/stdc++.h>
using namespace std;

enum class VehicleSize
{
    TwoWheeler,
    FourWheeler
};

class Vehicle
{
    string vehicleNo;
    VehicleSize vehicleSize;

public:
    Vehicle(VehicleSize pVehicleSize, string pVehicleNo)
        : vehicleSize(pVehicleSize), vehicleNo(pVehicleNo)
    {
    }
    VehicleSize getVehicleSize()
    {
        return vehicleSize;
    }
    string getVehicleNo()
    {
        return vehicleNo;
    }

    void setVehicleSize(VehicleSize pVehicleSize)
    {
        vehicleSize = pVehicleSize;
    }
    void setVehicleNo(string pVehicleNo)
    {
        vehicleNo = pVehicleNo;
    }
};

class Slot
{
    int slotNo;
    Vehicle *parkVehicle;
    bool isEmpty;

public:
    Slot(int pSlotNo) : slotNo(pSlotNo), isEmpty(true), parkVehicle(nullptr)
    {
    }

    bool isEmptySlot()
    {
        return isEmpty;
    }
    void setEmpty(bool pIsEmpty)
    {
        isEmpty = pIsEmpty;
    }
    int getSlotNo()
    {
        return slotNo;
    }
    void setSlotNo(int pSlotNo)
    {
        slotNo = pSlotNo;
    }
    Vehicle *getParkVehicle()
    {
        return parkVehicle;
    }
    void setParkVehicle(Vehicle *pVehicle)
    {
        parkVehicle = pVehicle;
    }
    void occupySlot(Vehicle *vehicle)
    {
        parkVehicle = vehicle;
        isEmpty = false;
    }
    void vacateSlot()
    {
        parkVehicle = nullptr;
        isEmpty = true;
    }
};

class Ticket
{
    string ticketId;
    int slotNo;
    Vehicle *vehicle;
    static int nextTicketId = 1;
    tm date;

public:
    Ticket(int pSlotNo, Vehicle *pVehicle, tm pDate) : slotNo(pSlotNo), vehicle(pVehicle), date(pDate)
    {
        ticketId = "T" + to_string(nextTicketId);
        nextTicketId++;
    }

    ~Ticket()
    {
        delete vehicle;
    }

    int getSlotNo()
    {
        return slotNo;
    }

    Vehicle *getVehicle()
    {
        return vehicle;
    }

    tm getDate()
    {
        return date;
    }

    void printTicket()
    {
        cout << " Ticket : " << ticketId << " SlotNo: " << slotNo << " Vehicle: " << vehicle->getVehicleNo() << " Date: " << date << endl;
    }
};

class ParkingChargeStrategy
{
public:
    virtual int getCharge(int hours) = 0;
};

class TwoWheelerWeekend : public ParkingChargeStrategy
{
public:
    int getCharge(int hours)
    {
        return hours * 20;
    }
};

class FourWheelerWeekDay : public ParkingChargeStrategy
{
public:
    int getCharge(int hours)
    {
        return hours * 30;
    }
};

class FourWheelerWeekend : public ParkingChargeStrategy
{
public:
    int getCharge(int hours)
    {
        return hours * 40;
    }
};

class TwoWheelerWeekDay : public ParkingChargeStrategy
{
public:
    int getCharge(int hours)
    {
        return hours * 15;
    }
};

class Parking
{
public:
    virtual Ticket *park(Vehicle *vehicle) = 0;
    virtual int unpark(Ticket *ticket, ParkingChargeStrategy *parkingStrategy) = 0;
    virtual ~Parking() {}
};

class ParkingLot : public Parking
{
    static ParkingLot *parkingLot = nullptr;
    static mutex mtx;
    vector<Slot *> twoWheelerSlots;
    vector<Slot *> fourWheelerSlots;
    ParkingLot() {}

    Slot *getNextTwoWheelerSlot()
    {
        for (auto slot : twoWheelerSlots)
        {
            if (slot->isEmptySlot())
                return slot;
        }
        return nullptr;
    }

    Slot *getNextFourWheelerSlot()
    {
        for (auto slot : fourWheelerSlots)
        {
            if (slot->isEmptySlot())
                return slot;
        }
        return nullptr;
    }

    int getHourParked(tm startDate, tm endDate)
    {
        int diff = difftime(mktime(&endDate), mktime(&startDate));
        return diff / 3600; // Convert seconds to hours
    }

    Slot *getFourWheelerSlotByVehicleNo(string vehicleNo)
    {
        for (auto slot : fourWheelerSlots)
        {
            if (!slot->isEmptySlot() && slot->getParkVehicle()->getVehicleNo() == vehicleNo)
                return slot;
        }
        return nullptr;
    }

    Slot *getTwoWheelerSlotByVehicleNo(string vehicleNo)
    {
        for (auto slot : twoWheelerSlots)
        {
            if (!slot->isEmptySlot() && slot->getParkVehicle()->getVehicleNo() == vehicleNo)
                return slot;
        }
        return nullptr;
    }
    tm getCurrentDate()
    {
        time_t now = time(0);
        tm *ltm = localtime(&now);
        return *ltm;
    }

public:
    static ParkingLot *getParkingLot()
    {
        if (parkingLot == nullptr)
        {
            mtx.lock();
            if (parkingLot == nullptr)
            {
                parkingLot = new ParkingLot();
            }
            mtx.unlock();
        }
        return parkingLot;
    }

    void initializeParkingSlots(int numOfTwoWheelerSlots, int numOfFourWheelerSlots)
    {
        for (int i = 1; i <= numOfTwoWheelerSlots; i++)
            twoWheelerSlots.push_back(new Slot(i));
        for (int i = 1; i <= numOfFourWheelerSlots; i++)
            fourWheelerSlots.push_back(new Slot(i));
    }

    Ticket *park(Vehicle *vehicle)
    {
        Ticket *ticket = nullptr;
        Slot *availableSlot = nullptr;
        if (vehicle->getVehicleSize() == VehicleSize ::FourWheeler)
            availableSlot = getNextFourWheelerSlot();
        else
            availableSlot = getNextTwoWheelerSlot();
        if (availableSlot == nullptr)
            return ticket;
        availableSlot->occupySlot(vehicle);
        ticket = new Ticket(availableSlot->getSlotNo(), vehicle, , getCurrentDate());
        return ticket;
    }

    int unpark(Ticket *ticket, ParkingChargeStrategy *parkingStrategy)
    {
        int costByHours = 0;
        Slot *slot = nullptr;
        Vehicle *vehicle = ticket->getVehicle();
        if (vehicle == nullptr)
            return 0;
        if (vehicle->getVehicleSize() == VehicleSize ::FourWheeler)
            slot = getFourWheelerSlotByVehicleNo(vehicle->getVehicleNo());
        else
            slot = getTwoWheelerSlotByVehicleNo(vehicle->getVehicleNo());
        slot->vacateSlot();
        int hours = getHourParked(ticket->getDate(), getCurrentDate());
        costByHours = parkingStrategy->getCharge(hours);
        return costByHours;
    }
    ~ParkingLot()
    {
        for (auto slot : twoWheelerSlots)
            delete slot;
        for (auto slot : fourWheelerSlots)
            delete slot;
    }
};

int main()
{
    ParkingLot *parkingLot = ParkingLot::getParkingLot();
    parkingLot->initializeParkingSlots(20, 10);

    Vehicle *car = new Vehicle(VehicleSize::FourWheeler, "BMW1");
    Vehicle *bike = new Vehicle(VehicleSize::TwoWheeler, "Platina1");

    Ticket *t = parkingLot->park(car);
    if (t != nullptr)
    {
        t->printTicket();
        int cost = parkingLot->unpark(t, new TwoWheelerWeekend());
        cout << " Cost = " << cost << endl;
    }
    else
        cout << "No available slot." << endl;

    delete car;
    delete bike;
    delete t;

    return 0;
}