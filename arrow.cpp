#include "arrow.h"
#include "board.h"

#include <QtWidgets>
#include <QtMath>


ArrowState::ArrowState() {
    pos_ = QPoint(0,0);
    rotation_ = 0;
    homing_ = false;
}

void ArrowState::changeRotation(int prevCardinal, int newCardinal) {
    setRotation((prevCardinal - rotation_) + newCardinal);
}

void ArrowState::setRotation(int rotation) {
    while (rotation < 0)
        rotation += 360;
    rotation %= 360;
    rotation_ = rotation;
}

void ArrowState::move() {
    int vertical = 0;
    int horizontal = 0;

    if (rotation_ > 0 && rotation_ < 180)
        horizontal = 1;
    if (rotation_ > 180 && rotation_ < 360)
        horizontal = -1;
    if (rotation_ < 90 || rotation_ > 270)
        vertical = -1;
    if (rotation_ > 90 && rotation_ < 270)
        vertical = 1;


    if (homing_ && opArrows_.size() > 0) {
        // Find the closest arrow
        Arrow* closestArrow = opArrows_[0];
        int closestDist = -1;
        for (Arrow* arrow : opArrows_) {
            QPoint diff = arrow->getState().pos() - pos_;
            int dist = qAbs(diff.x()) + qAbs(diff.y());
            if (closestDist == -1 || dist < closestDist) {
                closestDist = dist;
                closestArrow = arrow;
            }
        }
        // Determine rotation
        QPoint diff = closestArrow->getState().pos() - pos_;

        // Normalize
        horizontal = qMin(qMax(diff.x(), -1), 1);
        vertical   = qMin(qMax(diff.y(), -1), 1);
    }


    setX(x() + horizontal);
    setY(y() + vertical);

    if (x() < 0) {
        setX(1);
        changeRotation(270, 90);
    }
    if (x() >= Board::cols) {
        setX(Board::cols - 2);
        changeRotation(90, 270);
    }
    if (y() < 0) {
        setY(1);
        changeRotation(0, 180);
    }
    if (y() >= Board::rows) {
        setY(Board::rows - 2);
        changeRotation(180, 0);
    }
}


Arrow::Arrow(Player* player, QPoint pos, Direction direction, QColor color) :
    player(player),
    color(color)
{
    state.setPos(pos);
    setupDirection(direction);
    boosted = false;
    hovered = false;

    setAcceptHoverEvents(true);
}

void Arrow::setupDirection(Direction direction) {
    int rotation = 0;
    switch (direction) {
    case Direction::UP:
        rotation = 0;
        break;
    case Direction::UP_RIGHT:
        rotation = 45;
        break;
    case Direction::RIGHT:
        rotation = 90;
        break;
    case Direction::DOWN_RIGHT:
        rotation = 135;
        break;
    case Direction::DOWN:
        rotation = 180;
        break;
    case Direction::DOWN_LEFT:
        rotation = 225;
        break;
    case Direction::LEFT:
        rotation = 270;
        break;
    case Direction::UP_LEFT:
        rotation = 315;
        break;
    default:
        qDebug() << "No suitable direction found";
    }
    state.setRotation(rotation);
}

QRectF Arrow::boundingRect() const {
    return QRectF(0,0, Board::width, Board::height);
}

QPainterPath Arrow::shape() const {
    QPoint displayPos = Board::toDisplayPos(state.pos());
    displayPos -= QPoint(Board::colWidth / 2, Board::rowHeight / 2);

    QPainterPath path;
    path.addRect(displayPos.x(), displayPos.y(), Board::colWidth, Board::rowHeight);
    return path;
}

void Arrow::paint(QPainter *painter, const QStyleOptionGraphicsItem *item, QWidget *widget) {
    Q_UNUSED(widget);
    Q_UNUSED(item);

    paintState(painter, state, color);

    if (hovered) {
        ArrowState s = state;
        s.move();
        if (boosted)
            s.move();
        paintState(painter, s, QColor(175, 175, 175));
    }
}

void Arrow::paintState(QPainter *painter, ArrowState s, QColor c) {
    static const QPoint triangle[3] = {
        QPoint(-8, 15),
        QPoint(8, 15),
        QPoint(0, -15)
    };

    painter->save();

    painter->setBrush(QBrush(c));
    QPoint paintPos = Board::toDisplayPos(s.pos());
    painter->translate(paintPos);

    if (s.homing()) {
        painter->drawRect(-4, -16, 8, 32);
        painter->drawRect(-16, -4, 32, 8);
    } else {
        painter->rotate(s.rotation());
        painter->drawPolygon(triangle, 3);
    }

    if (boosted) {
        painter->drawEllipse(QPoint(0, 0), 5, 5);
    }

    painter->restore();
}

void Arrow::move() {
    state.move();
}

void Arrow::rotate(int r) {
    int rotation = state.rotation();
    rotation += r;
    state.setRotation(rotation);
}


void Arrow::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    if (event->button() == Qt::LeftButton)
        emit arrowClicked(this);
}
void Arrow::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
    hovered = true;
    emit rerender();
}
void Arrow::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
    hovered = false;
    emit rerender();
}
