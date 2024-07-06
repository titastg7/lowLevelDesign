/*##########################################################################
System Requirements
--------------------
1. Task Creation and Management: Enable users to create, update, and delete tasks.
2. User Management: Manage user accounts and associated tasks.
3. Task Assignment: Allow tasks to be assigned to specific users.
4. Task Tracking: Track the progress and status of tasks.
5. Notifications: Notify users about task deadlines and updates.
---------------------------------------------------------------------------------
Core Use Cases
--------------
1. Managing User Accounts
2. Creating and Updating Tasks
3. Assigning Tasks to Users
4. Tracking Task Progress
5. Sending Notifications
##########################################################################*/

#include <bits/stdc++.h>
using namespace std;

enum class TaskStatus
{
    NEW,
    IN_PROGRESS,
    COMPLETED,
    CANCELLED
};

class Task;

class User
{
private:
    string userId;
    string name;
    vector<Task *> assignedTasks;

public:
    User(const string &userId, const string &name)
        : userId(userId), name(name) {}

    void addTask(Task *task)
    {
        assignedTasks.push_back(task);
    }

    const string &getUserId() const
    {
        return userId;
    }

    // Getters and setters...
};

class Task
{
private:
    string taskId;
    string title;
    string description;
    tm dueDate;
    TaskStatus status;

public:
    Task(const string &taskId, const string &title, const tm &dueDate)
        : taskId(taskId), title(title), dueDate(dueDate), status(TaskStatus::NEW) {}

    void updateStatus(TaskStatus newStatus)
    {
        status = newStatus;
    }

    const string &getTaskId() const
    {
        return taskId;
    }

    // Getters and setters...
};

class TaskManagementSystem
{
private:
    vector<User *> users;
    vector<Task *> tasks;

public:
    TaskManagementSystem() {}

    void addUser(User *user)
    {
        users.push_back(user);
    }
    void addTask(Task *task)
    {
        tasks.push_back(task);
    }
    void assignTaskToUser(const string &taskId, const string &userId)
    {
        User *user = findUserById(userId);
        Task *task = findTaskById(taskId);

        if (user != nullptr && task != nullptr)
        {
            user->addTask(task);
        }
    }

    User *findUserById(const string &userId)
    {
        for (User *user : users)
        {
            if (user->getUserId() == userId)
            {
                return user;
            }
        }
        return nullptr;
    }

    Task *findTaskById(const string &taskId)
    {
        for (Task *task : tasks)
        {
            if (task->getTaskId() == taskId)
            {
                return task;
            }
        }
        return nullptr;
    }

    // Other necessary methods...
};

int main()
{
    TaskManagementSystem taskSystem;

    User user1("1", "John Doe");
    User user2("2", "Jane Smith");
    Task task1("101", "Task 1", tm{0, 0, 0, 1, 0, 121, 0, 0, -1});
    Task task2("102", "Task 2", tm{0, 0, 0, 1, 0, 121, 0, 0, -1});

    taskSystem.addUser(&user1);
    taskSystem.addUser(&user2);
    taskSystem.addTask(&task1);
    taskSystem.addTask(&task2);

    taskSystem.assignTaskToUser("101", "1");
    taskSystem.assignTaskToUser("102", "2");

    return 0;
}

/*
The code implements the Composite pattern.

Here's how the pattern applies to the code:

Component: The User and Task classes act as the components. They represent the individual
  elements of the composite structure.
Composite: The TaskManagementSystem class acts as the composite. It manages the components
  (User and Task) and provides methods to add, remove, and manipulate them.
Client: The main function acts as the client. It interacts with the composite structure by
  adding users, tasks, and assigning tasks to users.

In summary, the Composite pattern is used to treat individual objects and compositions of
objects uniformly, allowing clients to operate on them in a consistent manner.
*/