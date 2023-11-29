#ifndef MODUSER_H
#define MODUSER_H

#include <QWidget>
#include <QSqlDatabase>
#include "user_sql.h"
namespace Ui {
class modUser;
}

class modUser : public QWidget
{
    Q_OBJECT

public:
    explicit modUser(QWidget *parent = nullptr);
    ~modUser();
public:
    QString NAME;

    void init();

public slots:
    void receiveData(const QList<QString> &selectedData);

private slots:
    void on_btn_add_clicked();

    void on_btn_exit_clicked();

private:
    Ui::modUser *ui;
    QSqlDatabase m_db = user_sql::getdatabase();
};

#endif // MODUSER_H
