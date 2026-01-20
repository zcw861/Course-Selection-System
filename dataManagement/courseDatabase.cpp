module cs:database.course;

#include <pqxx/pqxx>
#include <string>

CourseDatabase::CourseDatabase() = default;
CourseDatabase::~CourseDatabase() = default;

void CourseDatabase::addCourse(int id, const std::string& name) {
    pqxx::connection conn("dbname=test user=test password=test");
    pqxx::work txn(conn);

    txn.exec_params(
        "INSERT INTO course(id, name) VALUES ($1,$2)",
        id, name
    );

    txn.commit();
}
