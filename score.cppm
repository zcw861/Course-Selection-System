// Module
// File: score.cppm    Version: 0.1.0  Lincense: AGPLv3
// Created: JiangFan       2026-01-09 18:09:35
// Description:
// a score class

export module selection:score;

import std;

import :course;
import :student;

using std::weak_ptr;
using std::string;
using std::print;

export class Score
{
public:
    explicit Score(weak_ptr<Course> course, weak_ptr<Student> student, double grade = 0);

    double infoGrade();
    string infoCourse();
    string infoStudentId();
    void modifyGrade(double g);

private:
    double m_grade;
    weak_ptr<Course> _course;
    weak_ptr<Student> _student;
};

Score::Score(weak_ptr<Course> course, weak_ptr<Student> student, double grade)
    : _course(course), _student(student), m_grade(grade)
{}

double Score::infoGrade()
{
    return m_grade;
}

string Score::infoCourse()
{
    //return std::format("{}", _course->infoId());
    //转化
    return std::format("{}", {auto c_ptr = _course.lock(); return c_ptr ? c_ptr->infoId() : "课程已销毁";});
}

string Score::infoStudentId()
{
    //return std::format("{}", _student->infoId());
    //转化
    return std::format("{}", {auto s_ptr = _student.lock(); return s_ptr ? s_ptr->infoId() : "学生已销毁";});
}

void Score::modifyGrade(double g)
{
    if (g < 0 || g > 100)
    {
        print("错误！分数应在[0,100]区间内！\n");
        return;
    }
    else if (g == m_grade)
    {
        print("不能将分数改成与之前一样的！\n");
        return;
    }

    m_grade = g;
}
