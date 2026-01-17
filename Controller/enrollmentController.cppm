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

    bool enrollCourse(const string &studentId, const string &courseId);
    bool dropCourse(const string &studentId, const string &courseId);
    std::shared_ptr<Course> findCourseById(const string &courseId);
    std::shared_ptr<Student> findStudentById(const string &studentId);

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

bool EnrollmentController::enrollCourse(const string &studentId, const string &courseId) {
    auto student = findStudentById(studentId);
    auto course = findCourseById(courseId);

    if (student && course) {
        student->enrollsIn(course);
        return true;
    }
    return false;
}

bool EnrollmentController::dropCourse(const string &studentId, const string &courseId) {
    auto student = findStudentById(studentId);
    auto course = findCourseById(courseId);

    if (student && course) {
        student->dropCourse(course);
        return true;
    }
    return false;
}

std::shared_ptr<Course> EnrollmentController::findCourseById(const string &courseId) {
    for (auto course : _courses) {
        if (course->hasId(courseId)) return course;
    }
    return nullptr;
}

std::shared_ptr<Student> EnrollmentController::findStudentById(const string &studentId) {
    for (auto student : _students) {
        if (student->hasId(studentId)) return student;
    }
    return nullptr;
}
