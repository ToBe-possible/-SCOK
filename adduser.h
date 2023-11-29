#ifndef ADDUSER_H
#define ADDUSER_H

#include <QWidget>
#include <QSqlDatabase>
#include "user_sql.h"
namespace Ui {
class addUser;
}

class addUser : public QWidget
{
    Q_OBJECT

public:
    explicit addUser(QWidget *parent = nullptr);
    ~addUser();


public:
    void init();


private slots:
    void on_btn_add_clicked();

    void on_btn_exit_clicked();

private:
    Ui::addUser *ui;
    QSqlDatabase m_db = user_sql::getdatabase();
};

#endif // ADDUSER_H
