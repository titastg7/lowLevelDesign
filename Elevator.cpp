#include <bits/stdc++.h>

using namespace std;

// Forward declarations
class Elevator;

// Request class
class Request
{
public:
    long requestId;
    std::vector<int> desiredFloors;

    Request(long requestId) : requestId(requestId) {}

    void addDesiredFloor(int desiredFloor)
    {
        desiredFloors.push_back(desiredFloor);
    }

    bool operator<(const Request &other) const
    {
        return requestId > other.requestId; // Priority Queue uses max-heap by default
    }
};

// ElevatorDoor class
class ElevatorDoor
{
private:
    int elevatorId;

public:
    ElevatorDoor(int elevatorId) : elevatorId(elevatorId) {}

    void open(int currentFloor)
    {
        std::cout << "Elevator with id: " << elevatorId << " 's door opened at floor: " << currentFloor << std::endl;
    }

    void close(int currentFloor)
    {
        std::cout << "Elevator with id: " << elevatorId << " 's door closed at floor: " << currentFloor << std::endl;
    }
};

// ElevatorButtonsPanel class
class ElevatorButtonsPanel
{
private:
    Elevator *elevator;
    Request request;

public:
    ElevatorButtonsPanel(Elevator *elevator)
        : elevator(elevator), request(std::chrono::system_clock::now().time_since_epoch().count()) {}

    void click(int desiredFloor);
};

// Elevator class
class Elevator
{
private:
    int elevatorId;
    int currentFloor;
    std::priority_queue<Request> queue;

    ElevatorDoor elevatorDoor;
    ElevatorButtonsPanel elevatorButtonsPanel;

    void moveUp()
    {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        currentFloor++;
        std::cout << "Elevator with id: " << elevatorId << " moving up to floor: " << currentFloor << std::endl;
    }

    void moveDown()
    {
        std::this_thread::sleep_for(std::chrono::seconds(2));
        currentFloor--;
        std::cout << "Elevator with id: " << elevatorId << " moving down to floor: " << currentFloor << std::endl;
    }

    void moveToDesiredFloors(std::vector<int> &desiredFloors)
    {
        while (!desiredFloors.empty())
        {
            int desiredFloor = desiredFloors.front();
            if (currentFloor == desiredFloor)
            {
                std::cout << "Elevator with id: " << elevatorId << " reached floor: " << currentFloor << std::endl;
                elevatorDoor.open(currentFloor);
                std::this_thread::sleep_for(std::chrono::seconds(2));
                elevatorDoor.close(currentFloor);
                desiredFloors.erase(desiredFloors.begin());
            }
            else if (currentFloor > desiredFloor)
            {
                moveDown();
            }
            else
            {
                moveUp();
            }
        }
    }

public:
    Elevator(int elevatorId, int currentFloor) : elevatorId(elevatorId), currentFloor(currentFloor),
                                                 elevatorDoor(elevatorId), elevatorButtonsPanel(this) {}

    int getElevatorId() const
    {
        return elevatorId;
    }

    int getCurrentFloor() const
    {
        return currentFloor;
    }

    void serveRequest(Request request)
    {
        queue.push(request);
    }

    void run()
    {
        while (true)
        {
            if (!queue.empty())
            {
                Request currentRequest = queue.top();
                queue.pop();
                moveToDesiredFloors(currentRequest.desiredFloors);
            }
            else
                std::this_thread::sleep_for(std::chrono::milliseconds(100)); // To avoid busy-waiting
        }
    }

    ElevatorButtonsPanel &getElevatorButtonPanel()
    {
        return elevatorButtonsPanel;
    }
};

// ElevatorButtonsPanel methods
void ElevatorButtonsPanel::click(int desiredFloor)
{
    request.addDesiredFloor(desiredFloor);
    elevator->serveRequest(request);
}

// ElevatorPickingStrategy interface
class ElevatorPickingStrategy
{
public:
    virtual Elevator *pickElevator(std::vector<Elevator *> &availableElevators, int desiredFloor) = 0;
};

// ShortestTimeFirstStrategy class
class ShortestTimeFirstStrategy : public ElevatorPickingStrategy
{
public:
    Elevator *pickElevator(std::vector<Elevator *> &availableElevators, int desiredFloor) override
    {
        return getShortestTimedElevator(availableElevators, desiredFloor);
    }

private:
    Elevator *getShortestTimedElevator(std::vector<Elevator *> &availableElevators, int desiredFloor)
    {
        Elevator *shortestTimedElevator = nullptr;
        int shortestTime = INT_MAX;

        for (Elevator *elevator : availableElevators)
        {
            int time = getTimeToReachFloor(elevator->getCurrentFloor(), desiredFloor);
            if (time < shortestTime)
            {
                shortestTime = time;
                shortestTimedElevator = elevator;
            }
        }

        std::cout << "Elevator with id " << shortestTimedElevator->getElevatorId() << " was selected to serve the request" << std::endl;
        return shortestTimedElevator;
    }

    int getTimeToReachFloor(int currentFloor, int desiredFloor)
    {
        return std::abs(desiredFloor - currentFloor) * 2; // in seconds
    }
};

// ElevatorSystemController class
class ElevatorSystemController
{
private:
    static std::vector<Elevator *> elevators;
    static ElevatorPickingStrategy *elevatorPickingStrategy;

public:
    static Elevator *requestForElevator(int floorNumber)
    {
        Elevator *elevator = elevatorPickingStrategy->pickElevator(elevators, floorNumber);
        Request request(std::chrono::system_clock::now().time_since_epoch().count());
        request.addDesiredFloor(floorNumber);
        elevator->serveRequest(request);
        return elevator;
    }

    static void setElevatorPickingStrategy(ElevatorPickingStrategy *strategy)
    {
        elevatorPickingStrategy = strategy;
    }

    static void addElevator(Elevator *elevator)
    {
        std::thread(&Elevator::run, elevator).detach();
        elevators.push_back(elevator);
    }
};

// Initialize static members
std::vector<Elevator *> ElevatorSystemController::elevators;
ElevatorPickingStrategy *ElevatorSystemController::elevatorPickingStrategy = new ShortestTimeFirstStrategy();

// FloorButton class
class FloorButton
{
private:
    int floorNumber;

public:
    FloorButton(int floorNumber) : floorNumber(floorNumber) {}

    Elevator *click()
    {
        return ElevatorSystemController::requestForElevator(floorNumber);
    }
};

// Main function
int main()
{
    ElevatorSystemController::addElevator(new Elevator(1, 5));
    ElevatorSystemController::addElevator(new Elevator(2, 2));

    FloorButton firstFloorButton(1);
    Elevator *elevator = firstFloorButton.click();
    elevator->getElevatorButtonPanel().click(5);
    elevator->getElevatorButtonPanel().click(3);

    std::this_thread::sleep_for(std::chrono::minutes(1)); // Keep the main thread alive to observe the output
    return 0;
}
