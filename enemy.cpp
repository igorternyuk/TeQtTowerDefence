#include "enemy.hpp"
#include "game.hpp"
#include <QPixmap>
#include <QGraphicsScene>
#include <QLineF>
#include <QTimer>
#include <QDebug>
#include <qmath.h>

Enemy::Enemy(Game *game, const QPointF &pos, float velocity,
             const QList<QPointF> &mPoints, int price, QGraphicsItem *parent):
    QObject(), QGraphicsPixmapItem(parent), mGame{game}, mVelocity{velocity},
    mPoints{mPoints}, mPrice{price}
{
    QPixmap img(":/gfx/enemy.png");
    this->setPixmap(img);
    this->setPos(pos);
    /*this->setPos(pos.x() + img.width() / 2,
                 pos.y() + img.height() / 2);*/
    rotateTo(mPoints[mCurrentTargetIndex]);

    connect(this, SIGNAL(noHealth()), this, SLOT(destroy()));
    mTimer = new QTimer;
    connect(mTimer, SIGNAL(timeout()), this, SLOT(move()));
    mTimer->start(TIMER_DELAY);   
}

Enemy::~Enemy()
{
    //qDebug() << "Enemy destructor was called";
}

void Enemy::hit(float damage)
{
    mHealth -= damage;
    if(mHealth <= 0)
    {
        emit awardPlayer(mPrice);
        emit noHealth();
    }
}

void Enemy::move()
{
    if(mGame->getStatus() != Game::Status::PLAY)
        return;
    QLineF heading(pos(), mPoints[mCurrentTargetIndex]);
    auto angle = heading.angle();
    //qDebug() << "heading = " << angle;
    auto dx = mVelocity * qCos(qDegreesToRadians(-angle));
    auto dy = mVelocity * qSin(qDegreesToRadians(-angle));
    setPos(x() + dx, y() + dy);

    if(heading.length() <= MIN_DISTANCE_TO_TARGET)
    {
        ++mCurrentTargetIndex;
        if(mCurrentTargetIndex < mPoints.size())
            rotateTo(mPoints[mCurrentTargetIndex]);
        else
            disconnect(mTimer, SIGNAL(timeout()), this, SLOT(move()));
    }

    mGame->checkGameStatus();
}

void Enemy::destroy()
{
    scene()->removeItem(this);
    delete this;
}

void Enemy::rotateTo(const QPointF &target)
{
    QLineF heading(pos(), target);
    auto angle = heading.angle();
    setRotation(-angle);
}
