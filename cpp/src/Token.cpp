//
// Created by misha on 07.06.2021.
//

#include <utility>

#include "Token.hpp"

void Token::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) {
    painter->save();
    auto sx = 0;
    auto sy = 0;
    auto w = 40;
    auto h = 40;
    switch (def.color) {
        case Scene::WHITE:
            painter->drawPixmap(sx, sy, getScene()->imWhitePeace);
            break;
        case Scene::BLACK:
            painter->drawPixmap(sx, sy, getScene()->imBlackPeace);
            break;
        case Scene::EMPTY:
            break;
    }
    if (def.highlight != QColor(Qt::transparent))
    {
        QPen pen;
        pen.setWidth(3);
        pen.setColor(def.highlight);
        painter->setPen(pen);
        painter->drawEllipse(sx, sy, w - 2, h - 2);
    }
    if (hovered) {
        QPen pen;
        pen.setWidth(3);
        pen.setColor(QColor(Qt::green));
        painter->setPen(pen);
        painter->drawEllipse(sx, sy, w - 2, h - 2);
    }
    if (!def.text.isNull()) {
        QFont font;
        font.setBold(true);
        font.setPixelSize(20);
        QPen pen;
        pen.setColor(QColor(Qt::white));
        painter->setPen(pen);
        painter->setFont(font);
        painter->drawText(sx, sy+15, def.text);
    }
    painter->restore();
}

QRectF Token::boundingRect() const {
    return {0, 0,38, 38};
}

Scene* Token::getScene() {
    return dynamic_cast<Scene*>(this->scene());
}

void Token::hoverEnterEvent(QGraphicsSceneHoverEvent *event) {
//    qDebug() << "hoverEnterEvent";
    QGraphicsItem::hoverEnterEvent(event);
    hovered = true;
    update();
}

void Token::hoverLeaveEvent(QGraphicsSceneHoverEvent *event) {
//    qDebug() << "hoverLeaveEvent";
    QGraphicsItem::hoverLeaveEvent(event);
    hovered = false;
    update();
}

Token::Token(int x, int y)
    : QGraphicsItem()
    , hovered(false)
    , def(Scene::EMPTY, QColor(Qt::transparent), QString())
    , x(x)
    , y(y)
{
    this->setAcceptHoverEvents(true);
}

void Token::setDef(Scene::TokenDef d) {
    def = std::move(d);
}


