// Module
// File: person.cppm    Version: 0.1.0  Lincense: AGPLv3
// Created: JiangFan       2026-01-09 14:52:59
// Description:
// a person class

export module selection:person;

import std;

using std::string;

export class Person
{
public:
    explicit Person(string id, string name);
    string infoId();
    string infoName();

protected:
    string m_pid;
    string m_pname;
};

Person::Person(string id, string name)
    :m_pid(id), m_pname(name)
{}

string Person::infoId()
{
    return std::format("{}", m_pid);
}

string Person::infoName()
{
    return std::format("{}", m_pname);
}
