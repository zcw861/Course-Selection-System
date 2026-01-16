// Module
// File: course.cppm    Version: 0.1.0  Lincense: AGPLv3
// Created: JiangFan       2026-01-09 14:27:01
// Description:
//  this is a course class

export module cs:entity.course;

import std;

class Student;
class Teacher;

using std::string;
using std::shared_ptr;
using std::weak_ptr;
using std::vector;
using std::print;


export class Course: public std::enable_shared_from_this<Course>
{
public:
    explicit Course(string name, string id, double credit, int capacity ,string time, string location);
    //获取课程id
    string infoId();
    //获取课程名
    string infoName();
    //课程时间地点学分
    string infoCTL();
    //处理选课请求
    bool acceptEnrollment(shared_ptr<Student> student);
    //处理退课请求
    bool acceptWithdraw(shared_ptr<Student> student);
    //输出选该课的学生表
    void roster();
    //找课
    bool hasId(string id);

private:
    string m_courseId;
    string m_courseName;
    int m_capacity;
    double m_credit;
    string m_time;
    string m_location;

    //该课程的学生
    vector<weak_ptr<Student>> _student;
    vector<weak_ptr<Teacher>> _teacher;
};


Course::Course(string name, string id, double credit, int capacity ,string time, string location)
    : m_courseName(name), m_courseId(id), m_credit(credit), m_capacity(capacity), m_time(time), m_location(location)
{}


string Course::infoId()
{
    return std::format("{}", m_courseId);
}

string Course::infoName()
{
    return std::format("{}", m_courseName);
}


bool Course::hasId(string id){
    return id == m_courseId;
}

string Course::infoCTL()
{
    return std::format("学分：{}, 时间：{}, 地点：{}", m_credit, m_time, m_location);
}



