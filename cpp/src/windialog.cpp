#include "windialog.hpp"
#include "ui_windialog.h"

WinDialog::WinDialog(QWidget *parent) :
    QDialog(parent, Qt::FramelessWindowHint),
    ui(new Ui::WinDialog)
{
    ui->setupUi(this);
    setAttribute(Qt::WA_TranslucentBackground);
}

WinDialog::~WinDialog()
{
    delete ui;
}

void WinDialog::asBlackWin() {
    ui->resultFrame->setStyleSheet(
"background-color: rgba(46, 52, 54, 50);"
    );
    ui->resultText->setStyleSheet(
"color: rgb(211, 215, 207);"
    );
    ui->resultText->setText("Black Win");
}

void WinDialog::asWhiteWin() {
    ui->resultFrame->setStyleSheet(
            "background-color: rgba(211, 215, 207, 50);"
    );
    ui->resultText->setStyleSheet(
            "color: rgb(46, 52, 54);"
    );
    ui->resultText->setText("White Win");
}

void WinDialog::asDraw() {
    ui->resultFrame->setStyleSheet(
            "background-color: rgba(211, 215, 207, 50);"
    );
    ui->resultText->setStyleSheet(
            "color: rgb(46, 52, 54);"
    );
    ui->resultText->setText("Draw");
}
