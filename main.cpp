#include "mainwindow.h"

#include <QApplication>
#include <cstdio>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    w.setWindowFlags(w.windowFlags() | Qt::WindowStaysOnTopHint);
    w.setWindowOpacity(0.5);
    w.setFixedSize(100, 50);
    w.move(0, 0);


    w.show();
    return a.exec();
}
