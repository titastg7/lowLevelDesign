#include <bits/stdc++.h>
using namespace std;

// Forward declarations
class User;
class Message;
class ChatServer;
class UserManager;
class MessageRouter;
class PersistenceManager;

// User class representing a user of the chat server
class User
{
private:
    std::string userID;
    std::string username;
    std::string password;
    std::vector<std::string> contactList;
    bool onlineStatus;

public:
    User(const std::string &userID, const std::string &username, const std::string &password)
        : userID(userID), username(username), password(password), onlineStatus(false) {}

    std::string getUserID() const
    {
        return userID;
    }

    std::string getUsername() const
    {
        return username;
    }

    bool authenticate(const std::string &password) const
    {
        return this->password == password;
    }

    void addContact(const std::string &contact)
    {
        contactList.push_back(contact);
    }

    void removeContact(const std::string &contact)
    {
        auto it = std::find(contactList.begin(), contactList.end(), contact);
        if (it != contactList.end())
        {
            contactList.erase(it);
        }
    }

    std::vector<std::string> getContactList() const
    {
        return contactList;
    }

    void setOnlineStatus(bool status)
    {
        onlineStatus = status;
    }

    bool isOnline() const
    {
        return onlineStatus;
    }

    void sendMessage(const std::string &receiverID, const std::string &content);
    void receiveMessage(const Message &message);
};

// Message class representing a message sent between users
class Message
{
private:
    std::string senderID;
    std::string receiverID;
    std::string content;
    std::chrono::system_clock::time_point timestamp;

public:
    Message(const std::string &senderID, const std::string &receiverID, const std::string &content)
        : senderID(senderID), receiverID(receiverID), content(content),
          timestamp(std::chrono::system_clock::now()) {}

    std::string getSenderID() const
    {
        return senderID;
    }

    std::string getReceiverID() const
    {
        return receiverID;
    }

    std::string getContent() const
    {
        return content;
    }

    std::string getTimestamp() const
    {
        auto time = std::chrono::system_clock::to_time_t(timestamp);
        return std::ctime(&time);
    }
};

// ChatServer class representing the central chat server component
class ChatServer
{
private:
    std::unordered_map<std::string, User *> users;
    UserManager *userManager;
    MessageRouter *messageRouter;
    PersistenceManager *persistenceManager;

public:
    ChatServer()
    {
        userManager = new UserManager();
        messageRouter = new MessageRouter();
        persistenceManager = new PersistenceManager();
    }

    ~ChatServer()
    {
        delete userManager;
        delete messageRouter;
        delete persistenceManager;
    }

    void registerUser(const std::string &userID, const std::string &username, const std::string &password)
    {
        userManager->registerUser(userID, username, password);
    }

    User *loginUser(const std::string &userID, const std::string &password)
    {
        return userManager->loginUser(userID, password);
    }

    void logoutUser(User *user)
    {
        userManager->logoutUser(user);
    }

    void sendMessage(User *sender, const std::string &receiverID, const std::string &content)
    {
        messageRouter->routeMessage(sender, receiverID, content);
    }

    std::vector<User *> getOnlineUsers() const
    {
        return userManager->getOnlineUsers();
    }
};

// UserManager class for managing user-related operations
class UserManager
{
private:
    std::unordered_map<std::string, User *> users;
    std::unordered_map<std::string, User *> onlineUsers;

public:
    void registerUser(const std::string &userID, const std::string &username, const std::string &password)
    {
        User *user = new User(userID, username, password);
        users[userID] = user;
        // Save user data using PersistenceManager
    }

    User *loginUser(const std::string &userID, const std::string &password)
    {
        auto it = users.find(userID);
        if (it != users.end())
        {
            User *user = it->second;
            if (user->authenticate(password))
            {
                user->setOnlineStatus(true);
                onlineUsers[userID] = user;
                return user;
            }
        }
        return nullptr;
    }

    void logoutUser(User *user)
    {
        user->setOnlineStatus(false);
        onlineUsers.erase(user->getUserID());
    }

    std::vector<User *> getOnlineUsers() const
    {
        std::vector<User *> onlineUserList;
        for (const auto &pair : onlineUsers)
        {
            onlineUserList.push_back(pair.second);
        }
        return onlineUserList;
    }
};

// MessageRouter class for routing messages between users
class MessageRouter
{
public:
    void routeMessage(User *sender, const std::string &receiverID, const std::string &content)
    {
        User *receiver = userManager->getUserByID(receiverID);
        if (receiver)
        {
            Message message(sender->getUserID(), receiverID, content);
            receiver->receiveMessage(message);
            // Save message using PersistenceManager
        }
    }
};

// PersistenceManager class for managing data persistence
class PersistenceManager
{
public:
    void saveUser(User *user)
    {
        // Save user data to a persistent storage
    }

    User *loadUser(const std::string &userID)
    {
        // Load user data from a persistent storage and create a User object
        return nullptr;
    }

    void saveMessage(const Message &message)
    {
        // Save message data to a persistent storage
    }

    Message loadMessage(const std::string &messageID)
    {
        // Load message data from a persistent storage and create a Message object
        return Message("", "", "");
    }
};

// Implementation of User class methods
void User::sendMessage(const std::string &receiverID, const std::string &content)
{
    ChatServer::getInstance()->sendMessage(this, receiverID, content);
}

void User::receiveMessage(const Message &message)
{
    std::cout << "Received message from " << message.getSenderID()
              << " at " << message.getTimestamp()
              << ": " << message.getContent() << std::endl;
}

int main()
{
    ChatServer chatServer;

    // Register users
    chatServer.registerUser("1", "User1", "password1");
    chatServer.registerUser("2", "User2", "password2");

    // Login users
    User *user1 = chatServer.loginUser("1", "password1");
    User *user2 = chatServer.loginUser("2", "password2");

    if (user1 && user2)
    {
        std::cout << "Users logged in successfully." << std::endl;

        // Send messages
        user1->sendMessage("2", "Hello, User2!");
        user2->sendMessage("1", "Hi, User1!");

        // Get online users
        std::vector<User *> onlineUsers = chatServer.getOnlineUsers();
        std::cout << "Online users: ";
        for (const auto &user : onlineUsers)
        {
            std::cout << user->getUsername() << " ";
        }
        std::cout << std::endl;

        // Logout users
        chatServer.logoutUser(user1);
        chatServer.logoutUser(user2);
        std::cout << "Users logged out." << std::endl;
    }

    return 0;
}
