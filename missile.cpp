#include "missile.hpp"
#include "game.hpp"
#include "enemy.hpp"
#include <qmath.h>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QDebug>
#include <QLineF>
#include <QList>

Missile::Missile(Game *game, float posX, float posY, float velocity, float maxDistance,
                 int damage, const QPixmap &img, QGraphicsItem *parent):
    QObject(), QGraphicsPixmapItem(parent), mGame{game},
    mInitPos{QPointF(posX, posY)}, mVelocity{velocity},
    mMaxDistance{maxDistance}, mDamage{damage}
{
    this->setPos(posX, posY);
    this->setPixmap(img);
    this->setTransformOriginPoint(img.width() / 2, img.height() / 2);
    mTimer = new QTimer(this);
    connect(this, SIGNAL(maxFlightDistance()), this, SLOT(destroy()));
    connect(mTimer, SIGNAL(timeout()), this, SLOT(move()));

    mTimer->start(MOVE_TIMER_DELAY);
}

Missile::Missile(Game *game, const QPointF &pos, float velocity, float maxDistance,
                 int damage, const QPixmap &img, QGraphicsItem *parent):
    Missile(game, pos.x(), pos.y(), velocity, maxDistance, damage, img, parent)
{}

void Missile::move()
{
    if(mGame->getStatus() != Game::Status::PLAY)
        return;
    auto dx = mVelocity * qCos(qDegreesToRadians(rotation()));
    auto dy = mVelocity * qSin(qDegreesToRadians(rotation()));
    this->setPos(x() + dx, y() + dy);
    QLineF distance(pos(), mInitPos);
    if(distance.length()  >= mMaxDistance) emit maxFlightDistance();

    //========Check for collision with enemies=============//
    handleCollisionsWithEnemies();
}

void Missile::handleCollisionsWithEnemies()
{
    QList<QGraphicsItem*> list = this->collidingItems();
    for(auto &e: list)
    {
        if(typeid(*e) == typeid(Enemy))
        {
            Enemy *enemy = static_cast<Enemy*>(e);
            enemy->hit(this->getDamage());
            this->destroy();
            break;
        }
    }
}

void Missile::destroy()
{
    scene()->removeItem(this);
    delete this;
}
