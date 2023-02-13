#include <QCoreApplication>
#include <QApplication>
#include "server.h"
#include "serverwindow.h"

int SERVER_PORT = 45678;

int main(int argc, char *argv[])
{

    QApplication a(argc, argv);
    ServerWindow w;
    w.show();
    Server server;

    return a.exec();
}

// QCoreApplication a(argc, argv);
