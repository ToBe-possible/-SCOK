#include "selectsock.h"
#include "ui_selectsock.h"
#include "sockman_work.h"
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QFont>
#include <QShortcut>

selectsock::selectsock(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::selectsock)
{
    ui->setupUi(this);
    init_select();

}

selectsock::~selectsock()
{
    delete ui;
}

void selectsock::init_select()
{

    //初始化字体
    init_font();



    //sql创建
    QSqlQuery sql(m_db);



    /*
        初始化combobox
        把数据库里的位置导入进去
    */
    ui->combo_position->addItem("空");
    QString str_select_position = QString("SELECT DISTINCT position FROM sock");
    if(sql.exec(str_select_position)){
        qDebug()<<"选择sql执行成功";
        while(sql.next()){
            QString pos_name = sql.value(0).toString();

            qDebug()<<pos_name;

            ui->combo_position->addItem(pos_name);
        }
    }
    else{
        qDebug()<<"select fasle"<<sql.lastError();
    }

    /*
        把 minPrice 和 maxPrice 的输入合法性
        只能输入数字，如果只写了一边要怎么办，min默认为0，max默认为9999
    */

    //设置只能输入数字
    QIntValidator *validator = new QIntValidator(this);
    ui->line_maxPrice->setValidator(validator);
    ui->line_minPrice->setValidator(validator);

    //设置minPrice maxPrice 的输入范围 0.01~9999
    //(0.01~9999)小数点后一位
    ui->line_maxPrice->setValidator(new QDoubleValidator(0.01,9999,1,this));
    ui->line_minPrice->setValidator(new QDoubleValidator(0.01,9999,1,this));





}

void selectsock::init_font()
{
    //设置光标在minPrice上
    ui->line_minPrice->setFocus();
    //设定字体的风格
    QFont font_man("Microsoft YaHei UI");

    this->setFont(font_man);


    //label
    QFont font_man_name("Microsoft YaHei UI",16);
    //按键
    QFont font_man_lable_idAndName("Microsoft YaHei UI",9);

    ui->btn_select->setFont(font_man);
    ui->btn_exit->setFont(font_man);

    ui->label_price->setFont(font_man);
    ui->label_other->setFont(font_man);
    ui->label_position->setFont(font_man);

    ui->combo_position->setFont(font_man);

    ui->line_maxPrice->setFont(font_man);
    ui->line_minPrice->setFont(font_man);


    //设置登录快捷键
    //Key_Enter是小键盘的确认键，Key_Return是大键盘的回车键
    QShortcut *shortcut_select = new QShortcut(QKeySequence(Qt::Key_Return|Qt::Key_Enter), this);
    connect(shortcut_select, &QShortcut::activated, [=]() {
        ui->btn_select->click();
    });


    //设置退出快捷键
    QShortcut *shortcut_exit = new QShortcut(QKeySequence(Qt::Key_Escape), this);
    connect(shortcut_exit, &QShortcut::activated, [=]() {
        ui->btn_exit->click();
    });
}

bool selectsock::checkNumValidator()
{
    QString maxPrice = ui->line_maxPrice->text();
    QString minPrice = ui->line_minPrice->text();

    QRegularExpression leadingZerosRegex("^0+");
    //检查 num 的合法性
    //使用正则表达式检查 字符串 是否包含多个前导零
    if (maxPrice.contains(leadingZerosRegex) && minPrice.contains(leadingZerosRegex)) {
        QMessageBox::information(this,"输入错误","您输入的最小值和最大值不是合法数字\n请输入正确的数字",QMessageBox::Yes);
        return true;
    }
    else if (minPrice.contains(leadingZerosRegex)) {
        QMessageBox::information(this,"输入错误","您输入的最小值不是合法数字\n请输入正确的数字",QMessageBox::Yes);
        return true;
    }
    else if (maxPrice.contains(leadingZerosRegex)) {
        QMessageBox::information(this,"输入错误","您输入的最大值不是合法数字\n请输入正确的数字",QMessageBox::Yes);
        return true;
    }

    return false;
}



void selectsock::on_btn_exit_clicked()
{
    this->hide();
}


void selectsock::on_btn_select_clicked()
{

    if(checkNumValidator()){
        return ;
    }

    //获取筛选信息
    QString position = ui->combo_position->currentText();
    QString minPrice = ui->line_minPrice->text();
    QString maxPrice = ui->line_maxPrice->text();


    QString str_select;
    if(minPrice.isEmpty()){
        minPrice = "0";
    }
    if(maxPrice.isEmpty()){
        maxPrice = "9999";
    }
    if(position == "空"){
        str_select = QString ("SELECT * FROM sock WHERE price BETWEEN %1 AND %2; ").arg(minPrice).arg(maxPrice);
    }
    else{
        str_select = QString ("SELECT * FROM sock WHERE position = '%1' AND price BETWEEN %2 AND %3 ; ").arg(position).arg(minPrice).arg(maxPrice);
    }
    getSelect(str_select);
}

void selectsock::getSelect(QString str)
{
    QList<QString> select;
    QList<QList<QString> >list;
    if(sql.exec(str)){
        select.clear();
        QString temp;
        while (sql.next()) {
            QList<QString> select;
            select.append(sql.value("name").toString());
            select.append(sql.value("time").toString());
            select.append(sql.value("position").toString());
            select.append(sql.value("num").toString());
            select.append(sql.value("price").toString());
            select.append(sql.value("total").toString());
            list.append(select);
        }
        emit sendSelectData(list);
        this->hide();
    }
    else{
        qDebug()<<"select fail"<<sql.lastError();
    }
}

