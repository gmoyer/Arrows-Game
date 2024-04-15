#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "homewindow.h"
#include "gamewindow.h"
#include "player.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    void startGame(Player** players);
    void gameOver();
    void newGameWindow();
    void newHomeWindow();

private slots:
signals:
private:
    Ui::MainWindow *ui;
    HomeWindow* homeWindow;
    GameWindow* gameWindow;
};
#endif // MAINWINDOW_H
