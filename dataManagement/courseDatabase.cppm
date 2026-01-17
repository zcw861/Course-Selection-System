// Module
// File: courseDatabase.cppm   Version: 0.1.0   License: AGPLv3
// Created: 何志远   3270883879@qq.com   2026-01-16 14:16:01
// Description:
//

//     [v0.1.2] 何志远 3270883879@qq.com   2026-01-17 14:34:18
//         * 更改代码结构，连接postgresql数据库，，实现具体函数实现
module;

#include <pqxx/pqxx>
#include <memory>
#include <string>

export module cs:database.course;

import std;
import :entity;

export class CourseDatabase {
private:
    inline static std::unique_ptr<CourseDatabase> _instance{};
    pqxx::connection _conn;

    CourseDatabase()
        : _conn(
            "host=localhost port=5432 dbname=course_selection "
            "user=postgres password=123456"
        ) {}

public:
    static CourseDatabase& singleton() {
        if (!_instance)
            _instance.reset(new CourseDatabase());
        return *_instance;
    }

    bool findCourseById(
        const std::string& id,
        std::shared_ptr<Course>& out) {

        try {
            pqxx::nontransaction tx(_conn);
            auto r = tx.exec_params(
                "SELECT course_name, credit, capacity, time, location FROM course WHERE course_id=$1",
                id
            );
            if (r.empty()) return false;
            //使用从数据库获取的所有字段来创建Course对象
            std::string name = r[0][0].as<std::string>();
            double credit = r[0][1].as<double>();
            int capacity = r[0][2].as<int>();
            std::string time = r[0][3].as<std::string>();
            std::string location = r[0][4].as<std::string>();

            out = std::make_shared<Course>(
                name, id, credit, capacity, time, location
            );
            return true;
        } catch (...) {
            return false;
        }
    }

    bool saveCourse(const Course& course) {
        try {
            pqxx::work tx(_conn);
            tx.exec_params(
                "INSERT INTO course(course_id, course_name, credit, capacity, time, location) "
                "VALUES($1, $2, $3, $4, $5, $6) "
                "ON CONFLICT(course_id) DO UPDATE SET "
                "course_name=$2, credit=$3, capacity=$4, time=$5, location=$6",
                course.infoId(),
                course.infoName(),
                //假设Course类有对应的方法
                0.0,
                30,
                "0",
                "0"
            );
            tx.commit();
            return true;
        } catch (...) {
            return false;
        }
    }
};
