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
    //修改学分、时间、地点
    string infoCTL();
    //修改学分、时间、地点
    bool modifyCTL(double credit, string time, string location);
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

bool Course::modifyCTL(double credit, string time, string location)
{
    //为学分设置了个大致的范围
    if (credit < 0 || credit > 10)
    {
        print("错误：学分应该在（0,10）区间范围内！\n");
        return false;
    }

    //输入信息均不为空（就不考虑更细致的问题了）
    if (time.empty() || location.empty())
    {
        print("错误：时间或地点不能为空！\n");
        return false;
    }

    m_credit = credit;
    m_time = time;
    m_location = location;

    print("修改成功！\n");
    return true;
}



