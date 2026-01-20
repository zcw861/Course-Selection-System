module cs:database.student;

#include <pqxx/pqxx>
#include <string>

StudentDatabase::StudentDatabase() = default;
StudentDatabase::~StudentDatabase() = default;

void StudentDatabase::addStudent(int id, const std::string& name) {
    pqxx::connection conn("dbname=test user=test password=test");
    pqxx::work txn(conn);

    txn.exec_params(
        "INSERT INTO student(id, name) VALUES ($1,$2)",
        id, name
    );

    txn.commit();
}
