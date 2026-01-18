// Module
// File: enrollmentController.cppm    Version: 0.1.0  License: AGPLv3
// Created: ChengWei Zhou       2026-01-13 20:11:34
// Description:
//  Student Course Selection

//          [v0.1.1] 周城伟 3269038743@qq.com   2026-01-18 14:05:48
//          *使用数据层接口，通过单例模式实现数据持久化

//          [v0.1.2] 周城伟 3269038743@qq.com   2026-01-18 19:41:17
//          *增加了一些错误处理，添加了一些新的方法

export module cs:controller.enrollment;

import std;
import :entity;
import :database;

using std::string;
using std::print;
using std::shared_ptr;
using std::vector;


export class EnrollmentController {
public:
    //学生选课
    bool enrollCourse(const string &studentId, const string &courseId);
    //学生退课
    bool dropCourse(const string &studentId, const string &courseId);
    //获取学生选课列表
    bool getStudentSchedule(const string &studentId, vector<shared_ptr<Course>>& out);
};

bool EnrollmentController::enrollCourse(const string &studentId, const string &courseId) {
    std::shared_ptr<Student> student;
    std::shared_ptr<Course> course;

    //从数据库获取学生和课程对象
    if (!StudentDatabase::singleton().findStudentById(studentId, student)) {
        print("学生 {} 不存在。\n", studentId);
        return false;
    }

    if (!CourseDatabase::singleton().findCourseById(courseId, course)) {
        print("课程 {} 不存在。\n", courseId);
        return false;
    }

    //检查数据库中是否已选过该课程
    if (StudentDatabase::singleton().isEnrolled(studentId, courseId)) {
        print("学生 {} 已经选过课程 {} 了！\n", student->infoName(), course->infoName());
        return false;
    }


    //检查课程容量,从数据库重新获取课程信息以确保容量是最新的
    auto freshCourse = std::make_shared<Course>("", "", 0.0, 0, "", "");
    if (!CourseDatabase::singleton().findCourseById(courseId, freshCourse)) {
        print("获取课程最新信息失败。\n");
        return false;
    }
    auto enrolledCount = freshCourse->getEnrolledCount(); //假设getEnrolledCount也能从数据库计算
    if (enrolledCount >= freshCourse->getCapacity()) {
        print("课程 {} 容量已满，选课失败！\n", freshCourse->infoName());
        return false;
    }

    //执行选课,更新内存中的course对象
    course->acceptEnrollment(student);

    //将选课记录保存到数据库
    if (!StudentDatabase::singleton().saveEnrollment(studentId, courseId)) {
        print("保存选课记录到数据库失败。\n");
        return false;
    }

    if (!CourseDatabase::singleton().saveCourse(course)) {
        print("更新课程信息到数据库失败。\n");
        //数据库不一致，返回失败
        return false;
    }

    print("学生 {} 选课 {} 成功！\n", student->infoName(), course->infoName());
    return true;
}


bool EnrollmentController::dropCourse(const string &studentId, const string &courseId) {
    std::shared_ptr<Student> student;
    std::shared_ptr<Course> course;

    if (!StudentDatabase::singleton().findStudentById(studentId, student)) {
        print("学生 {} 不存在。\n", studentId);
        return false;
    }

    if (!CourseDatabase::singleton().findCourseById(courseId, course)) {
        print("课程 {} 不存在。\n", courseId);
        return false;
    }

    //检查数据库中是否存在选课记录
    if (!StudentDatabase::singleton().isEnrolled(studentId, courseId)) {
        print("学生 {} 未选过课程 {}，无法退课！\n", student->infoName(), course->infoName());
        return false;
    }

    //执行退课
    bool success = course->acceptWithdraw(student);

    if (!success) {
        print("移除学生失败，状态不一致！\n");
        return false;
    }

    //从数据库中删除选课记录
    if (!StudentDatabase::singleton().removeEnrollment(studentId, courseId)) {
        print("从数据库删除选课记录失败。\n");
        return false;
    }

    //更新课程信息到数据库
    if (!CourseDatabase::singleton().saveCourse(course)) {
        print("更新课程信息到数据库失败。\n");
        return false;
    }

    print("学生 {} 退课 {} 成功！\n", student->infoName(), course->infoName());
    return true;
}

bool EnrollmentController::getStudentSchedule(const string &studentId, vector<shared_ptr<Course>>& out) {
    return StudentDatabase::singleton().findSchedule(studentId, out);
}

