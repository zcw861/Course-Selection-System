// Module
// File: teacher.cppm    Version: 0.1.0  Lincense: AGPLv3
// Created:  JiangFan      2026-01-09 17:02:17
// Description:
// a teacher class

export module cs:entity.teacher;

import std;
import :entity.person;

class Student;
class Course;

using std::shared_ptr;
using std::weak_ptr;
using std::cin;
using std::print;
using std::vector;

export class Teacher : public Person
{
public:
    explicit Teacher(string id, string name);
    //评定成绩
    void markGrade(shared_ptr<Student> student);
    //查询指定学生的课程（该老师负责的）成绩
    void checkStudentResult(shared_ptr<Student> student);
    //打印该老师负责的学生名单
    void printStudentRoster(shared_ptr<Course> course);

private:

    vector<weak_ptr<Course>> _courses;
};



Teacher::Teacher(string id, string name)
    : Person(id, name)
{}

