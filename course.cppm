// Module
// File: course.cppm    Version: 0.1.0  Lincense: AGPLv3
// Created: JiangFan       2026-01-09 14:27:01
// Description:
//  this is a course class

export module selection:course;

import std;
import :student;

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

//选课操作
bool Course::acceptEnrollment(shared_ptr<Student> student)
{
    //不用判空，上层会做

    //已经在该课程内
    for (auto& s: _student){
        auto s_ptr = s.lock(); // 这里需要将weak_ptr转换为shared_ptr才能访问成员函数  -m-
        if (s_ptr && s_ptr->hasId(student->infoId()))
        {
            print("你已经在该课程中，无需再选！\n");
            return false;
        }
    }

    //看课程容量
    if (_student.size() >= m_capacity)
    {
        print("课程名额已满，选课失败！\n");
        return false;
    }

    //选课
    _student.push_back(student);
    student->enrollsIn(shared_from_this());
    print("{}\n 选课成功，目前选择该课程的人数: {}\n", student->infoName(), _student.size());
    return true;
}

//退课操作
bool Course::acceptWithdraw(shared_ptr<Student> student)
{
    //不用判空，上层会做

    //找到该学生
    auto s = std::find_if(_student.begin(), _student.end(),[&student](const weak_ptr<Student>& s)
    {
        auto s_ptr = s.lock();
        return s_ptr && s_ptr->hasId(student->infoId());
    });

    if (s != _student.end())
    {
        _student.erase(s);
        print("{}, 退课成功！\n", student->infoName());
        return true;
    }

    print("{}, 退课失败！你没有选这门 {} 课程\n", student->infoName(), m_courseName);
    return false;
}

//输出选该课的学生表
void Course::roster()
{
    print("\n课程： {}     课程id： {}        选课人数： {}        最大容量： {}\n"
        , m_courseName, m_courseId, _student.size(), m_capacity);

    print("选课名单：");
    int flag = 0; //换行标志
    for (auto& s: _student)
    {
        //一排放八个
        if (flag % 8 == 0)
            print("\n");
        //
        auto s_ptr = s.lock();  //s是weak_ptr<Student>，需lock()判空后调用infoName()
        if (s_ptr)
            print("{:3}   ", s.infoName());
        flag++;
    }
}

bool Course::hasId(string id){
    return id == m_courseId;
}

string Course::infoCTL()
{
    return std::format("学分：{}, 时间：{}, 地点：{}", m_credit, m_time, m_location);
}



