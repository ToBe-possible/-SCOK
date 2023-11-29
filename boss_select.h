#ifndef BOSS_SELECT_H
#define BOSS_SELECT_H

#include <QWidget>

namespace Ui {
class boss_select;
}

class boss_select : public QWidget
{
    Q_OBJECT

public:
    explicit boss_select(QWidget *parent = nullptr);
    ~boss_select();

public:
    void init();//界面初始化





signals:
    void gotoUser();
private slots:
    void on_btn_exit_clicked();

    void on_btn_maketoken_clicked();

    void on_btn_user_clicked();

private:
    Ui::boss_select *ui;
};

#endif // BOSS_SELECT_H
