#include "ui_addsock.h"
#include "addsock.h"
#include "ui_addsock.h"
#include "sock_sql.h"
#include "sockman_work.h"


addSock::addSock(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::addSock)
{
    ui->setupUi(this);
    initSock();

}

addSock::~addSock()
{
    delete ui;
}


void addSock::initSock()
{
    //设定字体的风格
    QFont font_man("Microsoft YaHei UI");
    this->setFont(font_man);
    //sql创建
    QSqlQuery sql("",m_db);

    /*
        初始化combobox
        把数据库里的位置导入进去
    */
    // QString str_select_position = QString("SELECT DISTINCT position FROM sock");
    // if(sql.exec(str_select_position)){
    //     qDebug()<<"选择sql执行成功";
    //     while(sql.next()){
    //         QString pos_name = sql.value(0).toString();

    //         qDebug()<<pos_name;

    //         ui->line_pos->addItem(pos_name);
    //     }
    // }
    // else{
    //     qDebug()<<"select fasle"<<sql.lastError();
    // }
    ui->line_total->setText("0");



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
    connect(ui->line_price, &QLineEdit::textChanged, this, &addSock::updateTotal);



}




void addSock::updateTotal()
{
    double num = ui->line_num->text().toDouble();
    double price = ui->line_price->text().toDouble();
    double total = num * price;

    ui->line_total->setText(QString::number(total));
}


bool addSock::checkStrValidator()
{
    /*
        如果返回true 说明字符串是填写的不合法的
            这时on_btn_add_clicked()就不会执行
        如果返回false 说明字符串是合法的
    */
    QString name = ui->line_name->text();
    QString time = ui->dateTimeEdit->text();
    QString position = ui->line_pos->text();
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









void addSock::on_btn_add_clicked()
{
    //检查字符合法性
    if(checkStrValidator()){
        return;
    }
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "请求确认", "你确定要执行这个操作吗?", QMessageBox::Yes | QMessageBox::No);

    sock_sql::connect_db();
    QSqlQuery sql("",m_db);
    if (reply == QMessageBox::Yes) {
        // 用户点击了 Yes 按钮，执行相应的操作
        qDebug() << "用户点击了 Yes 按钮";
        QString name = ui->line_name->text();
        QString time = ui->dateTimeEdit->text();
        // QString position = ui->combo_pos->currentText();
        QString position = ui->line_pos->text();
        QString num = ui->line_num->text();
        QString price = ui->line_price->text();
        QString total = ui->line_total->text();
        QString str_insert = QString("INSERT INTO sock (name, time, position, num, price, total) "
                                      "VALUES ('%1', '%2', '%3', %4, %5, %6);")
                                  .arg(name)
                                  .arg(time)
                                  .arg(position)
                                  .arg(num)
                                  .arg(price)
                                  .arg(total);
        if(sql.exec(str_insert)){
//            qDebug() << "insert successful";
            QMessageBox::information(this, "", "添加成功", QMessageBox::Yes);
            this->hide();
            // sockman_work *work = new sockman_work();
//            qDebug() << "Object created: " << work;
//            work->clear_table();
//            work->loadTableData();

//            delete work;
            // work->show();



        }
        else{
            QMessageBox::information(this, "", "添加失败", QMessageBox::Yes);
            qDebug()<<"insert fail"<<sql.lastError();

        }

    }
    else {
        // 用户点击了 No 按钮，执行相应的操作
        qDebug() << "用户点击了 No 按钮";

    }
    // sock_sql::close_db();

}








void addSock::on_btn_exit_clicked()
{
    this->hide();
    // this->close();
    // sockman_work *work = new sockman_work();
    // work->show();
}




bool addSock::checkNumValidator()
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


void addSock::on_btn_plus_clicked()
{
    if(!checkNumValidator()){
        int num = ui->line_num->text().toInt();
        num++;
        ui->line_num->setText(QString::number(num));
    }
}


void addSock::on_btn_del_clicked()
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
