#include "mainwindow.h"
#include <QApplication>
Server *Admin;
using namespace std;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}

