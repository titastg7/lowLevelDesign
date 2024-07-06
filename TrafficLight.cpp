/*##########################################################################
System Requirements
--------------------
1. Signal Timing Management: Control the timing of traffic lights.
2. Intersection Management: Manage multiple traffic signals at an intersection.
3. Mode Configuration: Support different traffic modes.
4. Emergency Override: Allow manual control in emergencies.
---------------------------------------------------------------------------------
Core Use Cases
--------------
1. Switching Signals: Change signals from red to green and vice versa.
2. Timing Adjustment: Modify the duration of each signal.
3. Mode Selection: Choose operational modes.
4. Emergency Control: Manual override for emergencies.

##########################################################################*/

#include <bits/stdc++.h>
using namespace std;

// Enumeration for traffic light states
enum class LightState
{
    RED,
    GREEN,
    YELLOW
};

// Class representing a traffic light
class TrafficLight
{
private:
    string id;
    LightState state;

public:
    // Constructor
    TrafficLight(const string &id) : id(id), state(LightState::RED) {}

    // Method to change the state of the traffic light
    void changeState(LightState newState)
    {
        state = newState;
    }

    // Getter for ID
    const string &getId() const
    {
        return id;
    }

    // Getter for state
    LightState getState() const
    {
        return state;
    }
};

// Class representing an intersection controller
class IntersectionController
{
private:
    unordered_map<string, TrafficLight *> trafficLights;

public:
    // Constructor
    IntersectionController() {}

    // Method to add a traffic light to the intersection controller
    void addTrafficLight(TrafficLight *light)
    {
        trafficLights[light->getId()] = light;
    }

    // Method to change the signal of a traffic light
    void changeSignal(const string &lightId, LightState state)
    {
        if (trafficLights.find(lightId) != trafficLights.end())
        {
            trafficLights[lightId]->changeState(state);
        }
    }

    // Additional methods...
};

// Class representing a control panel
class ControlPanel
{
private:
    IntersectionController *controller;

public:
    // Constructor
    ControlPanel(IntersectionController *controller) : controller(controller) {}

    // Method to override the signal of a traffic light
    void overrideSignal(const string &lightId, LightState state)
    {
        controller->changeSignal(lightId, state);
    }

    // Additional methods...
};

// Class representing the traffic signal system
class TrafficSignalSystem
{
private:
    vector<IntersectionController *> intersectionControllers;

public:
    // Constructor
    TrafficSignalSystem() {}

    // Method to add an intersection controller to the system
    void addIntersectionController(IntersectionController *controller)
    {
        intersectionControllers.push_back(controller);
    }

    // Methods to update system configurations...
};

int main()
{
    // Create traffic light objects
    TrafficLight light1("TL1");
    TrafficLight light2("TL2");

    // Create an intersection controller
    IntersectionController controller;
    controller.addTrafficLight(&light1);
    controller.addTrafficLight(&light2);

    // Create a control panel
    ControlPanel panel(&controller);

    // Override signal for a traffic light
    panel.overrideSignal("TL1", LightState::GREEN);

    return 0;
}

/*
1. Observer Pattern:
   In the provided code, the IntersectionController acts as a subject, and the TrafficLight objects
   act as observers. The ControlPanel can override the signal of a traffic light, which triggers changes
   in the state of the TrafficLight objects.

2. Singleton Pattern (potentially):
   Although not explicitly implemented, the TrafficSignalSystem class resembles a singleton pattern in
   the way it manages intersection controllers. It ensures that only one instance of the traffic signal
   system exists throughout the application, thus managing system-wide traffic control.

These are the primary design patterns observed in the code. The Observer Pattern is evident in the
interaction between traffic lights and the intersection controller, while the Singleton Pattern is
potentially implied by the TrafficSignalSystem class's behavior.
*/