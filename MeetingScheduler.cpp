#include <bits/stdc++.h>

using namespace std;

class Meeting;
typedef unordered_map<int, vector<Meeting>> Calender;

class Room
{
    string name;
    Calender calender;

public:
    Room(string pName) : name(pName) {}
    string getName() const
    {
        return name;
    }
    bool book(int, int, int);
};

class Meeting
{
    int start, end;
    Room room;

public:
    Meeting(int pStart, int pEnd, Room pRoom) : start(pStart), end(pEnd), room(pRoom)
    {
    }

    int getStart() const
    {
        return start;
    }

    int getEnd() const
    {
        return end;
    }
};

// Acts as an interface to outside world. User can only call the public APIs of this class
// This class keeps a track of all the rooms in the form of a vector
class Scheduler
{
    vector<Room> rooms;

public:
    Scheduler(vector<Room> pRooms) : rooms(pRooms)
    {
    }

    bool addRoom(Room newRoom)
    {
        for (auto it = rooms.begin(); it != rooms.end(); ++it)
        {
            if (it->getName() == it->getName())
            {
                return false;
            }
        }

        rooms.push_back(newRoom);
        return true;
    }

    bool removeRoom(Room expireRoom)
    {
        for (auto it = rooms.begin(); it != rooms.end(); ++it)
        {
            if (it->getName() == expireRoom.getName())
            {
                rooms.erase(it);
                return true;
            }
        }
        return false;
    }
    string schedule(int day, int startTime, int endTime)
    {
        for (Room &room : rooms)
        {
            bool flag = room.book(day, startTime, endTime);
            if (flag == true)
            {
                return room.getName();
            }
        }
        return "No rooms available";
    }
};

/* Assuming
    1. End > Start
    2. Meeting can be scheduled in either one condition fulfills:
        a. End1 <= Start2
        b. End2 <= Start1
*/
bool Room ::book(int day, int start, int end)
{
    for (Meeting m : calender[day])
    {
        if (m.getStart() < end && start < m.getEnd())
        {
            return false;
        }
    }

    Meeting meeting(start, end, *this);
    calender[day].push_back(meeting);
    return true;
}

int main()
{
    Room room1("Atlas");
    Room room2("Nexus");
    Room room3("HolyCow");

    vector<Room> roomVec;
    roomVec.push_back(room1);
    roomVec.push_back(room2);
    roomVec.push_back(room3);

    Scheduler scheduler(roomVec);

    cout << scheduler.schedule(15, 2, 5) << "\n"; // Atlas
    cout << scheduler.schedule(15, 5, 8) << "\n"; // Atlas
    cout << scheduler.schedule(15, 4, 8) << "\n"; // Nexus
    cout << scheduler.schedule(15, 3, 6) << "\n"; // Holycow
    cout << scheduler.schedule(15, 7, 8) << "\n"; // Holycow
    cout << scheduler.schedule(15, 6, 9) << "\n"; // No rooms available
    cout << scheduler.schedule(16, 6, 9) << "\n"; // Atlas
    return 0;
}

/*
    Points
    1. Scheduler hold "to many" relation with "Room".
    2. Room hold "to many" relation with "Meeting".
    3. Meeting hold "to one"  relation with "Room".
*/