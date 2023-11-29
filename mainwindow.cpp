#include "mainwindow.h"
#include "qmessagebox.h"
#include "ui_mainwindow.h"
#include "signin.h"
#include "sockman_work.h"
#include "boss_select.h"
#include "userman_work.h"
#include <QShortcut>
MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    init_main();



}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::init_main()
{
    //设定字体的风格
    QFont font_man("Microsoft YaHei UI");
    //label
    QFont font_man_name("Microsoft YaHei UI",16);
    //按键
    QFont font_man_lable_idAndName("Microsoft YaHei UI",9);
    ui->man_name->setFont(font_man_name);
    ui->label_id->setFont(font_man_lable_idAndName);
    ui->label_password->setFont(font_man_lable_idAndName);
    ui->btn_login->setFont(font_man);
    ui->btn_signin->setFont(font_man);
    ui->btn_exit->setFont(font_man);
    ui->user_id->setFont(font_man);
    ui->user_pass->setFont(font_man);

    //设置最多能输入多少数字
    ui->user_id->setMaxLength(11);


    //15361320048
    //fucker20030315
    //设置密码框为password模式
    ui->user_pass->setEchoMode(QLineEdit::Password);

    //控制光标就是在输入id的lineEdit里面
    ui->user_id->setFocus();

    //这里创建sockman_work下的mainwindow

    //设置登录快捷键
    //Key_Enter是小键盘的确认键，Key_Return是大键盘的回车键
    QShortcut *shortcut_login = new QShortcut(QKeySequence(Qt::Key_Return|Qt::Key_Enter), this);
    connect(shortcut_login, &QShortcut::activated, [=]() {
        ui->btn_login->click();
    });


    //设置退出快捷键
    QShortcut *shortcut_exit = new QShortcut(QKeySequence(Qt::Key_Escape), this);
    connect(shortcut_exit, &QShortcut::activated, [=]() {
        ui->btn_exit->click();
    });

    sockman_work *sockman = new sockman_work();
    connect(this,&MainWindow::gotoSock,sockman,[=](){
        //关闭登录界面
        this->hide();
        sockman->show();
    });

    boss_select *select = new boss_select();
    userman_work *user = new userman_work();
    connect(this,&MainWindow::gotoSelect,[=](){
        // this->hide();
        select->show();
    });

    connect(select,&boss_select::gotoUser,[=](){
        this->hide();
        select->hide();
        user->show();
    });



}



void MainWindow::on_btn_login_clicked()
{
    /*  不需要密码登录  */
    // emit gotoSock();


    /*  需要密码登录  */
    user_sql *dbptr = new user_sql();
    QString id = ui->user_id->text();
    QString password = ui->user_pass->text();

    if(dbptr->login(id,password)){

        //发送打开注册界面的信号
        emit gotoSock();
    }

    else{
        QMessageBox::information(NULL,"登录失败","您输入的账号不存在\n或着您的号码或密码输错",QMessageBox::Ok);
    }

    delete dbptr;

}


void MainWindow::on_btn_signin_clicked()
{




    /*

        待修改

    QString str_search = QString("SELECT token FROM user WHERE id = '12345' AND password = '12345' ;");
    if (sql.exec(str_search)) {
        QString token = sql.value(0).toString();
        token = user_sql::generateAndSaveTokenForBoss();
        QMessageBox::information(this, "注册令牌", "此注册令牌只有一次使用机会\n若需要重新注册账号则需要重新获取\n令牌号 :"+token, QMessageBox::Ok);
    }
    // if (sql.exec(str_search_boss) && sql.next() ) {//
    //     boss_id = sql.value(0).toString();
    //     boss_password = sql.value(1).toString();
    //     qDebug()<<"this token = " <<sql.value(2).toString();
    // }

    */



    //先判断是不是老板号
    //如果是老板号就用一个QMessagebox来显示令牌号 但是不打开注册界面

    QSqlQuery sql(m_db);

    QString id = ui->user_id->text();
    QString password = ui->user_pass->text();

    QString str_search_boss = QString("SELECT id, password, token FROM user WHERE id = '%1' AND password = '%2' AND token IS NOT NULL;")
                                  .arg(id)
                                  .arg(password);

    QString boss_id, boss_password, boss_token;

    if (sql.exec(str_search_boss)) {
        if (sql.next()) {
            boss_id = sql.value(0).toString();
            boss_password = sql.value(1).toString();
            boss_token = sql.value(2).toString();
            qDebug() << boss_id << boss_password << boss_token;
            if (!boss_token.isNull() && id == boss_id && password == boss_password) {
                emit gotoSelect();
            } else {
                // 打开注册界面的代码
                signin *sign = new signin();
                sign->show();
            }
        } else {
            // 打开注册界面的代码
            signin *sign = new signin();
            sign->show();
        }
        this->hide();
    } else {
        qDebug() << "查询老板号失败";
        close();
        signin *sign = new signin();
        sign->show();
    }
}


void MainWindow::on_btn_exit_clicked()
{
    exit(0);
}




