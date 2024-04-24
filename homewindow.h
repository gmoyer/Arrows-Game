#ifndef HOMEWINDOW_H
#define HOMEWINDOW_H

#include <QWidget>
#include <fstream>
#include <sstream>
#include <QAbstractTableModel>
#include "player.h"

#define PLAYER_COUNT 2

namespace Ui {
class HomeWindow;
}

class HomeWindow : public QWidget
{
    Q_OBJECT

public:
    explicit HomeWindow(QWidget *parent = nullptr);
    ~HomeWindow();

private slots:
    void on_startButton_clicked();

signals:
    void startGame(Player* players[]);

private:
    Ui::HomeWindow *ui;
};

#endif // HOMEWINDOW_H
