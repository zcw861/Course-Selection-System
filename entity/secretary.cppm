// Module
// File: secretary.cppm    Version: 0.1.0  Lincense: AGPLv3
// Created: JiangFan       2026-01-09 18:35:26
// Description:
// a secretary class

export module cs:entity.secretary;

import std;
import :entity.person;

using std::string;
using std::print;
using std::shared_ptr;
using std::cin;

class Course;

export class Secretary : public Person
{
public:
    explicit Secretary(string id, string name);
    //排课
    bool schedulingCourse(std::weak_ptr<Course> course);
};

Secretary::Secretary(string id, string name)
    : Person(id, name)
{}

