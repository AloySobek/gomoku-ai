#include "Startup.hpp"
#include "ui_Startup.h"

Startup::Startup(QWidget *parent) :
    QDialog(parent)
    , ui(new Ui::Startup)
{
    ui->setupUi(this);
    on_modePvP_toggled(ui->modePvP->isChecked());
}

Startup::~Startup()
{
    delete ui;
}

bool Startup::isPvPMode() {
    return ui->modePvP->isChecked();
}

bool Startup::isPvAIMode() {
    return ui->modePvAI->isChecked();
}

bool Startup::isPlayAsWhite() {
    return ui->playAsWhite->isChecked();
}

bool Startup::isPlayAsBlack() {
    return ui->playAsBlack->isChecked();
}

void Startup::on_modePvP_toggled(bool checked)
{
    ui->playAsBlack->setDisabled(checked);
    ui->playAsWhite->setDisabled(checked);
}
