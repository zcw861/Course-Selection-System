// Module
// File: teacherDatabase.cppm   Version: 0.1.0   License: AGPLv3
// Created: 何志远   3270883879@qq.com   2026-01-16 14:31:46
// Description:
//
export module cs:database.teacher;

import std;
import :entity.teacher;


export class TeacherDatabase{
public:
    static TeacherDatabase singleton();
    bool saveTeacher(const Student &teacher);
    bool updateTeacher(const Student &teacher);
    bool findTeacherById(std::string teacherId);
private:
    std::vector<std::shared_ptr<Student>> _teachers;
};
