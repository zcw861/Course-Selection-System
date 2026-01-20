// Module
// File: score.cppm    Version: 0.1.0  Lincense: AGPLv3
// Created: JiangFan       2026-01-09 18:09:35
// Description:
// a score class


export module cs:entity.score;
import std;

using std::weak_ptr;
using std::string;
using std::print;
using std::string;
using std::shared_ptr;


class Course;
class Student;

export class Score
{
public:
    explicit Score(weak_ptr<Course> course, weak_ptr<Student> student, double grade = 0);

    double infoGrade() const;
    string infoCourse() const;
    string infoStudentId() const;
    void modifyGrade(double g);

private:
    double m_grade;
    weak_ptr<Course> _course;
    weak_ptr<Student> _student;
};



double Score::infoGrade() const
{
    return m_grade;
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



