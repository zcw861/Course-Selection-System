module cs:database.score;

#include <pqxx/pqxx>
#include <string>

ScoreDatabase::ScoreDatabase() = default;
ScoreDatabase::~ScoreDatabase() = default;

void ScoreDatabase::saveScore(int studentId, int courseId, int score) {
    pqxx::connection conn("dbname=test user=test password=test");
    pqxx::work txn(conn);

    txn.exec_params(
        "INSERT INTO score(student_id, course_id, score) VALUES ($1,$2,$3)",
        studentId, courseId, score
    );

    txn.commit();
}
