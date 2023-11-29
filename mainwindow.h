#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QPushButton>
#include <QMessageBox>
#include <QMainWindow>
#include <QIntValidator>
#include <QLabel>
#include "sockman_work.h"

#include "user_sql.h"
//#include "signin.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE



class sockman_work;
class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();


    void init_main();//初始化登录界面



//    signin *signinWidget = NULL;
signals:
    void gotoSock();
    void gotoSelect();

private slots:

    void on_btn_login_clicked();//登录

    void on_btn_signin_clicked();//注册

    void on_btn_exit_clicked();//退出

//    Ui::MainWindow* getMainWindow();


private:
    Ui::MainWindow *ui;

    QSqlDatabase m_db = user_sql::getdatabase();
};
#endif // MAINWINDOW_H
