/*##########################################################################
System Requirements
--------------------
1. Student Management: Handle student profiles and academic records.
2. Course Management: Manage course details, schedules, and capacities.
3. Registration Process: Enable students to register for courses.
4. Prerequisite Checking: Ensure students meet course prerequisites.
5. Enrollment Verification: Confirm student enrollments in courses.
---------------------------------------------------------------------------------
Core Use Cases
--------------
1. Registering and Managing Student Profiles
2. Adding and Updating Courses
3. Enrolling in Courses
4. Checking Prerequisites
5. Verifying Course Enrollment
##########################################################################*/

#include <bits/stdc++.h>
using namespace std;

// Forward declaration
class Course;

// Class representing a student
class Student
{
private:
    string studentId;
    string name;
    unordered_set<string> completedCourses;
    unordered_set<Enrollment *> enrollments;

public:
    Student(const string &studentId, const string &name)
        : studentId(studentId), name(name) {}

    bool enrollInCourse(Course *course)
    {
        if (course->checkPrerequisites(completedCourses))
        {
            Enrollment *newEnrollment = new Enrollment(this, course);
            enrollments.insert(newEnrollment);
            course->addStudent(this);
            return true;
        }
        return false;
    }
    void addCompletedCourse(string courseId)
    {
        completedCourses.add(courseId);
    }

    const string &getStudentId() const
    {
        return studentId;
    }

    const string &getName() const
    {
        return name;
    }
};

// Class representing a course
class Course
{
private:
    string courseId;
    string title;
    int capacity;
    unordered_set<string> prerequisites;
    unordered_set<Student *> studentsEnrolled;

public:
    Course(const string &courseId, const string &title, int capacity)
        : courseId(courseId), title(title), capacity(capacity) {}

    bool addStudent(Student *student)
    {
        if (studentsEnrolled.size() < capacity)
        {
            studentsEnrolled.insert(student);
            return true;
        }
        return false;
    }

    void addPrerequisite(const string &prerequisiteCourseId)
    {
        prerequisites.insert(prerequisiteCourseId);
    }

    bool checkPrerequisites(const unordered_set<string> &completedCourses) const
    {
        for (const auto &prereq : prerequisites)
        {
            if (completedCourses.find(prereq) == completedCourses.end())
            {
                return false;
            }
        }
        return true;
    }

    const string &getCourseId() const
    {
        return courseId;
    }
};

// Class representing a course enrollment
class Enrollment
{
private:
    Student *student;
    Course *course;

public:
    Enrollment(Student *student, Course *course)
        : student(student), course(course) {}
};

// Class responsible for managing course registration
class CourseRegistrationSystem
{
private:
    vector<Student *> students;
    vector<Course *> courses;

public:
    void addStudent(Student *student)
    {
        students.push_back(student);
    }

    void addCourse(Course *course)
    {
        courses.push_back(course);
    }

    bool registerStudentForCourse(const string &studentId, const string &courseId)
    {
        Student *student = nullptr;
        Course *course = nullptr;

        // Find student and course
        for (auto &s : students)
        {
            if (s->getStudentId() == studentId)
            {
                student = s;
                break;
            }
        }

        for (auto &c : courses)
        {
            if (c->getCourseId() == courseId)
            {
                course = c;
                break;
            }
        }

        if (student != nullptr && course != nullptr)
        {
            return student->enrollInCourse(course);
        }

        return false;
    }
};

int main()
{
    // Create course registration system
    CourseRegistrationSystem registrationSystem;

    // Create students
    Student *student1 = new Student("101", "Alice");
    Student *student2 = new Student("102", "Bob");

    // Add students to the registration system
    registrationSystem.addStudent(student1);
    registrationSystem.addStudent(student2);

    // Create courses
    Course *course1 = new Course("C101", "Introduction to Programming", 50);
    Course *course2 = new Course("C102", "Data Structures", 40);

    // Add courses to the registration system
    registrationSystem.addCourse(course1);
    registrationSystem.addCourse(course2);

    // Enroll students in courses
    registrationSystem.registerStudentForCourse("101", "C101");
    registrationSystem.registerStudentForCourse("102", "C102");

    return 0;
}
