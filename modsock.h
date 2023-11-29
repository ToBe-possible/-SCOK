#ifndef MODSOCK_H
#define MODSOCK_H

#include <QWidget>
#include "sock_sql.h"
#include "sockman_work.h"
namespace Ui {
class modsock;
}

class modsock : public QWidget
{
    Q_OBJECT

public:
    explicit modsock(QWidget *parent = nullptr);
    ~modsock();

    void initMod(); //初始化Mod ui界面

    void updateTotal();//更新line_total的值

    bool checkStrValidator();//检查填写字符的合法性

    bool checkNumValidator();//检查数量的合法性

    QString NAME;
signals:
    void gobackSock();//点击返回的时候发送返回sockman信号


public slots:
    void receiveData(const QList<QString> &selectedData);//获取sockman传递的选中行内容显示到ui上

private slots:
    void on_btn_plus_clicked();

    void on_btn_del_clicked();

    void on_btn_mod_clicked();

    void on_btn_exit_clicked();

private:
    Ui::modsock *ui;
    QSqlDatabase m_db = sock_sql::getdatabase();
};

#endif // MODSOCK_H
