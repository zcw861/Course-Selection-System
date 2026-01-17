// Module
// File: scoreDatabase.cppm   Version: 0.1.0   License: AGPLv3
// Created: 何志远   3270883879@qq.com   2026-01-16 14:20:00
// Description:
//
//     [v0.1.2] 何志远 3270883879@qq.com   2026-01-17 15:11:35
//         * 更改代码结构，连接postgresql数据库，实现具体函数实现
module;

#include <pqxx/pqxx>
#include <memory>
#include <string>

export module cs:database.score;

import std;
import :entity;
import :database.course;
import :database.student;

export class ScoreDatabase {
private:
    inline static std::unique_ptr<ScoreDatabase> _instance{};
    pqxx::connection _conn;

    ScoreDatabase()
        : _conn(
            "host=localhost port=5432 dbname=course_selection "
            "user=postgres password=123456"
        ) {}

public:
    static ScoreDatabase& singleton() {
        if (!_instance) _instance.reset(new ScoreDatabase());
        return *_instance;
    }

    bool saveScore(const Score& s) {
        try {
            pqxx::work tx(_conn);
            tx.exec_params(
                "INSERT INTO score(course_id,student_id,grade) "
                "VALUES($1,$2,$3) "
                "ON CONFLICT(course_id,student_id) DO UPDATE SET grade=$3",
                s.infoCourse(), s.infoStudentId(), s.infoGrade()
            );
            tx.commit();
            return true;
        } catch (...) { return false; }
    }

    bool deleteScore(std::string scoreId) {
        auto p = scoreId.find(':');
        if (p == std::string::npos) return false;

        try {
            pqxx::work tx(_conn);
            tx.exec_params(
                "DELETE FROM score WHERE course_id=$1 AND student_id=$2",
                scoreId.substr(0, p),
                scoreId.substr(p + 1)
            );
            tx.commit();
            return true;
        } catch (...) { return false; }
    }

    bool findScoreById(std::string scoreId,
                       std::shared_ptr<Score>& out) {

        auto p = scoreId.find(':');
        if (p == std::string::npos) return false;

        try {
            pqxx::nontransaction tx(_conn);
            auto r = tx.exec_params(
                "SELECT grade FROM score WHERE course_id=$1 AND student_id=$2",
                scoreId.substr(0, p),
                scoreId.substr(p + 1)
            );
            if (r.empty()) return false;

            std::shared_ptr<Course> c;
            std::shared_ptr<Student> s;
            CourseDatabase::singleton()
                .findCourseById(scoreId.substr(0, p), c);
            StudentDatabase::singleton()
                .findStudentById(scoreId.substr(p + 1), s);

            out = std::make_shared<Score>(std::weak_ptr<Course>(c), std::weak_ptr<Student>(s), r[0][0].as<double>());
            return true;
        } catch (...) { return false; }
    }
};
