#include "moduser.h"
#include "ui_moduser.h"

#include <QIntValidator>
#include <QMessageBox>

modUser::modUser(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::modUser)
{
    ui->setupUi(this);
    init();
}

modUser::~modUser()
{
    delete ui;
}

void modUser::init()
{
    //设定字体的风格
    QFont font_man("Microsoft YaHei UI");
    this->setFont(font_man);

    //label
    QFont font_man_name("Microsoft YaHei UI",16);
    //按键
    QFont font_man_lable_idAndName("Microsoft YaHei UI",9);

    this->setFont(font_man);

    QIntValidator *validator = new QIntValidator(this);
    ui->line_id->setValidator(validator);
}

void modUser::receiveData(const QList<QString> &selectedData)
{
    NAME = selectedData[0];
    ui->line_name->setText(selectedData[0]);
    ui->line_id->setText(selectedData[1]);
    ui->line_password->setText(selectedData[2]);
}

void modUser::on_btn_add_clicked()
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
    QString str_update = QString("UPDATE user SET name = '%1',id = '%2',password = '%3',token = '%4' WHERE name = '%5'")
                                .arg(name)
                                .arg(id)
                                .arg(password)
                                .arg(aut)
                                .arg(NAME);
    if(sql.exec(str_update)){
         QMessageBox::information(this, "", "修改成功", QMessageBox::Yes);
        this->hide();
    }
    else{
         QMessageBox::information(this, "", "修改失败", QMessageBox::Yes);
    }

}


void modUser::on_btn_exit_clicked()
{
    this->hide();
}

