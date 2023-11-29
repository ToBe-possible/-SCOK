#include "sock_sql.h"

#include <QDir>

QSqlDatabase sock_sql::m_db = QSqlDatabase::addDatabase("QSQLITE");

sock_sql::sock_sql()
{
    connect_db();

}

sock_sql::~sock_sql()
{
    // close_db();
}

void sock_sql::connect_db()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    auto FilePath = QCoreApplication::applicationDirPath() + "\\SOCK.db";
    m_db.setDatabaseName(FilePath);

    if (!m_db.isOpen()) {
        if (m_db.open()) {
            qDebug() << "数据库已连接";
        } else {
            qDebug() << "打开数据库失败" << m_db.lastError().text();
        }
    } else {
        qDebug() << "数据库已经打开，无需再次连接";
    }
}

void sock_sql::close_db()
{
    if (sock_sql::m_db.isValid() && sock_sql::m_db.isOpen()) {
        sock_sql::m_db.close();
        qDebug() << "关闭数据库连接";
    }
}

QSqlDatabase sock_sql::getdatabase()
{
    return m_db;
}
