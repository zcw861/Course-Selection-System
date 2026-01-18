// Module
// File: courseController.cppm    Version: 0.1.0  License: AGPLv3
// Created: ChengWei Zhou       2026-01-12 17:21:03
// Description:
//  Create a course, assign a teacher to it, and then publish the course.

//          [v0.1.1] 周城伟 3269038743@qq.com   2026-01-18 13:23:11
//          *使用数据层接口，通过单例模式实现数据持久化

export module cs:controller.course;

import std;
import :entity.teacher;
import :entity.course;
import :entity.secretary;
import :database.course;
import :database.teacher;


export class CourseController {
public:
    void initializeCourse();
    void createCourse(string name,string id, double credit, double capacity, string time = "0", string location = "0");
    bool attributeTeacher(const string &teacherId, const string &courseId);
};


void CourseController::initializeCourse() {
    CourseDatabase::singleton().saveCourse(std::make_shared<Course> ("计算机组成原理","CS101",4,60,"0","0"));
    CourseDatabase::singleton().saveCourse(std::make_shared<Course> ("计算机网络","CS102",4,70,"0","0"));
    CourseDatabase::singleton().saveCourse(std::make_shared<Course> ("数据结构","CS103",5,65,"0","0"));
    CourseDatabase::singleton().saveCourse(std::make_shared<Course> ("深入理解计算机","CS104",5,70,"0","0"));
    CourseDatabase::singleton().saveCourse(std::make_shared<Course> ("软件构建与实现","CS105",5,70,"0","0"));
}

void CourseController::createCourse(string name, string id, double credit,double capacity, string time, string location) {

    auto course = std::make_shared<Course>(name, id, credit, capacity, time, location);
    CourseDatabase::singleton().saveCourse(course);
}

bool CourseController::attributeTeacher(
    const string &teacherId,
    const string &courseId
) {
    std::shared_ptr<Teacher> teacher;
    std::shared_ptr<Course> course;

    if (!TeacherDatabase::singleton().findTeacherById(teacherId, teacher))
        return false;

    if (!CourseDatabase::singleton().findCourseById(courseId, course))
        return false;

    teacher->assignCourse(course);
    return true;
}




