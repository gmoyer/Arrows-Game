#include "windialog.h"
#include "ui_windialog.h"

WinDialog::WinDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::WinDialog)
{
    ui->setupUi(this);
}

WinDialog::~WinDialog()
{
    delete ui;
}

void WinDialog::setWinner(QString winner) {
    ui->winLabel->setText(QString("%1 wins!").arg(winner));
}
