// Module
// File: scoreDatabase.cppm   Version: 0.1.0   License: AGPLv3
// Created: 何志远   3270883879@qq.com   2026-01-16 14:20:00
// Description: 成绩数据库操作模块，提供学生课程成绩的增删改查功能

//     [v0.1.2] 何志远 3270883879@qq.com   2026-01-17 15:11:35
//         * 更改代码结构，连接postgresql数据库，实现具体函数实现

export module cs:database.score;

// 导入依赖模块
import <pqxx/pqxx>;        //PostgreSQL C++客户端库
import std;
import :entity;
import :database.course;
import :database.student;

//成绩数据库操作类
export class ScoreDatabase {
private:
    inline static std::unique_ptr<ScoreDatabase> _instance{};    //静态单例实例指针
    pqxx::connection _conn;    //PostgreSQL数据库连接对象
    ScoreDatabase()
        : _conn(
            "host=localhost port=5432 dbname=course_selection "
            "user=postgres password=123456"  //初始化数据库连接参数
        ) {}

public:
    static ScoreDatabase& singleton() {
        if (!_instance) _instance.reset(new ScoreDatabase());   //如果实例不存在则创建新实例
        return *_instance;
    }
    bool saveScore(const Score& s) {
        try {
            pqxx::work tx(_conn);
            //执行UPSERT操作（存在则更新，不存在则插入）
            //联合主键：course_id 和 student_id
            tx.exec_params(
                "INSERT INTO score(course_id, student_id, grade) "
                "VALUES($1, $2, $3) "
                "ON CONFLICT(course_id, student_id) DO UPDATE SET grade=$3",
                s.infoCourse(),
                s.infoStudentId(),
                s.infoGrade()
            );
            tx.commit();
            return true;
        } catch (...) {
            //返回操作失败
            return false;
        }
    }

    //删除成绩记录
    bool deleteScore(std::string scoreId) {
        //查找分隔符位置
        auto p = scoreId.find(':');
        //检查scoreId格式是否正确（应包含分隔符）
        if (p == std::string::npos) return false;
        try {
            pqxx::work tx(_conn);
            //根据课程ID和学生ID删除成绩记录
            tx.exec_params(
                "DELETE FROM score WHERE course_id=$1 AND student_id=$2",
                scoreId.substr(0, p),
                scoreId.substr(p + 1)
            );
            tx.commit();
            return true;
        } catch (...) {
            return false;
        }
    }
    //     [v0.1.2] 何志远 3270883879@qq.com   2026-01-18 16:24:15
    //         * 返回指定学生的所有成绩更适合日常查询
    //根据学生ID查询该学生的所有成绩
    bool findScoresByStudentId(const std::string& studentId,
                              std::vector<std::shared_ptr<Score>>& scores) {
        try {
            pqxx::work tx(_conn);
            //查询指定学生的所有成绩记录
            pqxx::result r = tx.exec_params(
                "SELECT course_id, grade "
                "FROM score "  //此处为"score"表，与上面的方法保持一致
                "WHERE student_id = $1",
                studentId
            );
            scores.clear();  //清空输出向量
            //遍历查询结果
            for (const auto& row : r) {
                std::string courseId = row["course_id"].as<std::string>();
                double grade = row["grade"].as<double>();
                std::shared_ptr<Course> course;
                if (!CourseDatabase::singleton().findCourseById(courseId, course))
                    //     [v0.1.2] 何志远 3270883879@qq.com   2026-01-18 16:30:29
                    //         * 增加检查课程和学生是否存在
                    continue; //如果课程不存在，跳过此记录
                std::shared_ptr<Student> student;
                if (!StudentDatabase::singleton().findStudentById(studentId, student))
                    continue; //如果学生不存在，跳过此记录
                //使用weak_ptr避免循环引用
                auto score = std::make_shared<Score>(
                    std::weak_ptr<Course>(course),
                    std::weak_ptr<Student>(student),
                    grade
                );
                scores.push_back(score);   //添加到输出向量
            }
            //提交事务，虽然work析构时也会自动提交，但显式调用更明确
            tx.commit();
            return true;
        }
        catch (const std::exception& e) {
            //捕获标准异常，输出详细错误信息
            std::cerr << "[ScoreDatabase] findScoresByStudentId failed: "
                      << e.what() << std::endl;
            return false;
        }
    }
};
