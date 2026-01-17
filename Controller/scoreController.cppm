// Module
// File: enrollmentController.cppm    Version: 0.1.0  License: AGPLv3
// Created: ChengWei Zhou       2026-01-15 20:31:37
// Description:
//  Grade Calculation and Release

export module cs:controller.score;

import std;
import :entity.teacher;
import :entity.student;
import :entity.secretary;
import :entity.score;

using std::string;

export class ScoreController {
public:
    ScoreController(
        const std::vector<std::shared_ptr<Student>>& students,
        const std::vector<std::shared_ptr<Teacher>>& teachers,
        const std::vector<std::shared_ptr<Secretary>>& secretaries,
        const std::vector<std::shared_ptr<Score>>& scores
    );

    bool gradeStdent(string studentId,string courseId,double grade);
    std::vector<Score> getStudentTranscript();

private:
    std::vector<std::shared_ptr<Student>> _students;
    std::vector<std::shared_ptr<Teacher>> _teachers;
    std::vector<std::shared_ptr<Secretary>> _secretaries;
    std::vector<std::shared_ptr<Score>> _scores;

};


ScoreController::ScoreController(
    const std::vector<std::shared_ptr<Student>>& students,
    const std::vector<std::shared_ptr<Teacher>>& teachers,
    const std::vector<std::shared_ptr<Secretary>>& secretaries,
    const std::vector<std::shared_ptr<Score>>& scores)
    : _students(students), _teachers(teachers), _secretaries(secretaries),_scores(scores) {}

std::vector<Score> ScoreController::getStudentTranscript() {

}

bool ScoreController::gradeStdent(string studentId, string courseId, double grade) {

}
