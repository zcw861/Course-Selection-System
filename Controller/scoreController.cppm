// Module
// File: enrollmentController.cppm    Version: 0.1.0  Lincense: AGPLv3
// Created: ChengWei Zhou       2026-01-15 20:31:37
// Description:
//  Grade Calculation and Release

export module Controller:Score;

import std;

class Student;
class Teacher;
class Secretary;
class Course;
class Score;


export class ScoreController {
public:
    ScoreController(
        const std::vector<std::shared_ptr<Student>>& students,
        const std::vector<std::shared_ptr<Teacher>>& teachers,
        const std::vector<std::shared_ptr<Secretary>>& secretaries
    );

    void inputScore(std::shared_ptr<Teacher> teacher, std::shared_ptr<Student> student, std::shared_ptr<Course> course, double score);
    double queryScore(std::shared_ptr<Student> student, std::shared_ptr<Course> course) const;

private:
    std::vector<std::shared_ptr<Student>> _students;
    std::vector<std::shared_ptr<Teacher>> _teachers;
    std::vector<std::shared_ptr<Secretary>> _secretaries;

};


ScoreController::ScoreController(
    const std::vector<std::shared_ptr<Student>>& students,
    const std::vector<std::shared_ptr<Teacher>>& teachers,
    const std::vector<std::shared_ptr<Secretary>>& secretaries)
    : _students(students), _teachers(teachers), _secretaries(secretaries) {}

void ScoreController::inputScore(
    std::shared_ptr<Teacher> teacher,
    std::shared_ptr<Student> student,
    std::shared_ptr<Course> course,
    double score)
{
    if (!teacher || !student || !course) {
        throw std::invalid_argument("Null pointer in inputScore.");
    }
    if (score < 0.0 || score > 100.0) {
        throw std::out_of_range("Score must be between 0 and 100.");
    }

    auto newScore = std::make_shared<Score>(student, course, score);

    student->addScore(newScore);

    std::cout << "[ScoreController] Score " << score
              << " recorded for student " << student->getName()
              << " in course " << course->getCourseName() << ".\n";
}

double ScoreController::queryScore(
    std::shared_ptr<Student> student,
    std::shared_ptr<Course> course) const
{
    if (!student || !course) {
        throw std::invalid_argument("Student or Course is null.");
    }

    // 遍历学生的成绩单，查找匹配课程的成绩
    auto transcript = student->getTranscript();
    for (const auto& scoreRecord : transcript) {
        // Score 内部持有 weak_ptr<Course>，需 lock() 获取 shared_ptr
        if (auto recordedCourse = scoreRecord->getCourse().lock()) {
            if (recordedCourse == course) {
                return scoreRecord->getScore();
            }
        }
    }

    throw std::runtime_error("Score not found for the given student and course.");
}