// Module
// File: secretaryDatabase.cppm   Version: 0.1.0   License: AGPLv3
// Created: 何志远   3270883879@qq.com   2026-01-16 14:24:06
// Description:
//
//     [v0.1.2] 何志远 3270883879@qq.com   2026-01-17 15:42:41
//         * 更改代码结构，连接postgresql数据库，实现具体函数实现
module;

#include <pqxx/pqxx>
#include <memory>
#include <string>

export module cs:database.secretary;

import std;
import :entity;

export class SecretaryDatabase {
private:
    inline static std::unique_ptr<SecretaryDatabase> _instance{};
    pqxx::connection _conn;

    SecretaryDatabase()
        : _conn(
            "host=localhost port=5432 dbname=course_selection "
            "user=postgres password=123456"
        ) {}

public:
    static SecretaryDatabase& singleton() {
        if (!_instance)
            _instance.reset(new SecretaryDatabase());
        return *_instance;
    }

    bool findSecretaryById(
        const std::string& id,
        std::shared_ptr<Secretary>& out) {

        try {
            pqxx::nontransaction tx(_conn);
            auto r = tx.exec_params(
                "SELECT secretary_name FROM secretary WHERE secretary_id=$1",
                id
            );
            if (r.empty()) return false;

            out = std::make_shared<Secretary>(
                id, r[0][0].as<std::string>()
            );
            return true;
        } catch (...) {
            return false;
        }
    }

    bool saveSecretary(const Secretary& secretary) {
        try {
            pqxx::work tx(_conn);
            tx.exec_params(
                "INSERT INTO secretary(secretary_id, secretary_name) "
                "VALUES($1, $2) "
                "ON CONFLICT(secretary_id) DO UPDATE SET secretary_name=$2",
                secretary.infoId(),
                secretary.infoName()
            );
            tx.commit();
            return true;
        } catch (...) {
            return false;
        }
    }
};
