// Module
// File: courseDatabase.cppm   Version: 0.1.0   License: AGPLv3
// Created: 何志远   3270883879@qq.com   2026-01-16 14:16:01
// Description:
//

//     [v0.1.2] 何志远 3270883879@qq.com   2026-01-17 14:34:18
//         * 更改代码结构，连接postgresql数据库，，实现具体函数实现

//定义课程数据库模块
export module cs:database.course;

import <pqxx/pqxx>;    //PostgreSQL C++客户端库
import std;
import :entity;

//课程数据库操作类
export class CourseDatabase {
private:
    //     [v0.1.2] 何志远 3270883879@qq.com   2026-01-18 14:50:07
    //         *使用智能指针，更好管理内存生命周期
    inline static std::unique_ptr<CourseDatabase> _instance{};
    pqxx::connection _conn;    //PostgreSQL数据库连接对象
    CourseDatabase()    //单例模式
        : _conn(
            "host=localhost port=5432 dbname=course_selection "
            "user=postgres password=123456"      //初始化数据库连接参数
        ) {}

public:
    static CourseDatabase& singleton() {    //得到单例实例的静态方法
        if (!_instance)
            _instance.reset(new CourseDatabase());  //如果实例不存在就创建新实例
        return *_instance;
    }
    bool findCourseById(const std::string& id, std::shared_ptr<Course>& out) { //根据课程ID查询课程信息
        try {
            pqxx::nontransaction tx(_conn);     //创建非事务性查询
            auto r = tx.exec_params(
                "SELECT course_name, credit, capacity, time, location FROM course WHERE course_id=$1",
                id
            );   //参数化查询，防止SQL注入
            if (r.empty()) return false;     //检查查询结果是否为空
            //从查询结果中提取数据
            std::string name = r[0][0].as<std::string>();
            double credit = r[0][1].as<double>();
            int capacity = r[0][2].as<int>();
            std::string time = r[0][3].as<std::string>();
            std::string location = r[0][4].as<std::string>();
            //创建Course对象并赋值给输出参数
            out = std::make_shared<Course>(
                name, id, credit, capacity, time, location
            );
            return true;
        } catch (...) {
            return false; //错误处理，返回查询失败
        }
    }

    //保存课程信息
    bool saveCourse(const std::shared_ptr<Course>& course) {
        try {
            pqxx::work tx(_conn);
            //执行UPSERT操作，存在则更新，不存在则插入
            tx.exec_params(
                "INSERT INTO course(course_id, course_name, credit, capacity, time, location) "
                "VALUES($1, $2, $3, $4, $5, $6) "
                "ON CONFLICT(course_id) DO UPDATE SET "
                "course_name=$2, credit=$3, capacity=$4, time=$5, location=$6",
                        //     [v0.1.2] 何志远 3270883879@qq.com   2026-01-18 15:20:39
                        //         * 正确调用Course对象的所有信息
                course->infoId(),
                course->infoName(),
                course->infoCredit(),
                course->infoCapacity(),
                course->infoTime(),
                course->infoLocation()
            );
            tx.commit(); //提交事务
            return true;
        } catch (...) {
            return false;   //错误处理，返回查询失败
        }
    }
    //     [v0.1.2] 何志远 3270883879@qq.com   2026-01-18 15:51:15
    //         * 提供了查询所有课程的功能
    bool findAllCourses(std::vector<std::shared_ptr<Course>>& out) {
        try {
            pqxx::nontransaction tx(_conn);
            auto r = tx.exec(
                "SELECT course_id, course_name, credit, capacity, time, location FROM course"
            );  //执行全表查询
            //遍历查询
            for (const auto& row : r) {
                //按列名提取数据
                std::string id = row["course_id"].as<std::string>();
                std::string name = row["course_name"].as<std::string>();
                double credit = row["credit"].as<double>();
                int capacity = row["capacity"].as<int>();
                std::string time = row["time"].as<std::string>();
                std::string location = row["location"].as<std::string>();
                out.emplace_back(std::make_shared<Course>(
                    name, id, credit, capacity, time, location
                ));
            }
            return true;
        } catch (...) {
            return false;
        }
    }
};
