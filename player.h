#ifndef PLAYER_H
#define PLAYER_H

#include "arrow.h"

#include <QObject>
#include <QGraphicsScene>

class Arrow;

class Player
{
private:
    QGraphicsScene* scene;
    QVector<Arrow*> arrows;
    QColor color;
    QString name;
public:
    Player(QString name);
    ~Player() {
        //scene handles deleting arrows
    }

    void setScene(QGraphicsScene* s) {scene = s;}
    void setColor(QColor c) {color = c;}
    void addArrow(Arrow* arrow) {arrows.push_back(arrow);}
    void removeArrow(Arrow* arrow) {arrows.removeOne(arrow);}

    void updateOpArrows(QVector<Arrow*> opArrows);

    QVector<Arrow*> getArrows() {return arrows;}

    QString getName() { return name; }
    QColor getColor() { return color; }

};

#endif // PLAYER_H
