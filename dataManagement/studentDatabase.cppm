// Module
// File: studentDatabase.cppm   Version: 0.1.0   License: AGPLv3
// Created: 何志远   3270883879@qq.com   2026-01-16 16:07:57
// Description: 学生数据库操作模块，提供学生信息管理和选课相关功能

//     [v0.1.2] 何志远 3270883879@qq.com   2026-01-17 16:36:40
//         * 更改代码结构，连接PostgreSQL数据库，数据类基本实现

module;
#include "pqxx/pqxx"

export module cs:database.student;


import std;
import :entity;
import :database.course;


//学生数据库操作类
export class StudentDatabase {
private:
    inline static std::unique_ptr<StudentDatabase> _instance{};    //静态单例实例指针
    pqxx::connection _conn;    //PostgreSQL数据库连接对象
    StudentDatabase()
        : _conn(
            "host=localhost port=5432 dbname=course_selection "
            "user=postgres password=123456"    //初始化数据库连接参数
        ) {}
public:
    //获取单例实例的静态方法
    static StudentDatabase& singleton() {
        if (!_instance) _instance.reset(new StudentDatabase());    //如果实例不存在则创建新实例
        return *_instance;
    }
    //保存学生信息
    bool saveStudent(const Student& s) {
        try {
            pqxx::work tx(_conn);
            //存在则更新，不存在则插入
            tx.exec_params(
                "INSERT INTO student(student_id, student_name) "
                "VALUES($1, $2) "
                "ON CONFLICT(student_id) DO UPDATE SET student_name=$2",
                s.infoId(),
                s.infoName()
            );
            tx.commit();
            return true;
        } catch (...) {
            return false;
        }
    }
    // 根据学生ID查询学生信息
    bool findStudentById(std::string id, std::shared_ptr<Student>& out) {
        try {
            pqxx::nontransaction tx(_conn);
            //执行参数化查询
            auto r = tx.exec_params(
                "SELECT student_name FROM student WHERE student_id=$1",
                id
            );
            // 检查查询结果是否为空
            if (r.empty()) return false;    //检查查询结果是否为空
            //创建Student对象并将其赋值给输出参数
            out = std::make_shared<Student>(
                id,
                r[0][0].as<std::string>()
            );
            return true;
        } catch (...) {
            return false;
        }
    }
    //保存选课记录
    bool saveEnrollment(std::string sid, std::string cid) {
        try {
            pqxx::work tx(_conn);
            //插入选课记录到enrollment表
            tx.exec_params(
                "INSERT INTO enrollment(student_id, course_id) VALUES($1, $2)",
                sid,
                cid
            );
            tx.commit();
            return true;
        } catch (...) {
            return false;
        }
    }
    //退课
    bool removeEnrollment(std::string sid, std::string cid) {
        try {
            pqxx::work tx(_conn);
            //删除指定的选课记录
            tx.exec_params(
                "DELETE FROM enrollment WHERE student_id=$1 AND course_id=$2",
                sid,
                cid
            );
            tx.commit();
            return true;
        } catch (...) {
            return false;
        }
    }
    //检查学生是否已选某门课程
    bool isEnrolled(std::string sid, std::string cid) {
        try {
            pqxx::nontransaction tx(_conn);
            //查询是否存在该选课记录
            auto r = tx.exec_params(
                "SELECT 1 FROM enrollment WHERE student_id=$1 AND course_id=$2",
                sid,
                cid
            );
            return !r.empty();
        } catch (...) {
            return false;
        }
    }
    //查询学生的课程表
    bool findSchedule(
        std::string sid,
        std::vector<std::shared_ptr<Course>>& courses) {

        try {
            pqxx::nontransaction tx(_conn);
            //查询学生已选的课程ID列表
            auto r = tx.exec_params(
                "SELECT course_id FROM enrollment WHERE student_id=$1",
                sid
            );

            //遍历查询结果
            for (auto row : r) {
                std::shared_ptr<Course> c;
                std::string courseId = row[0].as<std::string>();

                //根据课程ID查询课程详细信息
                CourseDatabase::singleton().findCourseById(courseId, c);

                //如果课程存在则添加到输出向量
                if (c) courses.push_back(c);
            }
            return true;
        } catch (...) {
            return false;
        }
    }
};
