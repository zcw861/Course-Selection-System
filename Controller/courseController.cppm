// Module
// File: courseController.cppm    Version: 0.1.0  Lincense: AGPLv3
// Created: ChengWei Zhou       2026-01-12 17:21:03
// Description:
//  Create a course, assign a teacher to it, and then publish the course.

export module Controller:Course;

import std;

// 前向声明领域层实体（由其他模块提供）
class Student;
class Teacher;
class Secretary;
class Course;
class Score;


export class CourseController {
public:
    CourseController(
        const std::vector<std::shared_ptr<Student>>& students,
        const std::vector<std::shared_ptr<Teacher>>& teachers,
        const std::vector<std::shared_ptr<Secretary>>& secretaries
    );

    void createCourse(const std::string& courseName, int courseId, std::shared_ptr<Teacher> teacher);

private:
    std::vector<std::shared_ptr<Student>> _students;
    std::vector<std::shared_ptr<Teacher>> _teachers;
    std::vector<std::shared_ptr<Secretary>> _secretaries;
};


CourseController::CourseController(
    const std::vector<std::shared_ptr<Student>>& students,
    const std::vector<std::shared_ptr<Teacher>>& teachers,
    const std::vector<std::shared_ptr<Secretary>>& secretaries)
    : _students(students), _teachers(teachers), _secretaries(secretaries) {}

void CourseController::createCourse(const std::string& courseName, int courseId, std::shared_ptr<Teacher> teacher) {
    if (!teacher) throw std::invalid_argument("Teacher is null.");
    // 注意：实际课程创建和关联逻辑应由 Teacher 或 Course 自身处理
    // 此处仅调用（假设 Teacher 有 assignCourse 方法）
    teacher->assignCourse(std::make_shared<Course>(courseId, courseName));
    std::cout << "[CourseController] Created course: " << courseName << " (ID: " << courseId << ")\n";
}



