#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <QtWidgets>

#include "board.h"
#include "event.h"
#include "player.h"
#include "arrow.h"

int Board::width = 0;
int Board::height = 0;
int Board::rowHeight = 0;
int Board::colWidth = 0;

Board::Board(QGraphicsScene* scene) : scene(scene) {
    color = QColor(0, 0, 0);

    for (int i = 0; i < squares; i++)
        events[i] = nullptr;

    scene->addItem(this);

    //creating rotations
    for (int i = 0; i < 20; i++) { //some tiles may collide so you end up with less than 20
        QPoint pos = QPoint(rand() % cols, (rand() % (rows-6))+3);
        RotateDirection direction = rand() % 2 == 0 ? RotateDirection::CLOCKWISE : RotateDirection::COUNTERCLOCKWISE;
        addEvent(new Rotate(pos, direction));
    }
    //creating boosters and homing tiles
    for (int i = 0; i < rows; i+= rows-1) { //first and last rows
        for (int j = 1; j < cols - 1; j++) {
            QPoint pos(j, i);
            addEvent(new Boost(pos));
        }
        for (int j = 0; j < cols; j += cols-1) {
            QPoint pos(j, i);
            addEvent(new Homing(pos));
        }
    }
}

QRectF Board::boundingRect() const {
    return QRectF(0,0, Board::width, Board::height);
}

QPainterPath Board::shape() const
{
    QPainterPath path;
    path.addRect(0,0, Board::width, Board::height);
    return path;
}


void Board::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) {
    Q_UNUSED(widget);
    Q_UNUSED(item);

    QBrush b = painter->brush();
    painter->setBrush(QBrush(color));

    for (int i = 1; i < rows; i++) {
        painter->drawLine(0, i * rowHeight, Board::width, i * rowHeight);
    }
    for (int i = 1; i < cols; i++) {
        painter->drawLine(i * colWidth, 0, i * colWidth, Board::height);
    }

    //painter->drawRect(QRect(0, 0, 10, 10));

    //reset brush back to what it was
    painter->setBrush(b);
}

Event* Board::getEvent(QPoint pos) {
    return events[eventsIndex(pos)];
}
void Board::addEvent(Event* event) {
    QPoint pos = event->getPos();
    int i = eventsIndex(pos);
    if (events[i] != nullptr) {
        // remove current event
        scene->removeItem(events[i]);
        delete events[i];
    }
    events[i] = event;
    scene->addItem(event);
}
int Board::eventsIndex(QPoint pos) {
    return (pos.x()*rows)+pos.y();
}


QPoint Board::toDisplayPos(QPoint pos) {
    return QPoint((pos.x() + 0.5) * colWidth, (pos.y() + 0.5) * rowHeight);
}
