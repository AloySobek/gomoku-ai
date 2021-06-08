//
// Created by misha on 07.06.2021.
//

#ifndef FT_GOMOKU_QT_TOKEN_H
#define FT_GOMOKU_QT_TOKEN_H
#include <QGraphicsItem>
#include <QPainter>
#include <QStyleOptionGraphicsItem>
#include <QWidget>
#include <QRectF>
#include <QPen>
#include "Scene.h"

class Token : public QGraphicsItem {
public:
    Scene::TokenDef def;
    const int x, y;
    bool hovered;
    const int size = 40;
    explicit Token(int x, int y);
    QRectF boundingRect() const override;
    void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget) override;
    void setDef(Scene::TokenDef d);

protected:
    void hoverLeaveEvent(QGraphicsSceneHoverEvent *event) override;
    void hoverEnterEvent(QGraphicsSceneHoverEvent *event) override;

private:
    Scene* getScene();
};

#endif //FT_GOMOKU_QT_TOKEN_H
