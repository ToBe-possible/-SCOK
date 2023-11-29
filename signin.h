#ifndef SIGNIN_H
#define SIGNIN_H

#include <QWidget>
#include "user_sql.h"
#include <QMessageBox>
#include <qmessagebox.h>
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
#include <QIntValidator>
#include "mainwindow.h"
//#include "ui_mainwindow.h"
namespace Ui {
class signin;
}

class MainWindow;
class signin : public QWidget
{
    Q_OBJECT

public:
    explicit signin(QWidget *parent = nullptr);
    ~signin();
//    MainWindow *mainwindow ;
    MainWindow *mainwindow =new MainWindow();
signals:
    void gobackLogin();
private slots:


    void on_btn_exit_clicked();//退出回退至登录界面

    void on_btn_signin_clicked();//注册

private:
    Ui::signin *ui;

};

#endif // SIGNIN_H
