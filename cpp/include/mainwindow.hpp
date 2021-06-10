#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QMainWindow>
#include <QGraphicsScene>
#include <QJsonDocument>
#include <QJsonObject>
#include <QDebug>
#include <QUrl>
#include <QPixmap>

#include "Token.hpp"
#include "Scene.hpp"
#include "game.hpp"


QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(Game *game, QWidget *parent = nullptr);
    ~MainWindow() override;
    void SetAiTitle();
    Scene *scene;
    Game *game;

private:
    Ui::MainWindow *ui;
    bool isDevMode();
public slots:
    void onActionExit();
    void onActionRestart();
    void onActionLoad();
    void onActionSave();
    void onActionDevMode();
    void onActionShowMask();
    void onActionPvPMode();
    void onActionShowFreeFour();
    void onActionShowFreeTow();
    void onActionShowFreeThree();
    void reset();
};

#endif // MAINWINDOW_H
