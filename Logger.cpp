/*##########################################################################
System Requirements
--------------------
1. Support Multiple Log Levels: Including INFO, DEBUG, WARN, and ERROR.
2. Flexible Log Destination: Enable logging to various outputs like the console, files, or external services.
3. Configurable Formatting: Allow for custom log message formats.
4. Performance Efficiency: Ensure minimal impact on application performance.
---------------------------------------------------------------------------------
Core Use Cases
--------------
1. Logging Messages: Ability to log messages at different levels.
2. Configuring Loggers: Setup loggers with varying settings and outputs.
3. Managing Log Output: Direct messages to appropriate destinations based on configurations.
##########################################################################*/

#include <bits/stdc++.h>
using namespace std;

// Enum for log levels
enum class LogLevel
{
    INFO,
    WARNING,
    ERROR,
    DEBUG
};

// Abstract Log Strategy
class LogStrategy
{
public:
    virtual ~LogStrategy() = default;
    virtual void log(string &message) = 0;
};

// Console Log Strategy
class ConsoleLogStrategy : public LogStrategy
{
public:
    void log(string &message) override
    {
        cout << message << endl;
    }
};

// File Log Strategy
class FileLogStrategy : public LogStrategy
{
    ofstream file;

public:
    FileLogStrategy(string &filename)
    {
        file.open(filename, ios::app);
    }
    ~FileLogStrategy()
    {
        if (file.is_open())
        {
            file.close();
        }
    }
    void log(string &message) override
    {
        if (file.is_open())
        {
            file << message << endl;
        }
    }
};

// Logger Singleton
class Logger
{
private:
    LogStrategy *strategy;
    static Logger *instance;

    Logger() : strategy(nullptr) {}

    string getCurrentTimestamp() const
    {
        std::time_t now = std::time(nullptr);
        char buf[20];
        std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
        return std::string(buf);
    }

    string getLogLevelString(LogLevel level) const
    {
        switch (level)
        {
        case LogLevel::INFO:
            return "INFO";
        case LogLevel::WARNING:
            return "WARNING";
        case LogLevel::ERROR:
            return "ERROR";
        case LogLevel::DEBUG:
            return "DEBUG";
        default:
            return "UNKNOWN";
        }
    }

public:
    static Logger *getInstance()
    {
        if (!instance)
        {
            instance = new Logger();
        }
        return instance;
    }

    void setStrategy(LogStrategy *newStrategy)
    {
        strategy = newStrategy;
    }

    void log(string &message, LogLevel level)
    {
        if (strategy)
        {
            string logMessage = "[" + getCurrentTimestamp() + "] [" + getLogLevelString(level) + "] " + message;
            strategy->log(logMessage);
        }
    }
};

Logger *Logger::instance = nullptr;

int main()
{
    // Singleton Logger with Strategy
    Logger *logger = Logger::getInstance();
    ConsoleLogStrategy consoleStrategy;
    FileLogStrategy fileStrategy("log.txt");

    logger->setStrategy(&consoleStrategy);
    logger->log("Logging to console", LogLevel::INFO);

    logger->setStrategy(&fileStrategy);
    logger->log("Logging to file", LogLevel::ERROR);

    return 0;
}

/*
Adherence to SOLID Principles
-------------------------------
Single Responsibility Principle: Each class has a single responsibility: LogStrategy handles logging, Logger manages the logging mechanism,
and specific strategies handle where to log.

Open/Closed Principle:   The system is open for extension (new logging strategies or log levels) but closed for modification.

Liskov Substitution Principle: Derived classes (ConsoleLogStrategy, FileLogStrategy) can be substituted for their base class (LogStrategy)
without affecting the program's correctness.

Interface Segregation Principle: The LogStrategy interface is focused and specific, ensuring that classes implementing it are not
forced to define methods they don't use.

Dependency Inversion Principle:  High-level modules depend on abstractions (LogStrategy) rather than concrete implementations,
making the system flexible and extensible.

Design patterns are used:
------------------------
Singleton Pattern: The Logger class is implemented as a singleton to ensure that only one instance of the logger
exists throughout the application.

Strategy Pattern: The logging mechanism uses the strategy pattern to allow the logging behavior to be defined at runtime.
This is done through the LogStrategy interface and its concrete implementations (ConsoleLogStrategy and FileLogStrategy).
The Logger class can switch between different logging strategies (e.g., console or file logging) dynamically using the setStrategy method.
*/