#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;

    QIcon iconS(":/Picture/img/B-icon.png");
    w.setWindowIcon(iconS);
    a.setWindowIcon(iconS);
    w.setWindowTitle("Bookmark-Sniper");
    w.show();

    return a.exec();
}
