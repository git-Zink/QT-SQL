#include <QtGui/QApplication>
#include "database.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Database w;
    w.show();

    return a.exec();
}
