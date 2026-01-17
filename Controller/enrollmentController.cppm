// Module
// File: enrollmentController.cppm    Version: 0.1.0  License: AGPLv3
// Created: ChengWei Zhou       2026-01-13 20:11:34
// Description:
//  Student Course Selection

export module cs:controller.enrollment;

import std;
import :entity.student;
import :entity.course;
import :entity.teacher;


using std::string;
using std::shared_ptr;


export class EnrollmentController {
public:
    EnrollmentController(
        const std::vector<shared_ptr<Student>> &students,
        const std::vector<shared_ptr<Teacher>> &teachers,
        const std::vector<shared_ptr<Course>> &courses
        );

    bool enrollCourse(shared_ptr<Student> student, shared_ptr<Course> course);
    bool dropCourse(shared_ptr<Student> student, shared_ptr<Course> course);
    std::shared_ptr<Course> findCourseById(string courseId);
    std::shared_ptr<Student> findStudentById(string studentId);


private:
    std::vector<std::shared_ptr<Student>> _students;
    std::vector<std::shared_ptr<Teacher>> _teachers;
    std::vector<std::shared_ptr<Course>> _courses;
};


EnrollmentController::EnrollmentController(
    const std::vector<std::shared_ptr<Student>>& students,
    const std::vector<std::shared_ptr<Teacher>>& teachers,
    const std::vector<std::shared_ptr<Course>> &courses)
    : _students(students), _teachers(teachers),_courses(courses) {}

bool EnrollmentController::enrollCourse(std::shared_ptr<Student> student, std::shared_ptr<Course> course) {

}

bool EnrollmentController::dropCourse(std::shared_ptr<Student> student, std::shared_ptr<Course> course) {

}

std::shared_ptr<Course> EnrollmentController::findCourseById(string courseId) {
    for (auto course : _courses) {
        if (course->hasId(courseId)) return course;
    }
    return nullptr;
}

std::shared_ptr<Student> EnrollmentController::findStudentById(string studentId) {
    for (auto student : _students) {
        if (student->hasId(studentId)) return student;
    }
    return nullptr;
}
