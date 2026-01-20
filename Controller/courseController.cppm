// Module
// File: courseController.cppm    Version: 0.1.0  License: AGPLv3
// Created: ChengWei Zhou       2026-01-12 17:21:03
// Description:
//  Create a course, assign a teacher to it, and then publish the course.

//          [v0.1.1] 周城伟 3269038743@qq.com   2026-01-18 13:23:11
//          *使用数据层接口，通过单例模式实现数据持久化

//          [v0.1.2] 周城伟 3269038743@qq.com   2026-01-18 23:30:46
//          *注释了老师绑定课程的函数

export module cs:controller.course;

import std;
import :entity;
import :database.course;
import :database.teacher;

using std::string;


export class CourseController {
public:
    void initializeCourse();    //初始化课程表
    void createCourse(string name,string id, double credit, int capacity, string time = "0", string location = "0");    //用于创建课程
    bool attributeTeacher(const string &teacherId, const string &courseId);     //用于分配教师
};


void CourseController::initializeCourse() {
    CourseDatabase::singleton().saveCourse(std::make_shared<Course> ("计算机组成原理","CS101",4,60,"0","0"));
    CourseDatabase::singleton().saveCourse(std::make_shared<Course> ("计算机网络","CS102",4,70,"0","0"));
    CourseDatabase::singleton().saveCourse(std::make_shared<Course> ("数据结构","CS103",5,65,"0","0"));
    CourseDatabase::singleton().saveCourse(std::make_shared<Course> ("深入理解计算机系统","CS104",5,70,"0","0"));
    CourseDatabase::singleton().saveCourse(std::make_shared<Course> ("软件构建与实现","CS105",5,70,"0","0"));
}

void CourseController::createCourse(string name, string id, double credit,int capacity, string time, string location) {

    auto course = std::make_shared<Course>(name, id, credit, capacity, time, location);
    // 通过数据层接口保存课程
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

    //teacher->assignCourse(course);
    return true;
}




