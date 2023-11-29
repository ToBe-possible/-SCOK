#ifndef SOCK_SQL_H
#define SOCK_SQL_H

#include <QSqlDatabase>
#include <QSqlQuery>
#include <qmessagebox.h>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QtSql/QSqlDatabase>
#include <QDebug>
#include <QString>
#include <QtDebug>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <qstring.h>
#include <QString>
#include <QCoreApplication>
#include <QSqlQuery>

class sock_sql
{
public:
    sock_sql();
    ~sock_sql();

    static void connect_db();//连接数据库

    static void close_db();//关闭数据库

    static QSqlDatabase getdatabase();//获取m_db

private:
    static QSqlDatabase m_db ;


private slots:
    bool updateAut();//更新老板号的令牌
};

#endif // SOCK_SQL_H
