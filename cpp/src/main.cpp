#include <QApplication>

#include "mainwindow.hpp"
#include "game.hpp"

int main(int argc, char *argv[])
{
    Game game;

    QApplication a(argc, argv);
    MainWindow w(&game, nullptr);
    w.show();
    return a.exec();
}
