#include "mainwindow.h"
#include <QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.show();

//    Pal.setColor(QPalette::Background, Qt::black);
//    w.setAutoFillBackground(true);
//    w.setPalette(Pal);
//    w.showFullScreen();

    return a.exec();
}
