#include <bits/stdc++.h>
#include <cassert>

using namespace std;

// Define the Module class, which is a base class for different types of modules
class Module
{
protected:
    string name;

public:
    virtual string getModuleName() = 0;
};

// Define the Course class, which is a base class for different types of courses
class Course
{
protected:
    std::vector<Module *> modules; // A vector to store modules

public:
    virtual void createCourse() = 0;
    Course() {}

    std::vector<Module *> getModules()
    {
        return modules;
    }
};

class IntroModule : public Module
{
public:
    IntroModule()
    {
        name = "IntroModule101";
    }

    string getModuleName() override
    {
        return name;
    }
};

class SummaryModule : public Module
{
public:
    SummaryModule()
    {
        name = "SummaryModule102";
    }
    string getModuleName() override
    {
        return name;
    }
};

class ConceptModule : public Module
{
public:
    ConceptModule()
    {
        name = "ConceptModule103";
    }
    string getModuleName() override
    {
        return name;
    }
};

class DemoModule : public Module
{
public:
    DemoModule()
    {
        name = "DemoModule104";
    }
    string getModuleName() override
    {
        return name;
    }
};

class ExerciseModule : public Module
{
public:
    ExerciseModule()
    {
        name = "ExerciseModule105";
    }
    string getModuleName() override
    {
        return name;
    }
};

// Define a specific concrete Course class (LLD) that inherits from Course
class LLD : public Course
{
public:
    LLD()
    {
        createCourse();
    }
    void createCourse() override
    {
        modules.push_back(new IntroModule());    // Create IntroModule and add it to the module vector
        modules.push_back(new ExerciseModule()); // Create ExerciseModule and add it
        modules.push_back(new SummaryModule());  // Create SummaryModule and add it
    }
};

// Define a specific concrete Course class (HLD) that inherits from Course

class HLD : public Course
{
public:
    HLD()
    {
        createCourse();
    }
    void createCourse() override
    {
        modules.push_back(new ConceptModule());
        modules.push_back(new DemoModule());
        modules.push_back(new SummaryModule());
    }
};

// Define a factory class (CourseFactory) to create instances of courses
class CourseFactory
{
public:
    static Course *getCourse(std::string courseType)
    {
        if (courseType == "LLD")
        {
            return new LLD(); // Create and return an LLD course
        }
        else if (courseType == "HLD")
        {
            return new HLD(); // Create and return an HLD course (if it exists)
        }
        return nullptr; // Return nullptr for unknown course types
    }
};

// Client
int main()
{
    Course *hldCourse = CourseFactory::getCourse("HLD"); // Create an HLD course using the factory
    Course *lldCourse = CourseFactory::getCourse("LLD"); // Create an LLD course using the factory

    assert(hldCourse != nullptr);
    std::cout << "HLD Modules: " << std::endl;
    for (Module *module : hldCourse->getModules())
    {
        std::cout << module->getModuleName() << std::endl; // Output the modules of the HLD course
    }

    cout << endl;
    assert(lldCourse != nullptr);
    std::cout << "LLD Modules: " << std::endl;
    for (Module *module : lldCourse->getModules())
    {
        std::cout << module->getModuleName() << std::endl; // Output the modules of the LLD course
    }

    // Clean up allocated memory
    delete hldCourse;
    delete lldCourse;

    return 0;
}
