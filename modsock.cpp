#include "modsock.h"
#include "ui_modsock.h"
#include <QDebug>
#include <QMessageBox>
#include <QSqlQuery>
#include "sockman_work.h"
modsock::modsock(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::modsock)
{
    ui->setupUi(this);
//    qDebug()<<"show";
//    sockman_work *sock = new sockman_work();

    initMod();


}

modsock::~modsock()
{
    delete ui;
}

void modsock::initMod()
{
//    qDebug()<<"enter initMod";

    //设定字体的风格
    QFont font_man("Microsoft YaHei UI");
    this->setFont(font_man);

    ui->line_total->setText("0");

    ui->combo_position->setEditable(true); // 设置为可编辑

    //设置dateTimeEdit为当前的时间
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());

    //设置只能输入数字
    QIntValidator *validator = new QIntValidator(this);
    ui->line_num->setValidator(validator);
    ui->line_price->setValidator(validator);


    //设置不能输入total price
    ui->line_total->setReadOnly(true);



    //设置num的输入范围 1~9999
    ui->line_num->setValidator(new QIntValidator(1,9999,this));

    //设置price的输入范围 0.01~9999
    //(0.01~9999)小数点后一位
    ui->line_price->setValidator(new QDoubleValidator(0.01,9999,1,this));



    //更新总价的信号槽
    connect(ui->line_price, &QLineEdit::textChanged, this, &modsock::updateTotal);

    QSqlQuery sql(m_db);
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
}

void modsock::updateTotal()
{

    double num = ui->line_num->text().toDouble();
    double price = ui->line_price->text().toDouble();
    double total = num * price;

    ui->line_total->setText(QString::number(total));

}

bool modsock::checkStrValidator()
{
    /*
        如果返回true 说明字符串是填写的不合法的
            这时on_btn_add_clicked()就不会执行
        如果返回false 说明字符串是合法的
    */
    //设定字体的风格
    QFont font_man("Microsoft YaHei UI");
    this->setFont(font_man);
    QString name = ui->line_name->text();
    QString time = ui->dateTimeEdit->text();
    QString position = ui->combo_position->currentText();
    QString num = ui->line_num->text();
    QString price = ui->line_price->text();
    QString total = ui->line_total->text();

    //创建QList存储字符方便判断字符合法性
    QList<QString>li={name,time,position,num,price,total};

    for(const QString &str:li){
        if(str.isEmpty()){
            QMessageBox::information(this,"输入错误","填写的信息不能为空",QMessageBox::Yes);
            return true;
        }
    }

    //检查num和price的合法性
    //使用正则表达式检查 字符串 是否包含多个前导零
    QRegularExpression leadingZerosRegex("^0+");
    if (num.contains(leadingZerosRegex) && price.contains(leadingZerosRegex)) {
        QMessageBox::information(this,"输入错误","您输入的数量和价格都不是合法数字",QMessageBox::Yes);
        return true;
    }
    else if (num.contains(leadingZerosRegex)) {
        QMessageBox::information(this,"输入错误","您输入的数量不是合法数字",QMessageBox::Yes);
        return true;
    }

    // 使用正则表达式检查 price 是否包含多个前导零
    else if (price.contains(leadingZerosRegex)) {
        QMessageBox::information(this,"输入错误","您输入的价格不是合法数字",QMessageBox::Yes);
        return true;
    }

    return false;
}



void modsock::receiveData(const QList<QString> &selectedData)
{
    NAME = selectedData[0];
    ui->line_name->setText(selectedData[0]);
    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());
    ui->combo_position->setCurrentText(selectedData[2]);
    ui->line_num->setText(selectedData[3]);
    ui->line_price->setText(selectedData[4]);
    ui->line_total->setText(selectedData[5]);

}
bool modsock::checkNumValidator()
{
    QString num = ui->line_num->text();
    QRegularExpression leadingZerosRegex("^0+");
    //检查 num 的合法性
    //使用正则表达式检查 字符串 是否包含多个前导零
    if (num.contains(leadingZerosRegex)) {
        QMessageBox::information(this,"输入错误","您输入的数量不是合法数字\n请输入正确的数字",QMessageBox::Yes);
        return true;
    }

    return false;
}

//还需要写一个信号槽，把updatetotal
void modsock::on_btn_plus_clicked()
{
    if(!checkNumValidator()){
        int num = ui->line_num->text().toInt();
        num++;
        ui->line_num->setText(QString::number(num));
    }
}


void modsock::on_btn_del_clicked()
{
    int num = ui->line_num->text().toInt();
    if(num <= 0){
        num = 0;
    }
    else{
        num--;
    }
    ui->line_num->setText(QString::number(num));

}


void modsock::on_btn_mod_clicked()
{
    if(checkStrValidator()){
        return;
    }

    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "请求确认", "你确定要执行这个操作吗?", QMessageBox::Yes | QMessageBox::No);

    QSqlQuery sql("",m_db);

    if(reply == QMessageBox::Yes){
        QString name = ui->line_name->text();
        QString time = ui->dateTimeEdit->text();
        QString position = ui->combo_position->currentText();
        QString num = ui->line_num->text();
        QString price = ui->line_price->text();
        QString total = ui->line_total->text();
        // qDebug()<<name<<" "<<time<<" "<<position<<" "<<num<<" "<<price<<" "<<total;

        QString str_search = QString("");
        QString str_update = QString("UPDATE sock SET name = '%1',time='%2', position='%3', num='%4', price='%5', total='%6' WHERE name='%7'")
                                 .arg(name)
                                 .arg(time)
                                 .arg(position)
                                 .arg(num)
                                 .arg(price)
                                 .arg(total)
                                 .arg(NAME);
        str_update = str_update.arg(time, position, num, price, total, name);
        if(sql.exec(str_update)){
            QMessageBox::information(this, "", "修改成功", QMessageBox::Yes);
            this->hide();
            // sockman_work *work = new sockman_work();
            // work->show();
        }
        else {
            QMessageBox::information(this, "", "修改失败", QMessageBox::Yes);
            qDebug()<<"update fail"<<sql.lastError();

        }


    }
    else{
        // 用户点击了 No 按钮，执行相应的操作
        qDebug() << "用户点击了 No 按钮";
    }
}


void modsock::on_btn_exit_clicked()
{
    this->close();
    // sockman_work *work = new sockman_work();
    // work->show();
}

