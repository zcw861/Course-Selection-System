// Module
// File: secretary.cppm    Version: 0.1.0  Lincense: AGPLv3
// Created: JiangFan       2026-01-09 18:35:26
// Description:
// a secretary class

export module selection:secretary;

import std;

using std::string;

export class Secretary : public Person
{
public:
    explicit Secretary(string id, string name);
    bool schedulingCourse(string courseId, string teacherId);
};

Secretary::Secretary(string id, string name)
    : Person(id, name)
{}

bool Secretary::schedulingCourse(string courseId, string teacherId)
{
}
