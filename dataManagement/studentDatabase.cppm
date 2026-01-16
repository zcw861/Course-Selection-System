// Module
// File: studentDatabase.cppm   Version: 0.1.0   License: AGPLv3
// Created: 何志远   3270883879@qq.com   2026-01-16 14:27:57
// Description:
//
export module cs:database.student;

import std;
import :entity.student;


export class StudentDatabase{
public:
    static StudentDatabase singleton();
    bool saveStudent(const Student &student);
    bool updateStudent(const Student &student);
    bool findStudentById(std::string studentId);
private:
    std::vector<std::shared_ptr<Student>> _students;
};
