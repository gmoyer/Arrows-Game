#ifndef BOARD_H
#define BOARD_H

#include <QColor>
#include <QGraphicsItem>

class Event;
class Chute;
class Ladder;
class Player;

class Board : public QGraphicsItem
{
public:
    static int width;
    static int height;
    static int rowHeight;
    static int colWidth;

    static const int rows{10};
    static const int cols{8};
    static const int squares{rows*cols};

    Board(QGraphicsScene* scene);
    ~Board() {
        //scene handles deleting events
    }

    QRectF boundingRect() const override;
    QPainterPath shape() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    static QPoint toDisplayPos(QPoint pos);
    Event* getEvent(QPoint pos);
    void addEvent(Event* event);
private:
    QColor color;
    QGraphicsScene* scene;
    Event* events[squares];
    int eventsIndex(QPoint pos);
};

#endif // BOARD_H
