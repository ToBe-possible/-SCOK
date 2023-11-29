#include "adduser.h"
#include "ui_adduser.h"
#include "user_sql.h"
#include <QFont>
#include <QIntValidator>
#include <QMessageBox>
addUser::addUser(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::addUser)
{
    ui->setupUi(this);
    init();
}

addUser::~addUser()
{
    delete ui;
}

void addUser::init()
{

    //设定字体的风格
    QFont font_man("Microsoft YaHei UI");
    //label
    QFont font_man_name("Microsoft YaHei UI",16);
    //按键
    QFont font_man_lable_idAndName("Microsoft YaHei UI",9);

    this->setFont(font_man);

    QIntValidator *validator = new QIntValidator(this);
    ui->line_id->setValidator(validator);


}

void addUser::on_btn_add_clicked()
{
    QSqlQuery sql("",m_db);
    QString name = ui->line_name->text();
    QString id = ui->line_id->text();
    QString password = ui->line_password->text();
    QString aut;
    if(ui->check_boss->isChecked()&&ui->check_worker->isChecked()){
        QMessageBox::information(this,"","不能同时勾选“管理员”和“员工”");
        return;
    }

    if (ui->check_boss->isChecked()) {
        // 执行相应的操作，因为复选框被勾选了
        aut=user_sql::generateToken();

    } else {
        // 执行相应的操作，因为复选框没有被勾选
        aut="NULL";
    }
    QString str_insert = QString("insert into user values('%1','%2','%3','%4')")
                             .arg(name)
                             .arg(id)
                             .arg(password)
                             .arg(aut);
    if(sql.exec(str_insert)){
        QMessageBox::information(this, "", "添加成功", QMessageBox::Yes);
        this->hide();
    }
    else{
        QMessageBox::information(this, "", "添加失败", QMessageBox::Yes);
    }



}


void addUser::on_btn_exit_clicked()
{
    this->hide();
}

