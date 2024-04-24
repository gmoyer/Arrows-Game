#include <QTimer>

#include "gamewindow.h"
#include "ui_gamewindow.h"

#include "board.h"
#include "player.h"
#include "windialog.h"

GameWindow::GameWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::GameWindow)
{
    ui->setupUi(this);

    //Create board view
    QGraphicsView *view = ui->boardGraphics;
    QRect boardRect = view->contentsRect();
    Board::width = boardRect.width();
    Board::height = boardRect.height();
    Board::rowHeight = Board::height / Board::rows;
    Board::colWidth = Board::width / Board::cols;

    //Create graphics scenes
    scene = new QGraphicsScene;
    view->setScene(scene);
    view->setSceneRect(0,0,Board::width,Board::height);

    //seed random numbers
    srand(static_cast<unsigned>(QTime::currentTime().msec()));

    //Create and add the board to the scene
    board = new Board(scene);


    //Set up the rules
    QGraphicsView *rulesView = ui->rulesGraphics;
    rulesScene = new QGraphicsScene;
    rulesView->setScene(rulesScene);
    rulesView->setSceneRect(0, 0, Board::width/Board::rows, (Board::height/Board::cols) * 4); // 4 = number of squares
    rulesScene->addItem(new Rotate(QPoint(0, 0), RotateDirection::CLOCKWISE));
    rulesScene->addItem(new Rotate(QPoint(0, 1), RotateDirection::COUNTERCLOCKWISE));
    rulesScene->addItem(new Boost(QPoint(0, 2)));
    rulesScene->addItem(new Homing(QPoint(0, 3)));

    showHints = true;
}

GameWindow::~GameWindow()
{
    delete ui;
    delete players[0];
    delete players[1];
    delete board;
    delete scene;
    //delete board;
}

void GameWindow::setPlayers(Player* p[PLAYER_COUNT]) {
    players[0] = p[0];
    players[1] = p[1];
    p[0]->setScene(scene);
    p[1]->setScene(scene);

    p[0]->setColor(QColor(0, 255, 0));
    p[1]->setColor(QColor(0, 0, 255));
}

void GameWindow::spawnArrows(int row, QColor color, Player* player, bool facingUp) {
    for (int i = row; i <= row + 1; i++) {
        for (int j = 0; j < Board::cols; j++) {
            QPoint pos(j, i);
            int r = rand() % 3;
            Direction direction;
            if (facingUp) {
                direction = Direction::UP;
            } else {
                direction = Direction::DOWN;
            }
            Arrow* arrow = new Arrow(player, pos, direction, color);
            player->addArrow(arrow);
            scene->addItem(arrow);

            connect(arrow, &Arrow::arrowClicked, this, &GameWindow::arrowClicked);
            connect(arrow, &Arrow::rerender, this, &GameWindow::rerender);
        }
    }
}

void GameWindow::startGame() {
    spawnArrows(1, QColor(0, 255, 0), players[0], false);
    spawnArrows(7, QColor(0, 0, 255), players[1], true);

    rerender();

    activePlayerNum = 0;
    startTurn();
}

void GameWindow::startTurn() {
    activePlayer = players[activePlayerNum];
    opposingPlayer = players[(activePlayerNum+1) % 2];
    updateLabels();
    activePlayer->updateOpArrows(opposingPlayer->getArrows());

    if (!canPlayerMove(activePlayer))
        endGame(opposingPlayer);
}

bool GameWindow::canPlayerMove(Player *player) {
    for (Arrow* arrow : player->getArrows()) {
        if (canMove(arrow))
            return true;
    }
    return false;
}

void GameWindow::arrowClicked(Arrow *arrow) {
    Player* arrowPlayer = arrow->getPlayer();
    if (arrowPlayer == activePlayer) {
        if (canMove(arrow)) {
            moveArrow(arrow);
            if (arrow->getBoosted() && canMove(arrow)) {
                moveArrow(arrow);
            }
            endTurn();
        }
    }
}

bool GameWindow::canMove(Arrow* arrow) {
    Player* arrowPlayer = arrow->getPlayer();
    ArrowState state = arrow->getState();
    state.move();
    for (Arrow* arrow : arrowPlayer->getArrows()) {
        if (arrow->getPos() == state.pos())
            return false;
    }
    return true;
}

void GameWindow::moveArrow(Arrow* arrow) {
    Player* otherPlayer = opposingPlayer;

    arrow->move();

    for (Arrow* other : otherPlayer->getArrows()) {
        if (arrow->getPos() == other->getPos()) {
            otherPlayer->removeArrow(other);
            activePlayer->updateOpArrows(otherPlayer->getArrows());
            scene->removeItem(other);
            delete other;
        }
    }
}

void GameWindow::endTurn() {

    updateArrows();

    checkWin();

    activePlayerNum++;
    activePlayerNum %= 2;

    rerender();

    startTurn();
}

void GameWindow::updateArrows() {
    for (Arrow* arrow : activePlayer->getArrows()) {
        Event* event = board->getEvent(arrow->getPos());
        if (event != nullptr) {
            event->modifyArrow(arrow);
        }
    }
}

void GameWindow::rerender() {
    scene->update();
}

void GameWindow::updateLabels() {
    //ui->timerLabel->setText(QString("Time left: %1").arg(QString::number(secondsLeft)));
    //ui->diceLabel->setText(QString("Dice: %1").arg(diceRoll == 0 ? "" : QString::number(diceRoll)));
    ui->playerLabel->setText(QString("%1's turn").arg(activePlayer->getName()));
    //ui->rerollButton->setText(QString("Reroll (%1 left)").arg(activePlayer->getRerollCount()));
    //ui->undoButton->setText(QString("Undo (%1 left)").arg(activePlayer->getUndoCount()));

    QString playerColor = activePlayer->getColor().name();
    ui->playerLabel->setStyleSheet(QString("background-color: %1").arg(playerColor));
}

void GameWindow::checkWin() {
    if (players[0]->getArrows().size() == 0)
        endGame(players[1]);
    else if (players[1]->getArrows().size() == 0)
        endGame(players[0]);
}

void GameWindow::endGame(Player *winner) {
    WinDialog dialog(this);
    dialog.setWinner(winner->getName());
    dialog.exec();

    emit gameOver();
}

void GameWindow::on_quitButton_clicked()
{
    int winPlayerNum = (activePlayerNum + 1) % 2;
    endGame(players[winPlayerNum]);
}


void GameWindow::on_hintButton_clicked()
{
    if (showHints) {
        ui->rulesGraphics->hide();
        ui->hint1->hide();
        ui->hint2->hide();
        ui->hint3->hide();
        ui->hint4->hide();
    } else {
        ui->rulesGraphics->show();
        ui->hint1->show();
        ui->hint2->show();
        ui->hint3->show();
        ui->hint4->show();
    }
    showHints = !showHints;
}

