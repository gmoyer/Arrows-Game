#include "mainwindow.h"
#include "player.h"
#include "homewindow.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow m;


    m.show();
    return a.exec();
}
