// Module
// File: teacher.cppm    Version: 0.1.0  Lincense: AGPLv3
// Created:  JiangFan      2026-01-09 17:02:17
// Description:
// a teacher class

export module selection:teacher;

import std;
import :person;

import :course;
import :student;

using std::shared_ptr;
using std::weak_ptr;
using std::cin;
using std::print;

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

void Teacher::markGrade(shared_ptr<Student> student)
{
    //学生存在
    if (!student)
    {
        print("错误：没找到该学生！\n");
        return;
    }
    //教师没有课程
    if (_courses.empty())
    {
        print("错误：您没有负责的课程！\n");
        return;
    }



}

void Teacher::checkStudentResult(shared_ptr<Student> student)
{

}

void Teacher::printStudentRoster(shared_ptr<Course> course)
{
    if (_courses.size() == 0)
    {
        print("该教师没有教授课程！\n");
        return;
    }

    //转化
    for (auto c : _courses)
    {
        auto c_ptr = c.lock();
        if (c_ptr)
            c_ptr->roster();
}
