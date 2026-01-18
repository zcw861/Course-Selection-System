// Module
// File: student.cppm    Version: 0.1.0  Lincense: AGPLv3
// Created: JiangFan       2026-01-09 14:48:04
// Description:
// a student class


export module cs:entity.student;

import std;

import :entity.person;

class Score;
class Course;

using std::vector;
using std::shared_ptr;
using std::string;
using std::print;
using std::weak_ptr;

export class Student : public Person, public std::enable_shared_from_this<Student> //传递智能指针需要
{
public:
    explicit Student(string id, string name);
    //选课
    void enrollsIn(shared_ptr<Course> course);
    //退课
    bool dropCourse(shared_ptr<Course> course);
    //查看成绩
    void checkResult();
    //输出打印课表
    void schedule();

    bool hasId(string id);

private:
    vector<weak_ptr<Score>> _transcript;
    vector<weak_ptr<Course>> _course;
};


Student::Student(string id, string name)
    :Person(id, name)
{}


bool Student::hasId(string id)
{
    return id == m_pid;
}


