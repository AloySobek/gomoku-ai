#include <QApplication>

#include "mainwindow.hpp"
#include "Startup.hpp"
#include "game.hpp"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Game game;
    MainWindow w(&game, nullptr);
    Startup s(nullptr);
    w.show();
    auto dialogCode = s.exec();
    if(dialogCode == QDialog::Rejected) {
        qDebug() << "QDialog::Rejected";
        return 0;
    }
    if(dialogCode == QDialog::Accepted) {
        qDebug() << "QDialog::Accepted";
        w.scene->pvpMode = s.isPvPMode();
        w.scene->playAs = s.isPlayAsWhite() ? Scene::WHITE : Scene::BLACK;
        w.scene->startGame();
    }
    return a.exec();
}
