#ifndef SELECTSOCK_H
#define SELECTSOCK_H

#include <QWidget>
#include "sock_sql.h"
namespace Ui {
class selectsock;
}

class selectsock : public QWidget
{
    Q_OBJECT

public:
    explicit selectsock(QWidget *parent = nullptr);
    ~selectsock();

    void init_select();//初始化选择界面
    void init_font();//字体初始化

    bool checkNumValidator();//检查数量的合法性

    // QList<QList<QString> > getSelect(QString);//执行sql语句获取内容
    void getSelect(QString);//执行sql语句获取内容

signals:
    void sendSelectData(const QList<QList<QString> >&selectData);

private slots:
    void on_btn_exit_clicked();

    void on_btn_select_clicked();

private:
    Ui::selectsock *ui;

    QSqlDatabase m_db = sock_sql::getdatabase();
    QSqlQuery sql;
};

#endif // SELECTSOCK_H
