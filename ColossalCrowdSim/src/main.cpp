#include <QApplication>
#include "MainWindow.h"

int main(int argc, char **argv)
{
    // make a static instance of a QApplication
    QApplication a(argc,argv);
    // create a new MainWindow
    MainWindow w;
    w.show();
    // hand control over to Qt framework
    return a.exec();



}
