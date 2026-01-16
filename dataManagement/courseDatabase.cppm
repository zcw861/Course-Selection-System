// Module
// File: courseDatabase.cppm   Version: 0.1.0   License: AGPLv3
// Created: 何志远   3270883879@qq.com   2026-01-16 14:16:01
// Description:
//
export module cs:database.course;

import std;
import :entity.course;

export class CourseDatabase{
public:
    static CourseDatabase singleton();
    bool saveCourse(const Course &course);
    bool deleteCourse(std::string courseId);
    bool updateCourse(const Course &course);
    bool findCourseById(std::string courseId);

private:
    std::vector<std::shared_ptr<Course>> _courses;
};





