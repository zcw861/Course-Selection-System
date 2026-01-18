// Module
// File: secretaryDatabase.cppm   Version: 0.1.0   License: AGPLv3
// Created: 何志远   3270883879@qq.com   2026-01-16 14:24:06
// Description: 教学秘书数据库操作模块，提供教学秘书信息的增删改查功能

//     [v0.1.2] 何志远 3270883879@qq.com   2026-01-17 15:42:41
//         * 更改代码结构，连接PostgreSQL数据库，实现具体函数功能
export module cs:database.secretary;
import <pqxx/pqxx>;    //PostgreSQL C++客户端库
import std;
import :entity;

//教学秘书数据库操作类
export class SecretaryDatabase {
private:
    inline static std::unique_ptr<SecretaryDatabase> _instance{};    //静态单例实例指针
    pqxx::connection _conn;    //PostgreSQL数据库连接对象
    SecretaryDatabase()
        : _conn(
            "host=localhost port=5432 dbname=course_selection "
            "user=postgres password=123456"    //初始化数据库连接参数
        ) {}
public:
    static SecretaryDatabase& singleton() {
        //如果实例不存在则创建新实例
        if (!_instance)
            _instance.reset(new SecretaryDatabase());
        return *_instance;
    }

    //根据教务员ID查询教学秘书信息
    bool findSecretaryById(
        const std::string& id,
        std::shared_ptr<Secretary>& out) {
        try {
            //创建非事务性查询
            pqxx::nontransaction tx(_conn);
            //执行参数化查询
            auto r = tx.exec_params(
                "SELECT secretary_name FROM secretary WHERE secretary_id=$1",
                id
            );
            if (r.empty()) return false;   //检查查询结果是否为空
            //创建Secretary对象并赋值给输出参数
            out = std::make_shared<Secretary>(
                id,
                r[0][0].as<std::string>()
            );
            return true;
        } catch (...) {
            return false;
        }
    }

    //保存教学秘书信息
    bool saveSecretary(const Secretary& secretary) {
        try {
            pqxx::work tx(_conn);   //创建事务性操作
            //存在则更新，不存在则插入
            tx.exec_params(
                "INSERT INTO secretary(secretary_id, secretary_name) "
                "VALUES($1, $2) "
                "ON CONFLICT(secretary_id) DO UPDATE SET secretary_name=$2",
                secretary.infoId(),
                secretary.infoName()
            );
            tx.commit();  //提交事务
            return true;
        } catch (...) {
            return false;
        }
    }
};
