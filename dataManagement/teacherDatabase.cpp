module cs:database.teacher;

#include <pqxx/pqxx>
#include <string>

TeacherDatabase::TeacherDatabase() = default;
TeacherDatabase::~TeacherDatabase() = default;

void TeacherDatabase::addTeacher(int id, const std::string& name) {
    pqxx::connection conn("dbname=test user=test password=test");
    pqxx::work txn(conn);

    txn.exec_params(
        "INSERT INTO teacher(id, name) VALUES ($1,$2)",
        id, name
    );

    txn.commit();
}
