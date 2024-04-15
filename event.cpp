#include <cstdlib>
#include <ctime>
#include <cmath>
#include <iostream>
#include <QtWidgets>

#include "event.h"
#include "board.h"


Event::Event(QPoint p) {
    color = QColor(0, 0, 0);
    pos = p;
}



QRectF Event::boundingRect() const {
    return QRectF(0,0, Board::width, Board::height);
}

QPainterPath Event::shape() const
{
    QPainterPath path;
    path.addRect(0,0,0,0);
    return path;
}


Rotate::Rotate(QPoint p, RotateDirection d) : Event(p) {
    color = QColor(175, 175, 175);
    direction = d;
}

void Rotate::modifyArrow(Arrow *arrow) {
    if (direction == RotateDirection::CLOCKWISE) {
        arrow->rotate(45);
    } else {
        arrow->rotate(-45);
    }
}

void Rotate::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) {
    Q_UNUSED(widget);
    Q_UNUSED(item);


    static const QPoint counterclockwise[3] = {
        QPoint(-5, 5),
        QPoint(5, 10),
        QPoint(-5, 15)
    };
    static const QPoint clockwise[3] = {
        QPoint(5, 5),
        QPoint(-5, 10),
        QPoint(5, 15)
    };

    painter->save();

    painter->setBrush(QBrush(QColor(255, 255, 255)));
    QPoint paintPos = Board::toDisplayPos(pos);
    painter->translate(paintPos);

    painter->drawEllipse(QPoint(0, 0), 10, 10);

    painter->setBrush(QBrush(color));

    for (int i = 0; i < 4; i++) {
        painter->rotate(90);
        if (direction == RotateDirection::CLOCKWISE)
            painter->drawPolygon(clockwise, 3);
        else
            painter->drawPolygon(counterclockwise, 3);
    }

    painter->restore();
}

Boost::Boost(QPoint p) : Event(p) {
    color = QColor(175, 175, 175);
}

void Boost::modifyArrow(Arrow *arrow) {
    arrow->setBoosted(true);
}

void Boost::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) {
    Q_UNUSED(widget);
    Q_UNUSED(item);

    painter->save();

    painter->setBrush(QBrush(color));
    QPoint paintPos = Board::toDisplayPos(pos);
    painter->translate(paintPos);

    painter->drawEllipse(QPoint(0, 0), 10, 10);

    painter->restore();
}





Homing::Homing(QPoint p) : Event(p) {
    color = QColor(175, 175, 175);
}

void Homing::modifyArrow(Arrow *arrow) {
    arrow->setHoming(true);
}

void Homing::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) {
    Q_UNUSED(widget);
    Q_UNUSED(item);

    painter->save();

    painter->setBrush(QBrush(color));
    QPoint paintPos = Board::toDisplayPos(pos);
    painter->translate(paintPos);

    painter->drawRect(-2.5, -10, 5, 20);
    painter->drawRect(-10, -2.5, 20, 5);

    painter->restore();
}
