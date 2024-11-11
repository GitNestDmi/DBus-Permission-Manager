#include "database.h"
#include <QDebug>

Database::Database(QObject* parent)
    : QObject(parent),
      m_db(QSqlDatabase::addDatabase("QSQLITE")),
      query(QSqlQuery{m_db}) {}

//-------------------------------------------------------------------------------
Database::~Database() {
  closeDatabase();
}

//-------------------------------------------------------------------------------
bool Database::openDatabase(const QString& path) {
  if (!m_db.isValid()) {
    qWarning() << "Invalid database";
    return false;
  }

  m_db.setDatabaseName(path);
  if (!m_db.open()) {
    qCritical() << "Failed to open database:" << m_db.lastError().text();
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
