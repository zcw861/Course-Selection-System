module cs:database.secretary;

#include <pqxx/pqxx>
#include <string>

SecretaryDatabase::SecretaryDatabase() = default;
SecretaryDatabase::~SecretaryDatabase() = default;

void SecretaryDatabase::addSecretary(int id, const std::string& name) {
    pqxx::connection conn("dbname=test user=test password=test");
    pqxx::work txn(conn);

    txn.exec_params(
        "INSERT INTO secretary(id, name) VALUES ($1,$2)",
        id, name
    );

    txn.commit();
}
