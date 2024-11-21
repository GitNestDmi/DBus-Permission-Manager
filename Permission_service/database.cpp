#include "database.h"

#include <QDebug>

IDataBase::~IDataBase() { }
//-------------------------------------------------------------------------------
Database::Database()
    : db_(QSqlDatabase::addDatabase("QSQLITE"))
    , query_(QSqlQuery { db_ })
{
}

//-------------------------------------------------------------------------------
Database::~Database() { closeDatabase(); }
//-------------------------------------------------------------------------------
bool Database::openDatabase(const QString& path)
{
    if (!db_.isValid()) {
        qWarning() << "Error, invalid database";
        return false;
    }

    db_.setDatabaseName(path);
    if (!db_.open()) {
        qWarning() << "Error, the database could not be opened:"
                   << db_.lastError().text();
        return false;
    }

    return true;
}
//-------------------------------------------------------------------------------
void Database::closeDatabase() { db_.close(); }
