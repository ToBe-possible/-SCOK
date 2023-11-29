#include "boss_select.h"
#include "ui_boss_select.h"
#include "user_sql.h"

#include <QMessageBox>
#include <QFont>


boss_select::boss_select(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::boss_select)
{
    ui->setupUi(this);
    init();
}

boss_select::~boss_select()
{
    delete ui;
}

void boss_select::init()
{
    //设定字体的风格
    QFont font_man("Microsoft YaHei UI");
    //label
    QFont font_man_name("Microsoft YaHei UI",16);
    //按键
    QFont font_man_lable_idAndName("Microsoft YaHei UI",9);

    this->setFont(font_man);
}




void boss_select::on_btn_exit_clicked()
{
    this->hide();
}


void boss_select::on_btn_maketoken_clicked()
{
    QString token = user_sql::generateAndSaveTokenForBoss();
    QMessageBox::information(this, "注册令牌", "此注册令牌只有一次使用机会\n若需要重新注册账号则需要重新获取\n令牌号 :" + token, QMessageBox::Ok);

    // this->hide();
}


void boss_select::on_btn_user_clicked()
{
    emit gotoUser();
}

