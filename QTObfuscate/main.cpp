#include <QtGui/QApplication>
//#include "mainwindow.h"
#include "Engine.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    new Engine();
    g_engine->getmainwindow()->show();

    return a.exec();

    /*
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    new Engine();

    return a.exec();
    */
}

/*
#include "Engine.h"

int main()
{
	new Engine();


	TreeDemoSample app;
	return app.run();
}
*/
