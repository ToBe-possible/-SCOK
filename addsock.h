#ifndef ADDSOCK_H
#define ADDSOCK_H

#include <QWidget>
#include <QMessageBox>
#include <QRegularExpression>
#include <QIntValidator>
#include <QCloseEvent>
//#include "addsock.h"
#include "sock_sql.h"
#include <QSqlDatabase>

namespace Ui {
    class addSock;
}

class addSock : public QWidget
{
    Q_OBJECT

public:
    explicit addSock(QWidget *parent = nullptr);
    ~addSock();

    void initSock();//初始化ui

    void updateTotal();//更新line_total的值.

    bool checkStrValidator();//检查填写字符的合法性

    bool checkNumValidator();//检查数量的合法性


private slots:

    void on_btn_add_clicked();



    void on_btn_exit_clicked();

    void on_btn_plus_clicked();

    void on_btn_del_clicked();

private:
    Ui::addSock *ui;


    QSqlDatabase m_db = sock_sql::getdatabase();
};

#endif // ADDSOCK_H
