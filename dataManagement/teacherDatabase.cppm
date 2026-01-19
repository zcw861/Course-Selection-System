// Module
// File: teacherDatabase.cppm   Version: 0.1.0   License: AGPLv3
// Created: 何志远   3270883879@qq.com   2026-01-16 14:31:46
// Description: 教师数据库操作模块，提供教师信息的增删改查功能

//     [v0.1.2] 何志远 3270883879@qq.com   2026-01-17 17:27:09
//         * 更改代码结构，连接PostgreSQL数据库，实现具体函数功能

module;
#include "pqxx/pqxx"

export module cs:database.teacher;


import std;
import :entity;
//教师数据库操作类
export class TeacherDatabase {
private:
    //静态单例实例指针
    inline static std::unique_ptr<TeacherDatabase> _instance{};
    pqxx::connection _conn;    //PostgreSQL数据库连接对象

    //单例模式
    TeacherDatabase()
        : _conn(
            "host=localhost port=5432 dbname=course_selection "
            "user=postgres password=123456"
        ) {}

public:
    //获取单例实例的静态方法
    static TeacherDatabase& singleton() {
        //如果实例不存在，创建新实例
        if (!_instance)
            _instance.reset(new TeacherDatabase());
        return *_instance;
    }
    //根据教师ID查询教师信息
    bool findTeacherById(
        const std::string& id,
        std::shared_ptr<Teacher>& out) {

        try {
            pqxx::nontransaction tx(_conn);  //创建非事务性查询
            //执行参数化查询，防止SQL注入
            auto r = tx.exec_params(
                "SELECT teacher_name FROM teacher WHERE teacher_id=$1",
                id
            );
            if (r.empty()) return false;
            //创建Teacher对象并赋值给输出参数
            out = std::make_shared<Teacher>(
                id,
                r[0][0].as<std::string>()
            );
            return true;
        } catch (...) {
            return false;
        }
    }

    // 保存教师信息（插入或更新）
    bool saveTeacher(const Teacher& teacher) {
        try {
            //创建事务性操作
            pqxx::work tx(_conn);

            //存在则更新，不存在则插入
            tx.exec_params(
                "INSERT INTO teacher(teacher_id, teacher_name) "
                "VALUES($1, $2) "
                "ON CONFLICT(teacher_id) DO UPDATE SET teacher_name=$2",
                teacher.infoId(),
                teacher.infoName()
            );

            tx.commit();   //提交
            return true;
        } catch (...) {
            return false;
        }
    }
};
