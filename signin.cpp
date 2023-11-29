#include "signin.h"
#include "ui_signin.h"
#include "mainwindow.h"
#include "user_sql.h"


#include <QFont>
signin::signin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::signin)
{
    ui->setupUi(this);

    QMessageBox::information(NULL,"注册标准","请按照以下要求填写用户信息: \n请填写您的真实姓名\n用户名为您的手机电话号码，密码长度要6位以上",QMessageBox::Ok);
    QFont font_man("Microsoft YaHei UI");
    this->setFont(font_man);
    MainWindow *mainwindow_ptr =new MainWindow();

    connect(this,&signin::gobackLogin,mainwindow_ptr,[=](){
        this->hide();
        mainwindow_ptr->show();
    });


}

signin::~signin()
{
    delete ui;
}




void signin::on_btn_exit_clicked()
{

    emit gobackLogin();
}


void signin::on_btn_signin_clicked()
{
    //连接数据库
    user_sql *dbptr = new user_sql();
    QSqlDatabase db = dbptr->getdatabase();
    QSqlQuery sql("",db);
    //检查是否打开数据库
    if(db.open()){
        qDebug()<<"signin connect successful"<<sql.lastError();
    }
    else{
        qDebug()<<"sigin connect fail"<<sql.lastError();
    }


    //获取控件里的字符串
    //但是没有对数据进行标准判定
    QString name = ui->line_name->text();
    QString id = ui->line_id->text();
    QString password = ui->line_pass->text();
    QString scan_token = ui->line_token->text();
//    qDebug()<<name<<" "<<id<<" "<<password<<" "<<scan_token;

    //检查输入信息是否为空
    if(name == ""||id == ""||password == "" || scan_token == ""){
        QMessageBox::information(this, "注册失败", "失败原因: 注册信息不能为空", QMessageBox::Ok);
        return;
    }



    //检查是不是已经注册过了该账号
    //如果已经注册过了就弹窗显示并退出这个函数
    QString str_find = QString("SELECT id FROM user WHERE id = %1").arg(id);//123456
    if(sql.exec(str_find) && sql.next()){
        qDebug() << "Error in checking ID:" << sql.lastError();
        QMessageBox::information(this, "注册失败", "失败原因: 账号已注册", QMessageBox::Ok);
        return;
    }
    else{
        qDebug() << "Successful in checking ID:" << sql.lastError();
    }


    //15361320048
    //fucker20030315
    //检索老板号下的令牌
//    QString str = "SELECT token FROM user WHERE id = '15361320048' AND password = 'fucker20030315'";
    QString str = QString("SELECT token FROM user WHERE token = '%1'").arg(scan_token);
    // qDebug()<<"你好";
//    if (sql.exec(str)) {
    if (sql.exec(str) && sql.next()) {
        // QString token = sql.value(0).toString();
        //匹配token
        //比较用户输入的令牌和老板号令牌是否相同
        //如果相同就将账号添加到数据库里面

        QString str_insert = QString("insert into user values('%1','%2','%3',NULL)")
                                 .arg(name)
                                 .arg(id)
                                 .arg(password);
        //注册成功
        if(sql.exec(str_insert)){
            QMessageBox::information(this, "", "注册成功", QMessageBox::Ok);//123456
            dbptr->generateAndSaveTokenForBoss();

            //关闭当前注册界面打开登录界面
            // close();
//                this->hide();
            this->destroy();
            // mainwindow->show();
            sockman_work *work = new sockman_work();
            work->show();
        }

        //注册失败
        else{
            qDebug()<<sql.lastError();
            QMessageBox::information(this, "", "注册失败", QMessageBox::Ok);
        }
    }
    else {
        QMessageBox::information(this, "注册失败", "失败原因: 令牌输入错误", QMessageBox::Ok);
    }

}

