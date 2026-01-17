// Module
// File: courseController.cppm    Version: 0.1.0  License: AGPLv3
// Created: ChengWei Zhou       2026-01-12 17:21:03
// Description:
//  Create a course, assign a teacher to it, and then publish the course.

export module cs:controller.course;

import std;
import :entity.teacher;
import :entity.course;
import :entity.secretary;
#include <memory>


export class CourseController {
public:
    CourseController(
        const std::vector<std::shared_ptr<Course>> &courses,
        const std::vector<std::shared_ptr<Teacher>> &teachers,
        const std::vector<std::shared_ptr<Secretary>> &secretaries
    );

    void createCourse();
    bool attributeTeacher(const string &teacherId, const string &courseId);
    shared_ptr<Teacher> findTeacherById(string teacherId);
    shared_ptr<Course> findCourseById(string courseId);

private:
    std::vector<std::shared_ptr<Course>> _courses;
    std::vector<std::shared_ptr<Teacher>> _teachers;
    std::vector<std::shared_ptr<Secretary>> _secretaries;
};


CourseController::CourseController(
    const std::vector<std::shared_ptr<Course>>& courses,
    const std::vector<std::shared_ptr<Teacher>>& teachers,
    const std::vector<std::shared_ptr<Secretary>>& secretaries)
    : _courses(courses), _teachers(teachers), _secretaries(secretaries) {}


void CourseController::createCourse() {
    _courses.push_back(std::make_shared<Course> ("计算机组成原理","CS101",4,60,"0","0"));
    _courses.push_back(std::make_shared<Course> ("计算机网络","CS102",4,70,"0","0"));
    _courses.push_back(std::make_shared<Course> ("数据结构","CS103",5,65,"0","0"));
    _courses.push_back(std::make_shared<Course> ("深入理解计算机","CS104",5,70,"0","0"));
    _courses.push_back(std::make_shared<Course> ("软件构建与实现","CS105",5,70,"0","0"));
}


bool CourseController::attributeTeacher(const string &teacherId, const string &courseId) {
    auto teacher = findTeacherById(teacherId);
    auto course = findCourseById(courseId);
    teacher->assginCourse(course);
}

shared_ptr<Teacher> CourseController::findTeacherById(string teacherId) {
    for (const auto& teacher : _teachers) {
        if (teacher->hasId(teacherId))  return teacher;
    }
    return nullptr;
}

shared_ptr<Course> CourseController::findCourseById(string courseId) {
    for (const auto& course : _courses) {
        if (course->hasId(courseId))  return course;
    }
    return nullptr;
}





