#ifndef DATABASE_H
#define DATABASE_H

#include <QDebug>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

struct IDataBase {
    virtual ~IDataBase();
    virtual bool openDatabase(const QString& path) = 0;
    virtual void closeDatabase() = 0;
    virtual QSqlQuery* getQuery() = 0;
};

struct Database : public IDataBase {
    Database();
    ~Database();

    bool openDatabase(const QString& path);
    void closeDatabase();
    QSqlQuery* getQuery() { return &query_; }

private:
    QSqlDatabase db_;
    QSqlQuery query_;
};

#endif // DATABASE_H
