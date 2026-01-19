// Module
// File: course.cppm    Version: 0.1.0  Lincense: AGPLv3
// Created: JiangFan       2026-01-09 14:27:01
// Description:
//  this is a course class
    // Chengwei Zhou: 添加了一些info函数

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
    string infoId()const;
    //获取课程名
    string infoName()const;
    string infoCredit()const;
    string infoCapacity()const;
    string infoTime()const;
    string infoLocation()const;
    //修改学分、时间、地点
    string infoCTL();
    //修改学分、时间、地点
    bool modifyTL(string time, string location);
    //处理选课请求
    bool acceptEnrollment(shared_ptr<Student> student);
    //处理退课请求
    bool acceptWithdraw(shared_ptr<Student> student);
    //输出选该课的学生表
    void roster();
    //找课
    bool hasId(string id);
    //给课程添加老师
    bool addTeacher(shared_ptr<Teacher> teacher);

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


string Course::infoId()const
{
    return std::format("{}", m_courseId);
}

string Course::infoName()const
{
    return std::format("{}", m_courseName);
}

string Course::infoCredit() const {
    return std::format("{}", m_credit);
}

string Course::infoCapacity() const {
    return std::format("{}", m_capacity);
}

string Course::infoTime() const {
    return std::format("{}", m_time);
}

string Course::infoLocation() const {
    return std::format("{}", m_location);
}


bool Course::hasId(string id){
    return id == m_courseId;
}

string Course::infoCTL()
{
    return std::format("学分：{}, 时间：{}, 地点：{}", m_credit, m_time, m_location);
}

bool Course::modifyTL(string time, string location)
{
    //输入信息均不为空（就不考虑更细致的问题了）
    if (time.empty() || location.empty())
    {
        print("错误：时间或地点不能为空！\n");
        return false;
    }

    m_time = time;
    m_location = location;

    print("修改成功！\n");
    return true;
}



