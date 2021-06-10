//
// Created by misha on 07.06.2021.
//

#ifndef SCENE_H
#define SCENE_H
#ifndef GSIZE
#define GSIZE 19
#endif
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QPixmap>
#include <QDebug>
#include <QPoint>
#include <QPointF>
#include <QColor>
#include <utility>

#include "game.hpp"

class Token;

class Scene : public QGraphicsScene {
    Q_OBJECT
public:
    Move lastPredictedMove;
    enum TokenColor {
        EMPTY,
        BLACK,
        WHITE,
    };
    static TokenColor tokenColorFromInt(int v);
    static int intFromTokenColor(TokenColor v);
    class TokenDef {
    public:
        TokenDef(TokenColor c, QColor h, QString t):
            color(c)
            , highlight(std::move(h))
            , text(std::move(t))
        {
        }

        TokenColor color = EMPTY;
        QColor highlight = QColor(Qt::transparent);
        QString text = QString();
    };
    const int offsetY = 67;
    const int offsetX = 69;
    const int pSize = 40;
    const int pHeight = 40 / 2;
    bool devMode = false;
    bool showMask = false;
    bool pvpMode = false;
    TokenColor playAs = WHITE;
    void startGame();
    explicit Scene(Game *game, QObject *parent = nullptr);
    ~Scene() override;
    Game *game;
    QPixmap imWhitePeace;
    QPixmap imBlackPeace;
    QPixmap imPanel;
    void setBoard(TokenDef tks[GSIZE][GSIZE]);
    void setBoard(TokenDef tks[GSIZE * GSIZE]);
    void setToken(int x, int y, TokenDef td);
    Token* getToken(int x, int y);
private:
    Token* tokens[GSIZE][GSIZE]{nullptr};
    QPixmap bg;
    QPoint viewPosToBoard(QPoint pt);
    QPoint boardPosToView(QPoint pt);
    void onTokenClicked(Token *token, QGraphicsSceneMouseEvent *event);
protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *event) override;
public slots:
    void reset();
signals:
    void resetted();
};
#endif //SCENE_H
