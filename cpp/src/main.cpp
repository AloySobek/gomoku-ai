#include "include/mainwindow.h"
#include <Game.h>
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Game game;
    MainWindow w(&game, nullptr);
    w.show();
    return a.exec();
}
