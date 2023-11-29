#ifndef USER_SQL_H
#define USER_SQL_H

#include <QString>
#include <QSqlDatabase>
#include <QDebug>
#include <QCoreApplication>
#include <QSqlError>
#include <QSqlQuery>
#include <QRandomGenerator>

struct UserInfo{
    int id;
    QString password;
    QString aut;

};
class user_sql
{
public:
    user_sql();
    ~user_sql();
    static void connect_db();//连接数据库

    static void close_db();//关闭数据库

    bool login(QString,QString);//登录

    static QSqlDatabase getdatabase();//获取m_db

    static QString generateToken();//获取新的随机数字

    static QString generateAndSaveTokenForBoss();//更新老板的令牌

//    bool addUser(UserInfo);//注册



private:

    static QSqlDatabase m_db ;

};

#endif // USER_SQL_H
