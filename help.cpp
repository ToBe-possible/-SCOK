#include "help.h"
#include "ui_help.h"

#include <QFileDialog>
#include <QFile>
#include <QString>
#include <QFont>
help::help(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::help)
{
    ui->setupUi(this);
    init_text();
}

help::~help()
{
    delete ui;
}

void help::init_text()
{


    //设定字体的风格
    QFont font_man("Microsoft YaHei UI");
    this->setFont(font_man);
    // QString path=QFileDialog::getOpenFileName(this,"open","../");//获取读入文件的路径表示可
        //以读入所有类型的文
        //件，也可以指定文件类型
    // QString FilePath = "help.txt";
    auto FilePath = QCoreApplication::applicationDirPath() + "\\help.txt";
    if(FilePath.isEmpty()==false)//如果非空
    {
        QFile file(FilePath);//创建文件对象
        //只读方式,默认只读UTF-8
        bool isok=file.open(QIODevice::ReadOnly);//以只读的方式打开
        if(isok)
        {
            //读文件,一次性读完
            // QByteArray array=file.readAll();//因为这个方法返回的是一个字节数组
            //一行一行读
            QByteArray array;
            while(file.atEnd()==false)
            {
                array+=file.readLine();

            }
            //显示到编辑区
            ui->textEdit->setText(array);
            //文本居中
            //下面这一行只有第一行是居中的
            // ui->textEdit->setAlignment(Qt::AlignCenter);
            ui->textEdit->document()->setDefaultTextOption(QTextOption(Qt::AlignHCenter));
        }
        file.close();//关闭文件
    }

    // 设置为不可写
    ui->textEdit->setReadOnly(true);
    init_font();

}

void help::init_font()
{
    //设定字体的风格
    QFont font_man("Microsoft YaHei UI");
    //label
    QFont font_man_name("Microsoft YaHei UI",14);
    //按键
    QFont font_man_lable_idAndName("Microsoft YaHei UI",9);

    ui->btn_exit->setFont(font_man);
    ui->textEdit->setFont(font_man_name);
}

void help::on_btn_exit_clicked()
{
    this->hide();
}

