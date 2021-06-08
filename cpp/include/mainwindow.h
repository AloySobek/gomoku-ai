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
#include "Token.h"
#include "Scene.h"
#include <Game.h>


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

private:
    Ui::MainWindow *ui;
    Scene *scene;
    Game *game;
    bool isDevMode();
public slots:
    void onActionExit();
    void onActionRestart();
    void onActionLoad();
    void onActionSave();
    void onActionDevMode();
    void SetAiTitle();
};

#endif // MAINWINDOW_H
