// Module
// File: enrollmentController.cppm    Version: 0.1.0  Lincense: AGPLv3
// Created: ChengWei Zhou       2026-01-13 20:11:34
// Description:
//  Student Course Selection

export module Controller:Enrollenmet;

import std;

class Student;
class Teacher;
class Secretary;
class Course;
class Score;


export class EnrollmentController {
public:
    EnrollmentController(
        const std::vector<std::shared_ptr<Student>>& students,
        const std::vector<std::shared_ptr<Teacher>>& teachers,
        const std::vector<std::shared_ptr<Secretary>>& secretaries
    );

    void enroll(std::shared_ptr<Student> student, std::shared_ptr<Course> course);
    void drop(std::shared_ptr<Student> student, std::shared_ptr<Course> course);

private:
    std::vector<std::shared_ptr<Student>> _students;
    std::vector<std::shared_ptr<Teacher>> _teachers;
    std::vector<std::shared_ptr<Secretary>> _secretaries;
};


EnrollmentController::EnrollmentController(
    const std::vector<std::shared_ptr<Student>>& students,
    const std::vector<std::shared_ptr<Teacher>>& teachers,
    const std::vector<std::shared_ptr<Secretary>>& secretaries)
    : _students(students), _teachers(teachers), _secretaries(secretaries) {}

void EnrollmentController::enroll(std::shared_ptr<Student> student, std::shared_ptr<Course> course) {
    if (!student || !course) throw std::invalid_argument("Student or Course is null.");
    // 假设 Student 有 addCourse 方法
    student->addCourse(course);
    std::cout << "[EnrollmentController] Student enrolled in course.\n";
}

void EnrollmentController::drop(std::shared_ptr<Student> student, std::shared_ptr<Course> course) {
    if (!student || !course) throw std::invalid_argument("Student or Course is null.");
    // 假设 Student 有 removeCourse 方法
    student->removeCourse(course);
    std::cout << "[EnrollmentController] Student dropped course.\n";
}