#include <iostream>
#include <queue>
#include <vector>

// Forward declarations
class Call;
class Employee;

// Enum for employee levels
enum class Level {
    Respondent,
    Manager,
    Director
};

// Call class representing an incoming telephone call
class Call {
private:
    std::string callerName;
    // Other call information...

public:
    Call(const std::string& callerName)
        : callerName(callerName) {}

    std::string getCallerName() const {
        return callerName;
    }

    // Other methods...
};

// Employee class representing an employee in the call center
class Employee {
private:
    std::string name;
    Level level;
    bool available;

public:
    Employee(const std::string& name, Level level)
        : name(name), level(level), available(true) {}

    std::string getName() const {
        return name;
    }

    Level getLevel() const {
        return level;
    }

    bool isAvailable() const {
        return available;
    }

    void setAvailability(bool availability) {
        available = availability;
    }

    void handleCall(Call& call) {
        std::cout << "Employee " << name << " is handling the call from " << call.getCallerName() << std::endl;
        // Handle the call
    }
};

// CallCenter class representing the call center
class CallCenter {
private:
    std::queue<Call> incomingCalls;
    std::vector<Employee> respondents;
    std::vector<Employee> managers;
    std::vector<Employee> directors;

public:
    void dispatchCall() {
        if (incomingCalls.empty()) {
            std::cout << "No incoming calls at the moment." << std::endl;
            return;
        }

        Call call = incomingCalls.front();
        incomingCalls.pop();

        Employee* availableEmployee = findAvailableEmployee(Level::Respondent);
        if (availableEmployee != nullptr) {
            availableEmployee->handleCall(call);
            return;
        }

        availableEmployee = findAvailableEmployee(Level::Manager);
        if (availableEmployee != nullptr) {
            availableEmployee->handleCall(call);
            return;
        }

        availableEmployee = findAvailableEmployee(Level::Director);
        if (availableEmployee != nullptr) {
            availableEmployee->handleCall(call);
            return;
        }

        std::cout << "All employees are currently busy. Please wait." << std::endl;
    }

    void addIncomingCall(const Call& call) {
        incomingCalls.push(call);
        dispatchCall();
    }

    void addEmployee(const Employee& employee) {
        Level level = employee.getLevel();
        switch (level) {
            case Level::Respondent:
                respondents.push_back(employee);
                break;
            case Level::Manager:
                managers.push_back(employee);
                break;
            case Level::Director:
                directors.push_back(employee);
                break;
        }
    }

private:
    Employee* findAvailableEmployee(Level level) {
        std::vector<Employee>* employees;
        switch (level) {
            case Level::Respondent:
                employees = &respondents;
                break;
            case Level::Manager:
                employees = &managers;
                break;
            case Level::Director:
                employees = &directors;
                break;
        }

        for (Employee& employee : *employees) {
            if (employee.isAvailable()) {
                employee.setAvailability(false);
                return &employee;
            }
        }

        return nullptr;
    }
};

int main() {
    // Create employees
    Employee respondent1("John", Level::Respondent);
    Employee respondent2("Alice", Level::Respondent);
    Employee manager1("Mike", Level::Manager);
    Employee director1("Emily", Level::Director);

    // Create the call center
    CallCenter callCenter;
    callCenter.addEmployee(respondent1);
    callCenter.addEmployee(respondent2);
    callCenter.addEmployee(manager1);
    callCenter.addEmployee(director1);

    // Incoming calls
    Call call1("Caller 1");
    callCenter.addIncomingCall(call1);

    Call call2("Caller 2");
    callCenter.addIncomingCall(call2);

    // Simulating employee availability
    respondent1.setAvailability(false);
    callCenter.dispatchCall(); // Should assign the call to respondent2

    manager1.setAvailability(false);
    callCenter.dispatchCall(); // Should assign the call to director1

    // Cleanup

    return 0;
}