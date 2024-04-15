#include "homewindow.h"
#include "ui_homewindow.h"

#include <QFile>
#include <QStringList>
#include <QTextStream>

HomeWindow::HomeWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::HomeWindow)
{
    ui->setupUi(this);
}

HomeWindow::~HomeWindow()
{
    delete ui;
}

void HomeWindow::on_startButton_clicked()
{
    QString p1 = ui->player1Name->text();
    QString p2 = ui->player2Name->text();
    if (p1 == "" || p2 == "")
        return;
    Player* players[] = {
        new Player(p1),
        new Player(p2),
    };

    emit startGame(players);
}

