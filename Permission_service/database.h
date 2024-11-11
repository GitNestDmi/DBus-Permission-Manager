#ifndef DATABASE_H
#define DATABASE_H

#include <QDebug>
#include <QObject>
#include <QSharedPointer>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>

class Database : public QObject {
  Q_OBJECT
 public:
  explicit Database(QObject* parent = nullptr);
  ~Database();

  bool openDatabase(const QString& path);
  void closeDatabase();
  QSqlQuery* getQuery() { return &query; }

 private:
  QSqlDatabase m_db;
  QSqlQuery query;
};

#endif  // DATABASE_H
