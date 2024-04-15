#ifndef GAMEWINDOW_H
#define GAMEWINDOW_H

#include <iostream>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QDebug>
#include <QTime>
#include <QValidator>
#include <QRegularExpression>
#include <QWidget>
#include "board.h"
#include "player.h"
#include "arrow.h"
#include "event.h"

#define PLAYER_COUNT 2

QT_BEGIN_NAMESPACE
namespace Ui {
class GameWindow;
}
QT_END_NAMESPACE

class GameWindow : public QWidget
{
    Q_OBJECT

public:
    GameWindow(QWidget *parent = nullptr);
    ~GameWindow();
    void setPlayers(Player* players[PLAYER_COUNT]);
    void startGame();

private slots:

    void on_quitButton_clicked();

    void on_hintButton_clicked();

signals:
    void gameOver();

private:
    Ui::GameWindow *ui;
    Board* board;
    QGraphicsScene *scene;
    QGraphicsScene *rulesScene;

    Player* players[PLAYER_COUNT];
    Player* activePlayer;
    Player* opposingPlayer;
    int activePlayerNum;

    bool showHints;

    void spawnArrows(int row, QColor color, Player* player, bool facingUp);
    void startTurn();
    void endTurn();
    void arrowClicked(Arrow* arrow);

    bool canMove(Arrow* arrow);
    void moveArrow(Arrow* arrow);
    void updateArrows();
    void rerender();

    void updateLabels();

    void checkWin();

    void endGame(Player* winner);
};

#endif // GAMEWINDOW_H
