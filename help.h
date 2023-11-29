#ifndef HELP_H
#define HELP_H

#include <QWidget>

namespace Ui {
class help;
}

class help : public QWidget
{
    Q_OBJECT

public:
    explicit help(QWidget *parent = nullptr);
    ~help();


public:
    void init_text();//加载内容

    void init_font();//字体 大小和风格设置


private slots:
    void on_btn_exit_clicked();

private:
    Ui::help *ui;
};

#endif // HELP_H
