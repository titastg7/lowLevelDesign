#include <bits/stdc++.h>

using namespace std;

class Student
{
public:
    virtual string getName() = 0;
    virtual string getSurname() = 0;
    virtual string getEmail() = 0;
};

class CollegeStudent : public Student
{
private:
    string name;
    string surname;
    string email;

public:
    CollegeStudent(string cname, string csurname, string cemail)
    {
        name = cname;
        surname = csurname;
        email = cemail;
    }
    string getName() override
    {
        return name;
    }
    string getSurname() override
    {
        return surname;
    }
    string getEmail() override
    {
        return email;
    }
};

class SchoolStudent
{
private:
    string firstname;
    string lastname;
    string emailaddress;

public:
    SchoolStudent() {}
    SchoolStudent(string sfirstname, string slastname, string semailaddress)
    {
        firstname = sfirstname;
        lastname = slastname;
        emailaddress = semailaddress;
    }
    string getFirstname()
    {
        return firstname;
    }
    string getLastname()
    {
        return lastname;
    }
    string getEmailaddress()
    {
        return emailaddress;
    }
};

class SchoolStudentAdapter : public Student
{
private:
    SchoolStudent schoolStudent;

public:
    SchoolStudentAdapter(SchoolStudent *sStudent)
    {
        schoolStudent = *sStudent;
    }
    string getName() override
    {
        return schoolStudent.getFirstname();
    }
    string getSurname() override
    {
        return schoolStudent.getLastname();
    }
    string getEmail() override
    {
        return schoolStudent.getEmailaddress();
    }
};

class StudentClient
{
public:
    vector<Student *> getStudentList()
    {
        vector<Student *> students;
        CollegeStudent *collegeStudent = new CollegeStudent("a", "b", "c");
        SchoolStudent *schoolStudent = new SchoolStudent("x", "y", "z");

        students.push_back(collegeStudent);
        students.push_back(new SchoolStudentAdapter(schoolStudent));
        return students;
    }
};

int main()
{
    StudentClient *client = new StudentClient();
    vector<Student *> studentList = client->getStudentList();
    for (auto e : studentList)
        cout << e->getName() << endl;
    return 0;
}