// Module
// File: student.cppm    Version: 0.1.0  Lincense: AGPLv3
// Created: JiangFan       2026-01-09 14:48:04
// Description:
// a student class

export module selection:student;

import std;
import :person;
import :course;

using std::vector;
using std::shared_ptr;
using std::string;
using std::print;

export class Student : public Person, public std::enable_shared_from_this<Student> //传递智能指针需要
{
public:
    explicit Student(string id, string name);
    //选课
    void enrollsIn(shared_ptr<class Course> course);
    //退课
    bool dropCourse(shared_ptr<class Course> course);
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


void Student::enrollsIn(shared_ptr<class Course> course)
{
    //不用判空，上层会做
    if (course->acceptEnrollment(shared_from_this()))
        _course.push_back(course);
}

//退课
void Student::dropCourse(shared_ptr<class Course> course)
{
    //不用判空，上层会做
    if (course->acceptWithdraw(shared_from_this())) //转化
    {
        //找到该课程
        auto c = std::find_if(_course.begin(), _course.end(),[course](const weak_ptr<Course>& c) //
            { auto c_ptr = c.lock();
            return c_ptr && c_ptr->infoId() == course->infoId();}); //c 是weak_ptr<Course>，需lock()判空后调用成员函数
    }

    if (c != _course.end())
    {
        _course.erase(c);
        return;
    }

}

//输出打印成绩单
void Student::checkResult()
{
    print("\n+-----------成绩单-----------+\n");
    print("     姓名：{}  学号：{}\n", infoName(), infoId());

    for (auto sc: _transcript)
    {
        //course name and score
        //转化
        auto sc_ptr = sc.lock();
        if (sc_ptr)
            print("{}:  {}\n", sc_ptr->infoCourse(), sc_ptr->infoGrade());

    print("+---------------------------+\n");
}

void Student::schedule()
{
    print("\n+-----------课程表-----------+\n");
    print("     姓名：{}  学号：{}\n", m_pname, m_pid);

    for (auto c: _course)
    {
        //转化
        auto c_ptr = c.lock();
        if (c_ptr)
            print("{}:  {}\n", c_ptr->infoName(),  c_ptr->infoCTL());
    }

    print("+---------------------------+\n");
}

bool Student::hasId(string id)
{
    return id == m_pid;
}

