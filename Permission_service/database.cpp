#include "database.h"
#include <QDebug>

IDataBase::~IDataBase() {}
//-------------------------------------------------------------------------------
Database::Database()
    : m_db(QSqlDatabase::addDatabase("QSQLITE")), query(QSqlQuery{m_db}) {}

//-------------------------------------------------------------------------------
Database::~Database() {
  closeDatabase();
}
//-------------------------------------------------------------------------------
bool Database::openDatabase(const QString& path) {
  if (!m_db.isValid()) {
    qWarning() << "Error, invalid database";
    return false;
  }

  m_db.setDatabaseName(path);
  if (!m_db.open()) {
    qWarning() << "Error, the database could not be opened:"
               << m_db.lastError().text();
    return false;
  }

  return true;
}
//-------------------------------------------------------------------------------
void Database::closeDatabase() {
  m_db.close();
}
//-------------------------------------------------------------------------------
// const QSqlQuery Database::getQuery() { }
