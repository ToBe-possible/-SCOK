#ifndef USERMAN_WORK_H
#define USERMAN_WORK_H


#include "user_sql.h"
#include <QMainWindow>
#include <QSqlDatabase>



namespace Ui {
class userman_work;
}

class userman_work : public QMainWindow
{
    Q_OBJECT

public:
    explicit userman_work(QWidget *parent = nullptr);
    ~userman_work();



public:
    void init();//窗口初始化

    void init_font();//字体初始化


    void adaptorTable();//行距自适应

    QList<QString> getSelectedData();//获取选中行的内容


    void sortHeader();

signals:
    void sendSelectLineData(const QList<QString> &selectedData);//传递选中行的信号
public slots:
    void loadTableData();//显示数据库内容

    void loadSearchData();//搜索框模糊
private slots:
    void on_btn_excel_clicked();

    void on_btn_exit_clicked();

    void on_btn_plus_clicked();

    void on_btn_mod_clicked();

    void on_btn_update_clicked();

private:
    Ui::userman_work *ui;
    QSqlDatabase m_db = user_sql::getdatabase();
};

#endif // USERMAN_WORK_H
