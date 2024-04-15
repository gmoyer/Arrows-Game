#ifndef EVENT_H
#define EVENT_H

#include <QColor>
#include <QGraphicsItem>
#include "arrow.h"

enum RotateDirection {
    CLOCKWISE,
    COUNTERCLOCKWISE
};

class Board;
class Arrow;

class Event : public QGraphicsItem
{
protected:
    QColor color;

    QPoint pos;
public:
    Event(QPoint p);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    QPoint getPos() {return pos;}
    virtual void modifyArrow(Arrow* arrow) = 0;
};

class Rotate : public Event {
private:
    RotateDirection direction;
public:
    Rotate(QPoint p, RotateDirection d);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    void modifyArrow(Arrow* arrow) override;
};

class Boost : public Event {
public:
    Boost(QPoint p);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    void modifyArrow(Arrow* arrow) override;
};

class Homing : public Event {
public:
    Homing(QPoint p);

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    void modifyArrow(Arrow* arrow) override;
};

#endif // EVENT_H
