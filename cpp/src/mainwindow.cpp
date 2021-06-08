#include <Token.h>
#include <QJsonArray>
#include <Game.h>
#include "include/mainwindow.h"
#include "./ui_mainwindow.h"
#include "Scene.h"

MainWindow::MainWindow(Game *game, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , game(game)
    , scene(new Scene(game, this))
{
    ui->setupUi(this);
    connect(ui->actionExit, SIGNAL(triggered(bool)), this, SLOT(onActionExit()));
    connect(ui->actionRestart, SIGNAL(triggered(bool)), this, SLOT(onActionRestart()));
    connect(ui->actionLoad, SIGNAL(triggered(bool)), this, SLOT(onActionLoad()));
    connect(ui->actionSave, SIGNAL(triggered(bool)), this, SLOT(onActionSave()));
    connect(ui->actionDevMode, SIGNAL(toggled(bool)), this, SLOT(onActionDevMode()));
    connect(ui->actionDevMode, SIGNAL(toggled(bool)), this, SLOT(SetAiTitle()));

    ui->graphicsView->setScene(scene);
    ui->graphicsView->setMouseTracking(true);
    scene->addText("Hello World");
    scene->setSceneRect(ui->graphicsView->rect());
    setWindowTitle("Gomoku");
    SetAiTitle();
}


MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::onActionExit() {
    qDebug() << "onActionExit" << this;
    qApp->quit(); // NOLINT(cppcoreguidelines-pro-type-static-cast-downcast)
}

bool MainWindow::isDevMode() {
    return ui->actionDevMode->isChecked();
}

void MainWindow::onActionRestart() {
    qDebug() << "onActionRestart" << this;
    if (!game)
        return;
    game->reset();
    scene->reset();
}

void MainWindow::onActionLoad() {
    auto f = QFileDialog::getOpenFileName(
        this,
        "Select board saved to .json file!"
    );
    qDebug() << "onActionLoad" << f;
    if (f.isNull())
        return;
    QFile file;
    QString data;
    file.setFileName(f);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    data = file.readAll();
    file.close();
    auto doc = QJsonDocument::fromJson(data.toUtf8());
    if (doc.isNull() || !doc.isArray())
        return;
    auto obj = doc.array();
    if (obj.size() != 19)
        return;
    for (const auto &item : obj) {
        if (!item.isArray())
            return;
        auto row = item.toArray();
        if (row.size() != 19)
            return;
    }
    auto y = 0;
    for (const auto &item : obj)
    {
        auto x = 0;
        auto row = item.toArray();
        for (const auto &val : row)
        {

            auto v = val.toInt(0);
            Scene::TokenDef td(Scene::tokenColorFromInt(v),
                                 QColor(Qt::transparent),
                                 QString());
            scene->setToken(x, y, td);
            x++;
        }
        y++;
    }
    qDebug() << obj;
}

void MainWindow::onActionSave() {
    qDebug() << "onActionSave";
    QJsonArray rows;
    for (int i = 0; i < GSIZE; ++i) {
        QJsonArray row;
        for (int j = 0; j < GSIZE; ++j) {
            auto t = scene->getToken(j, i);
            row.append(Scene::intFromTokenColor(t->def.color));
        }
        rows.append(row);
    }
    QJsonDocument doc;
    doc.setArray(rows);
    QFile file(QFileDialog::getSaveFileName(
            this,
            "File to save board to!"
    ));
    file.open(QFile::WriteOnly | QFile::Text | QFile::Truncate);
    if (!file.isWritable())
        return;
    file.write(doc.toJson());
    file.close();
}

void MainWindow::onActionDevMode() {
    scene->devMode = ui->actionDevMode->isChecked();
    qDebug() << "onActionDevMode" << ui->actionDevMode->isChecked();
    if (scene->devMode)
        setWindowTitle("Gomoku (DEV MODE)");
    else
        setWindowTitle("Gomoku");
}

void MainWindow::SetAiTitle() {
    ui->aiTitle->setText(QString(
                "<html><head/><body>"
                "<h1>Hi there!</h1>"
                "<p>Gomoku will crush you!</p>"
                "<p>Last Move took: %1 sec</p>"
                "<p>Game in dev mode: %2 </p>"
                "</body></html>"
        )
        .arg(
                QString::number(scene->lastPredictedMove.tookSecond, 'g', 4),
                scene->devMode ? "<span style=\" color:#cc0000;\">True</span>" : "False"
        )
    );
}