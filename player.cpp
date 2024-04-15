#include "player.h"
#include "arrow.h"

Player::Player(QString name) : name(name) {
    color = QColor(0, 0, 0);
}

void Player::updateOpArrows(QVector<Arrow *> opArrows) {
    for (Arrow* arrow : arrows) {
        arrow->setOpArrows(opArrows);
    }
}
