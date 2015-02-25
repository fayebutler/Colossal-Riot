#include "crowdengine.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CrowdEngine w;
    w.show();

    return a.exec();
}
