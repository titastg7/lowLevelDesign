// Online C++ compiler to run C++ program online
#include <bits/stdc++.h>
using namespace std;

class Singleton
{
private:
    // static pointer which points
    // to the instance of this class.
    static Singleton *instancePtr;
    static mutex instanceMutex; // Mutex for thread safety

    // Default constructor
    Singleton() {}
    // copy constructor
    Singleton(const Singleton &);
    // operator overloading
    Singleton operator=(const Singleton &);

public:
    static Singleton *getInstance()
    {
        // Double-checked locking
        if (instancePtr == nullptr)
        {
            instanceMutex.lock(); // Lock the mutex
            if (instancePtr == nullptr)
            {
                instancePtr = new Singleton();
            }
            instanceMutex.inlock();
        }
        return instancePtr;
    }
};

// initializing instancePtr with NULL
Singleton *Singleton::instancePtr = nullptr;
mutex Singleton::instanceMutex;

// Driver code
int main()
{
    // Same instance is returned after the first getInstance()
    Singleton *singletonExample = Singleton::getInstance();
    cout << singletonExample << endl;
    Singleton *singletonExample2 = Singleton::getInstance();
    cout << singletonExample2 << endl;

    // Cannot create object of Singleton class
    // as default constructor is private &
    // no method is used to access it.
    // Singleton *error = new Singleton();
    return 0;
}