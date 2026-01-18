// Module
// File: enrollmentController.cppm    Version: 0.1.0  License: AGPLv3
// Created: ChengWei Zhou       2026-01-13 20:11:34
// Description:
//  Student Course Selection

//          [v0.1.1] 周城伟 3269038743@qq.com   2026-01-18 14:05:48
//          *使用数据层接口，通过单例模式实现数据持久化

export module cs:controller.enrollment;

import std;
import :entity.student;
import :entity.course;
import :entity.teacher;
import :database.student;
import :database.course;


using std::string;
using std::shared_ptr;


export class EnrollmentController {
public:
    bool enrollCourse(const string &studentId, const string &courseId);
    bool dropCourse(const string &studentId, const string &courseId);
};

bool EnrollmentController::enrollCourse(const string &studentId, const string &courseId) {
    std::shared_ptr<Student> student;
    std::shared_ptr<Course> course;

    if (!StudentDatabase::singleton().findStudentById(studentId, student))
        return false;

    if (!CourseDatabase::singleton().findCourseById(courseId, course))
        return false;

    // 业务逻辑
    course->acceptEnrollment(student);

    // 持久化
    StudentDatabase::singleton().saveEnrollment(studentId, courseId);
    return true;
}

bool EnrollmentController::dropCourse(const string &studentId, const string &courseId) {
    std::shared_ptr<Student> student;
    std::shared_ptr<Course> course;

    if (!StudentDatabase::singleton().findStudentById(studentId, student))
        return false;

    if (!CourseDatabase::singleton().findCourseById(courseId, course))
        return false;

    course->acceptWithdraw(student);

    StudentDatabase::singleton().removeEnrollment(studentId, courseId);
    return true;
}

