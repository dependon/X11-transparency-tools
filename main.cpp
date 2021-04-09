#include "mainwindow.h"
#include <QApplication>
#include "trayicon.h"
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
//    MainWindow w ;
//    w.show();

    TrayIcon icon ;
    icon.show();
    return a.exec();
}
