// Module
// File: studentDatabase.cppm   Version: 0.1.0   License: AGPLv3
// Created: 何志远   3270883879@qq.com   2026-01-16 16:07:57
// Description:
//
//     [v0.1.2] 何志远 3270883879@qq.com   2026-01-17 16:36:40
//         * 更改代码结构，连接postgresql数据库，数据类基本实现

export module cs:database.student;

import  <pqxx/pqxx>;
import std;
import :entity;
import :database.course;

export class StudentDatabase {
private:
    inline static std::unique_ptr<StudentDatabase> _instance{};
    pqxx::connection _conn;

    StudentDatabase()
        : _conn(
            "host=localhost port=5432 dbname=course_selection "
            "user=postgres password=123456"
        ) {}

public:
    static StudentDatabase& singleton() {
        if (!_instance) _instance.reset(new StudentDatabase());
        return *_instance;
    }

    bool saveStudent(const Student& s) {
        try {
            pqxx::work tx(_conn);
            tx.exec_params(
                "INSERT INTO student(student_id,student_name) "
                "VALUES($1,$2) "
                "ON CONFLICT(student_id) DO UPDATE SET student_name=$2",
                s.infoId(), s.infoName()
            );
            tx.commit();
            return true;
        } catch (...) { return false; }
    }

    bool findStudentById(std::string id, std::shared_ptr<Student>& out) {
        try {
            pqxx::nontransaction tx(_conn);
            auto r = tx.exec_params(
                "SELECT student_name FROM student WHERE student_id=$1", id
            );
            if (r.empty()) return false;
            out = std::make_shared<Student>(id, r[0][0].as<std::string>());
            return true;
        } catch (...) { return false; }
    }

    bool saveEnrollment(std::string sid, std::string cid) {
        try {
            pqxx::work tx(_conn);
            tx.exec_params(
                "INSERT INTO enrollment(student_id,course_id) VALUES($1,$2)",
                sid, cid
            );
            tx.commit();
            return true;
        } catch (...) { return false; }
    }

    bool removeEnrollment(std::string sid, std::string cid) {
        try {
            pqxx::work tx(_conn);
            tx.exec_params(
                "DELETE FROM enrollment WHERE student_id=$1 AND course_id=$2",
                sid, cid
            );
            tx.commit();
            return true;
        } catch (...) { return false; }
    }

    bool isEnrolled(std::string sid, std::string cid) {
        try {
            pqxx::nontransaction tx(_conn);
            auto r = tx.exec_params(
                "SELECT 1 FROM enrollment WHERE student_id=$1 AND course_id=$2",
                sid, cid
            );
            return !r.empty();
        } catch (...) { return false; }
    }

    bool findSchedule(
        std::string sid,
        std::vector<std::shared_ptr<Course>>& courses) {

        try {
            pqxx::nontransaction tx(_conn);
            auto r = tx.exec_params(
                "SELECT course_id FROM enrollment WHERE student_id=$1", sid
            );

            for (auto row : r) {
                std::shared_ptr<Course> c;
                CourseDatabase::singleton()
                    .findCourseById(row[0].as<std::string>(), c);
                if (c) courses.push_back(c);
            }
            return true;
        } catch (...) { return false; }
    }
};
