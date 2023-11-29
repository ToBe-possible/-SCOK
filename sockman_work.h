#ifndef SOCKMAN_WORK_H
#define SOCKMAN_WORK_H

#include <QMainWindow>
#include <QDebug>
#include <QtDebug>
#include "sock_sql.h"
#include "ui_sockman_work.h"
#include <QSqlRecord>
//#include "addsock.h"
//#include "modsock.h"
#include "selectsock.h"
namespace Ui {
    class sockman_work;
}

class sockman_work : public QMainWindow
{
    Q_OBJECT

public:
    explicit sockman_work(QWidget *parent = nullptr);
    ~sockman_work();

    Ui::sockman_work * getWidget();



    void init();//窗口初始化


    void init_font();//字体初始化

    void adaptorTable();//行距自适应

    void setCenter();//居中

    QList<QString> getSelectedData();//获取选中行的内容

    void sortHeader();

public:
    QSqlQuery sql;

signals:
    void sendSelectLineData(const QList<QString> &selectedData);//传递选中行的信号
    void gotoModsock();//进入modsock

public slots:
    void loadTableData();//显示数据库内容

    void loadSearchData();//搜索框模糊

    void receiveSelectData(const QList<QList<QString> >&selectData);//接收筛选信息

    void onHeaderClicked(int Index);

private slots:


    void on_btn_exit_clicked();

    void on_btn_plus_clicked();



    void LoadTaskSlot();


    void on_btn_clear_clicked();
//    void rightClickedSlot(const QPoint);
    void on_btn_mod_clicked();

    void on_btn_excel_clicked();

    void on_btn_select_clicked();

    void on_btn_update_clicked();

    void on_btn_help_clicked();

private:
    Ui::sockman_work *ui;
    QSqlDatabase m_db = sock_sql::getdatabase();


};

#endif // SOCKMAN_WORK_H
