#ifndef WINDIALOG_H
#define WINDIALOG_H

#include <QDialog>

namespace Ui {
class WinDialog;
}

class WinDialog : public QDialog
{
    Q_OBJECT

public:
    explicit WinDialog(QWidget *parent = nullptr);
    ~WinDialog();
    void asBlackWin();
    void asWhiteWin();
    void asDraw();

private:
    Ui::WinDialog *ui;
};

#endif // WINDIALOG_H
