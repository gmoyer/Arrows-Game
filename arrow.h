#ifndef ARROW_H
#define ARROW_H

#include <QObject>
#include <QGraphicsItem>
#include "player.h"
#include "event.h"

class Player;

enum Direction {
    UP,
    UP_LEFT,
    LEFT,
    DOWN_LEFT,
    DOWN,
    DOWN_RIGHT,
    RIGHT,
    UP_RIGHT
};

class ArrowState {
private:
    QPoint pos_;
    int rotation_;
    QVector<Arrow*> opArrows_;
    bool homing_;
public:
    ArrowState();

    QPoint pos() const { return pos_; }
    int x() const { return pos_.x(); }
    int y() const { return pos_.y(); }
    int rotation() const { return rotation_; }
    bool homing() const { return homing_; }

    void setPos(QPoint pos) { pos_ = pos; }
    void setX(int x) { pos_.setX(x); }
    void setY(int y) { pos_.setY(y); }
    void setOpArrows(QVector<Arrow*> opArrows) { opArrows_ = opArrows; }
    void setHoming(bool homing) { homing_ = homing; }
    void setRotation(int rotation);

    void changeRotation(int prevCardinal, int newCardinal);
    void move();
};

class Arrow : public QObject, public QGraphicsItem {

    Q_OBJECT

private:
    ArrowState state;
    QColor color;
    Player* player;
    bool boosted;
    bool hovered;

    void setupDirection(Direction direction);
public:
    Arrow(Player* player, QPoint pos, Direction direction, QColor color);

    QRectF boundingRect() const override;
    QPainterPath shape() const override;

    void paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) override;

    void paintState(QPainter *painter, ArrowState s, QColor c);

    Player* getPlayer() {return player;}

    ArrowState getState() { return state; }
    QPoint getPos() { return state.pos(); }
    void move();
    void rotate(int r);

    void setBoosted(bool b) { boosted = b; }
    bool getBoosted() { return boosted; }

    void setHoming(bool h) { state.setHoming(h); }
    bool getHoming() { return state.homing(); }

    void setOpArrows(QVector<Arrow*> opArrows) { state.setOpArrows(opArrows); }
signals:
    void arrowClicked(Arrow* arrow);
    void rerender();
protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
};

#endif // ARROW_H
