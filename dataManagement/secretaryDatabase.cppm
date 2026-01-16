// Module
// File: secretaryDatabase.cppm   Version: 0.1.0   License: AGPLv3
// Created: 何志远   3270883879@qq.com   2026-01-16 14:24:06
// Description:
//
export module cs:database.secretary;

import std;
import :entity.secretary;


export class SecretaryDatabase{
public:
    static SecretaryDatabase singleton();
    bool saveSecretary(const Secretary &secretary);
    bool updateSecretary(const Secretary &secretary);
    bool findSecretaryById(std::string secretaryId);
private:
    std::vector<std::shared_ptr<Secretary>> _secretaries;
};
