// Module
// File: person.cppm    Version: 0.1.0  Lincense: AGPLv3
// Created: JiangFan       2026-01-09 14:52:59
// Description:
// a person class



export module cs:entity.person;

import std;

using std::string;

export class Person
{
public:
    explicit Person(string id, string name);
    string infoId() const;
    string infoName() const;

protected:
    string m_pid;
    string m_pname;
};

Person::Person(string id, string name)
    :m_pid(id), m_pname(name)
{}

string Person::infoId() const
{
    return std::format("{}", m_pid);
}

string Person::infoName() const
{
    return std::format("{}", m_pname);
}
