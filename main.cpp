#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
//关闭高分屏幕
#if (QT_VERSION >= QT_VERSION_CHECK(6,0,0))
    QGuiApplication::setHighDpiScaleFactorRoundingPolicy(Qt::HighDpiScaleFactorRoundingPolicy::Floor);
#endif


    //    sock_sql s_sql;

    QApplication a(argc, argv);

    // QFont defaultFont("Microsoft YaHei UI");
    // a.setFont(defaultFont);
    MainWindow w;
    w.show();




    return a.exec();
}
