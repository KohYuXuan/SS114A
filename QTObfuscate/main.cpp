#include <QtGui/QApplication>
#include "Engine.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    new Engine();
    g_engine->getmainwindow()->show();

    return a.exec();
}
