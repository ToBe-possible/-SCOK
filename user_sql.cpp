#include "user_sql.h"

QSqlDatabase user_sql::m_db = QSqlDatabase::addDatabase("QSQLITE");

//显式构造函数
user_sql::user_sql()
{
    //先将m_db连接数据库
    connect_db();

}


//显式析构函数 关闭m_db数据库
user_sql::~user_sql(){
    // m_db.close();
}


//链接数据库函数

void user_sql::connect_db()
{
    m_db = QSqlDatabase::addDatabase("QSQLITE");
    auto FilePath = QCoreApplication::applicationDirPath() + "\\SOCK.db";
    // auto FilePath = "SOCK.db";
    m_db.setDatabaseName(FilePath);
    qDebug()<<"路径名称为"<<QCoreApplication::applicationDirPath() + "\\SOCK.db";
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

void user_sql::close_db()
{
    if (user_sql::m_db.isValid() && user_sql::m_db.isOpen()) {
        user_sql::m_db.close();
        qDebug() << "关闭数据库连接";
    }
}

// void user_sql::connect_db()
// {
//     m_db = QSqlDatabase::addDatabase("QSQLITE");
//     auto FilePath = QCoreApplication::applicationDirPath()+"\\SOCK.db";
//     m_db.setDatabaseName(FilePath);

//     // qDebug()<<QCoreApplication::applicationFilePath();

//     if (m_db.isOpen()) {
//         qDebug() << "数据库已连接，无需再次连接";
//         return;
//     }
//     else{
//         qDebug()<<"user db fail";

//     }


// }


//提供其他文件使用m_db
QSqlDatabase user_sql::getdatabase()
{
    return m_db;
}


//获取随机数
QString user_sql::generateToken() {
    // 生成一个五位随机数
    int randomNumber = QRandomGenerator::global()->bounded(10000, 99999);

    // 将随机数转换为字符串
    return QString::number(randomNumber);
}

//更新老板号令牌
QString user_sql::generateAndSaveTokenForBoss()
{
    QString token = generateToken();

    // 打印当前老板号令牌
    qDebug()<<"boss's token = "<<token<<"now";


    QSqlQuery sql("", m_db);

    QString str = QString("UPDATE user SET token = '%1' WHERE name = '杨文谦'").arg(token);

    if(sql.exec(str)){
        // qDebug() << "Update successful" ;
    }
    else{
        qDebug() << "Update failed" << sql.lastError();
    }
    return token;
}


//登录
bool user_sql::login(QString id,QString password)
{
    QSqlQuery sql(m_db);
    QString str = QString("select * from user where id= '%1' and password = '%2'").arg(id).arg(password);

    if(!sql.exec(str)){
        qDebug()<<"login Fail"<<sql.lastError().text();
        return false;
    }

    if(sql.next()){
        qDebug()<<"login successful"<<sql.lastError();
        return true;
    }
    else {
        return false;
    }


}




