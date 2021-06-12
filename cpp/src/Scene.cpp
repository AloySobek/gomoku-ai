#include "Token.hpp"
#include "Scene.hpp"
#include "Patterns.hpp"
#include <QApplication>

void Scene::drawBackground(QPainter *painter, const QRectF &rect) {
    QGraphicsScene::drawBackground(painter, rect);
    painter->drawPixmap(QRectF(bg.rect()), bg, QRectF(bg.rect()));
    QRectF pr(imPanel.rect());
    pr.moveLeft(bg.rect().width());
    painter->drawPixmap(pr, imPanel, QRectF(imPanel.rect()));
//    qDebug() << "drawBackground" << "bg valid: " << !bg.isNull();
}

Scene::Scene(Game *game, QObject *parent)
    : QGraphicsScene(parent)
    , lastPredictedMove(false)
    , game(game)
    , imWhitePeace(QPixmap(QString().fromStdString("images/whitePiece.png")))
    , imBlackPeace(QPixmap(QString().fromStdString("images/blackPiece.png")))
    , imPanel(QPixmap(QString().fromStdString("images/panel.jpg")))
    , bg(QPixmap(QString().fromStdString("images/board.jpg")))
    {
    for (int y = 0; y < GSIZE; ++y) {
        for (int x = 0; x < GSIZE; ++x) {
            auto t = new Token(x, y);
            this->addItem(t);
//            qDebug() << "xy: " << x << y;
            t->setPos(this->boardPosToView(QPoint(x, y)));
            tokens[y][x] = t;
        }
    }
    reset();
}

Scene::~Scene() {
    for (const auto &item : items()) {
        removeItem(item);
        delete item;
    }
}

QPoint Scene::viewPosToBoard(QPoint pt) {
    QPoint p{
            (pt.x() - this->offsetX + this->pHeight) / this->pSize,
            (pt.y() - this->offsetY + this->pHeight) / this->pSize
    };
    return p;
}

QPoint Scene::boardPosToView(QPoint pt) {
    QPoint p;
    p.setX(pt.x() * this->pSize + this->offsetX - this->pHeight);
    p.setY(pt.y() * this->pSize + this->offsetY - this->pHeight);
//    qDebug() << p;
    return p;
}

Token *Scene::getToken(int x, int y) {
    if (x >= GSIZE || x < 0 || y >= GSIZE || y < 0)
        return nullptr;
    return tokens[y][x];
}

void Scene::setBoard(TokenDef tks[GSIZE][GSIZE]) {
    for (int y = 0; y < GSIZE; ++y) {
        for (int x = 0; x < GSIZE; ++x) {
            getToken(x,y)->setDef(tks[y][x]);
        }
    }
}

void Scene::setBoard(TokenDef tks[GSIZE*GSIZE]) {
    for (int y = 0; y < GSIZE; ++y) {
        for (int x = 0; x < GSIZE; ++x) {
            getToken(x,y)->setDef(tks[y * GSIZE + x]);
        }
    }
}

void Scene::setToken(int x, int y, Scene::TokenDef td) {
    auto tk = getToken(x, y);
    if (!tk)
        return;
    tk->setDef(td);
    game->setToken(tk->x, tk->y, tk->def.color);
}

Scene::TokenColor Scene::tokenColorFromInt(int v) {
    switch (v) {
        case 1:
            return Scene::BLACK;
        case 2:
            return Scene::WHITE;
        case 0:
            return Scene::EMPTY;
        default:
            throw std::runtime_error("tokenColorFromInt unknown int value!");
    }
}

void Scene::mousePressEvent(QGraphicsSceneMouseEvent *event) {
    QGraphicsScene::mousePressEvent(event);
    if (event->button() == Qt::LeftButton)
    {
        auto *item = itemAt(event->scenePos(), QTransform());// it is your clicked item, you can do everything what you want. for example send it somewhere
        auto *ell = qgraphicsitem_cast<Token *>(item);
        if(ell)
        {
            qDebug() << "clicked token at" << ell->x << ell->y;
            onTokenClicked(ell, event);
        }
        else
            qDebug() << "not ell" << event->scenePos();
    }
}

int Scene::intFromTokenColor(Scene::TokenColor v) {
    return v;
}

void Scene::onTokenClicked(Token *token, QGraphicsSceneMouseEvent *event) {
    if (devMode){
        switch (event->modifiers())
        {
            case Qt::Modifier::SHIFT:
                token->def.color = BLACK;
                break;
            case Qt::Modifier::ALT:
                token->def.color = WHITE;
                break;
            case 0:
                token->def.color = EMPTY;
                break;
        }
        game->setToken(token->x, token->y, token->def.color);
        token->update();
        reset();
        return;
    } else if (pvpMode && game->result() == Board::NO_RESULT) {
        if (!game->getToken(token->x, token->y))
        {
            if (lastPredictedMove.v == BLACK)
                token->def.color = WHITE;
            else
                token->def.color = BLACK;
            game->setToken(token->x, token->y, token->def.color);
            token->update();
            Move move{true, static_cast<int8_t>(token->x), static_cast<int8_t>(token->y), static_cast<int8_t>(token->def.color), 0};
            if (lastPredictedMove.valid)
                getToken(lastPredictedMove.x, lastPredictedMove.y)->def.highlight = Qt::transparent;
            lastPredictedMove = move;
            reset();
            getToken(lastPredictedMove.x, lastPredictedMove.y)->def.highlight = Qt::darkRed;
        }
    } else if (game->result() == Board::NO_RESULT) {
        if (!game->getToken(token->x, token->y))
        {
            token->def.color = playAs;
            game->setToken(token->x, token->y, token->def.color);
            token->update();
            auto move  = game->predictMove(playAs == BLACK ? WHITE : BLACK);
            if (move.valid) {
                if (lastPredictedMove.valid)
                    getToken(lastPredictedMove.x, lastPredictedMove.y)->def.highlight = Qt::transparent;
                lastPredictedMove = move;
                game->setToken(move.x, move.y, move.v);
                reset();
                getToken(lastPredictedMove.x, lastPredictedMove.y)->def.highlight = Qt::darkRed;
            }
        }
    }
}

void Scene::reset() {
    if (!game)
        return;
    for (int y = 0; y < GSIZE; ++y) {
        for (int x = 0; x < GSIZE; ++x) {
            getToken(x, y)->setDef({
              Scene::tokenColorFromInt(game->getToken(x, y)),
              QColor(Qt::transparent),
              showMask && game->board.move_map[GSIZE * y + x] ? QString::number(game->board.move_map[GSIZE * y + x]) : QString()
            });
        }
    }
    update();
    emit resetted();
}

void Scene::startGame() {
    if (playAs == WHITE && !pvpMode) {
        game->setToken(BOARD_SIZE/2, BOARD_SIZE/2, BLACK);
        getToken(BOARD_SIZE/2, BOARD_SIZE/2)->def.color = BLACK;
        getToken(BOARD_SIZE/2, BOARD_SIZE/2)->update();
    }
}

void Scene::onGameFinished() {
    WinDialog dlg;
    switch (game->result()) {
        case Board::WHITE_WIN:
            dlg.asWhiteWin();
            break;
        case Board::BLACK_WIN:
            dlg.asBlackWin();
            break;
        case Board::DRAW:
            dlg.asDraw();
            break;
        default:
            throw std::runtime_error("onGameFinished called when game is still running!");
    }
    dlg.exec();
    qDebug() << "Game finished, exiting";
    emit finished();
}
