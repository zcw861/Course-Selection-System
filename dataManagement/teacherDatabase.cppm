// Module
// File: teacherDatabase.cppm   Version: 0.1.0   License: AGPLv3
// Created: 何志远   3270883879@qq.com   2026-01-16 14:31:46
// Description:
//
//     [v0.1.2] 何志远 3270883879@qq.com   2026-01-17 17:27:09
//         * 更改代码结构，连接postgresql数据库，实现具体函数实现
module;

#include <pqxx/pqxx>
#include <memory>
#include <string>

export module cs:database.teacher;

import std;
import :entity;

export class TeacherDatabase {
private:
    inline static std::unique_ptr<TeacherDatabase> _instance{};
    pqxx::connection _conn;

    TeacherDatabase()
        : _conn(
            "host=localhost port=5432 dbname=course_selection "
            "user=postgres password=123456"
        ) {}

public:
    static TeacherDatabase& singleton() {
        if (!_instance)
            _instance.reset(new TeacherDatabase());
        return *_instance;
    }

    bool findTeacherById(
        const std::string& id,
        std::shared_ptr<Teacher>& out) {

        try {
            pqxx::nontransaction tx(_conn);
            auto r = tx.exec_params(
                "SELECT teacher_name FROM teacher WHERE teacher_id=$1",
                id
            );
            if (r.empty()) return false;

            out = std::make_shared<Teacher>(
                id, r[0][0].as<std::string>()
            );
            return true;
        } catch (...) {
            return false;
        }
    }

    bool saveTeacher(const Teacher& teacher) {
        try {
            pqxx::work tx(_conn);
            tx.exec_params(
                "INSERT INTO teacher(teacher_id, teacher_name) "
                "VALUES($1, $2) "
                "ON CONFLICT(teacher_id) DO UPDATE SET teacher_name=$2",
                teacher.infoId(),
                teacher.infoName()
            );
            tx.commit();
            return true;
        } catch (...) {
            return false;
        }
    }
};
