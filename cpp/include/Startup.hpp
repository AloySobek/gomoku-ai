#ifndef STARTUP_H
#define STARTUP_H

#include <QDialog>
#include <Scene.hpp>

namespace Ui {
class Startup;
}

class Startup : public QDialog
{
    Q_OBJECT

public:
    explicit Startup(QWidget *parent = nullptr);
    ~Startup();
    bool isPvPMode();
    bool isPvAIMode();
    bool isPlayAsWhite();
    bool isPlayAsBlack();

private slots:
    void on_modePvP_toggled(bool checked);

private:
    Ui::Startup *ui;
};

#endif // STARTUP_H
