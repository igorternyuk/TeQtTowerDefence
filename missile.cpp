#include "missile.hpp"
#include "enemy.hpp"
#include <qmath.h>
#include <QTimer>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QDebug>
#include <QLineF>
#include <QList>

Missile::Missile(float posX, float posY, float velocity, float maxDistance,
                 int damage, const QPixmap &img, QGraphicsItem *parent):
    QObject(), QGraphicsPixmapItem(parent), mInitPos{QPointF(posX, posY)},
    mVelocity{velocity}, mMaxDistance{maxDistance}, mDamage{damage}
{
    this->setPos(posX, posY);
    this->setPixmap(img);
    this->setTransformOriginPoint(img.width() / 2, img.height() / 2);
    mTimer = new QTimer(this);
    connect(this, SIGNAL(maxFlightDistance()), this, SLOT(destroy()));
    connect(mTimer, SIGNAL(timeout()), this, SLOT(move()));

    mTimer->start(50);
}

Missile::Missile(const QPointF &pos, float velocity, float maxDistance,
                 int damage, const QPixmap &img, QGraphicsItem *parent):
    Missile(pos.x(), pos.y(), velocity, maxDistance, damage, img, parent)
{}

void Missile::move()
{
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
            //qDebug() << "Collision with enemy";
            Enemy *enemy = static_cast<Enemy*>(e);
            //e->destroy();
            enemy->hit(this->getDamage());
            this->destroy();
        }
    }
}

void Missile::destroy()
{
    //qDebug() << "Projectile was destroyed";
    scene()->removeItem(this);
    delete this;
}
