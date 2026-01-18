// Module
// File: enrollmentController.cppm    Version: 0.1.0  License: AGPLv3
// Created: ChengWei Zhou       2026-01-15 20:31:37
// Description:
//  Grade Calculation and Release

//          [v0.1.1] 周城伟 3269038743@qq.com   2026-01-18 14:54:31
//          *使用数据层接口，通过单例模式实现数据持久化


export module cs:controller.score;

import std;
import :entity;
import :database;


using std::string;
using std::shared_ptr;
using std::weak_ptr;

export class ScoreController {
public:
    //记录学生成绩
    bool gradeStudent(string studentId, string courseId, double grade);
    //得到学生成绩单
    std::vector<weak_ptr<Score>> getStudentTranscript(const string& studentId) const;
};


std::vector<weak_ptr<Score>>
ScoreController::getStudentTranscript(const string& studentId) const {
    std::vector<shared_ptr<Score>> scores;
    std::vector<weak_ptr<Score>> result;

    ScoreDatabase::singleton().findScoresByStudentId(studentId, scores);

    for (auto& score : scores)
        result.emplace_back(score);

    return result;
}


bool ScoreController::gradeStudent(
    string studentId,
    string courseId,
    double grade
) {
    shared_ptr<Student> student;
    shared_ptr<Course> course;

    if (!StudentDatabase::singleton().findStudentById(studentId, student))
        return false;

    if (!CourseDatabase::singleton().findCourseById(courseId, course))
        return false;

    auto score = std::make_shared<Score>(
        weak_ptr<Course>(course),
        weak_ptr<Student>(student),
        grade
    );

    //存储交给数据库
    return ScoreDatabase::singleton().saveScore(*score);
}


