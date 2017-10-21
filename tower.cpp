#include "tower.hpp"

#include "missile.hpp"
#include "enemy.hpp"
#include <qmath.h>
#include <QGraphicsItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsScene>
#include <QVector>
#include <QList>
#include <QDebug>
#include <QLineF>
#include <QTimer>
#include "game.hpp"

Tower::Tower(Game *game, Type type, const QPointF &pos,
             QObject *parent):
    Tower(game, type, pos.x(), pos.y(), parent)
{}

auto Tower::distanceTo(const QPointF &target) const
{
    QLineF line(pos(), target);
    return line.length();
}

Tower::Tower(Game *game, Type type, float posX, float posY, QObject *parent):
    QObject(parent), QGraphicsPixmapItem(), mGame{game}, mType{type}
{
    this->setPixmap(QPixmap(":/gfx/tower.png"));
    this->setPos(posX, posY);
    switch (type) {
    case Type::RED:
        mAttackRadius = 140;
        mShootingTimerDelay = 1500;
        mTurret = new QGraphicsPixmapItem(QPixmap(":/gfx/redTowerTurret.png"),this);
        mImgMissile = QPixmap(":/gfx/redTowerMissile.png");
        mMissileVelocity = 8;
        mMissileDamage = 5;
        break;
    case Type::GREEN:
        mAttackRadius = 135;
        mShootingTimerDelay = 1300;
        mTurret = new QGraphicsPixmapItem(QPixmap(":/gfx/greenTowerTurret.png"),this);
        mImgMissile = QPixmap(":/gfx/greenTowerMissile.png");
        mMissileVelocity = 12;
        mMissileDamage = 3;
        break;
    case Type::YELLOW:
        mShootingTimerDelay = 1000;
        mAttackRadius = 130;
        mTurret = new QGraphicsPixmapItem(QPixmap(":/gfx/yellowTowerTurret.png"),this);
        mImgMissile = QPixmap(":/gfx/yellowTowerMissile.png");
        mMissileVelocity = 7;
        mMissileDamage = 7;
        break;
    default:
        break;
    }
    mTurret->setTransformOriginPoint(mTurret->pixmap().width() / 2,
                                     mTurret->pixmap().height() / 2);
    mAttackArea = new QGraphicsEllipseItem(-mAttackRadius, -mAttackRadius,
                                           2 * mAttackRadius,
                                           2 * mAttackRadius, this);
    mAttackArea->setPos(this->pixmap().width() / 2,
                        this->pixmap().height() / 2);
    mAttackArea->setPen(Qt::DashLine);

    mTimer = new QTimer;
    connect(mTimer, SIGNAL(timeout()), this, SLOT(aquireTarget()));
    mTimer->start(mShootingTimerDelay);
}

QPointF Tower::getCenter() const noexcept
{
    return QPointF{x() + pixmap().width() / 2, y() + pixmap().height() / 2};
}

void Tower::aquireTarget()
{
    QList<QGraphicsItem*> list = mAttackArea->collidingItems();
    mHasTarget = false;
    QPointF closestTarget;
    double closestDistanse{300};
    for(auto &e: list)
    {
        Enemy *enemy = dynamic_cast<Enemy*>(e);
        if(enemy)
        {
            mHasTarget = true;
            auto currDist = this->distanceTo(enemy->pos());
            if(currDist < closestDistanse)
            {
                closestDistanse = currDist;
                closestTarget = enemy->pos();
            }
        }
    }
    if(mHasTarget)
    {
        mCurrentTarget = closestTarget;
        fire();
    }
}

void Tower::fire()
{
    if(mGame->getStatus() != Game::Status::PLAY) return;
    if(!mHasTarget) return;

    QLineF firingLine{getCenter(), getCurrentTarget()};
    auto angle = firingLine.angle();

    if(mType == Type::GREEN)
    {
        auto x = getCenter().x() - mImgMissile.width() / 2 +
                 48 * qCos(qDegreesToRadians(angle));
        auto y = getCenter().y() - mImgMissile.height() / 2 -
                 48 * qSin(qDegreesToRadians(angle));
        Missile *p = new Missile(mGame, x, y, mMissileVelocity,
                                 getAttackRadius() -
                                 POINT_OF_MISSILE_DEPARTURE_OFFSET,
                                 mMissileDamage, mImgMissile);
        p->setRotation(-angle);
        this->setTurretRotation(-angle);
        scene()->addItem(p);
    }
    else if(mType == Type::RED)
    {
        //40 - расстояние от центра башни до точки вылета снаряда из ствола - по гипотенузе
        // 10 - это расстояние между параллельными стволами пушек турели
        // 5 вверх и 5 вниз от средней линии между стволами получается два
        //прямоугольных треугольника
        QLineF line1{QLineF::fromPolar(40,angle + qRadiansToDegrees(qAsin(5.f/40)))};
        line1.translate(getCenter());
        QLineF line2{QLineF::fromPolar(40,angle - qRadiansToDegrees(qAsin(5.f/40)))};
        line2.translate(getCenter());
        Missile *p1 = new Missile(mGame, line1.p2(), mMissileVelocity,
                                  getAttackRadius() -
                                  POINT_OF_MISSILE_DEPARTURE_OFFSET,
                                  mMissileDamage, mImgMissile);
        Missile *p2 = new Missile(mGame, line2.p2(), mMissileVelocity,
                                  getAttackRadius() -
                                  POINT_OF_MISSILE_DEPARTURE_OFFSET,
                                  mMissileDamage, mImgMissile);
        p1->setPos(p1->x() - p1->pixmap().width() / 2,
                   p1->y() - p1->pixmap().height() / 2);
        p2->setPos(p2->x() - p2->pixmap().width() / 2,
                   p2->y() - p2->pixmap().height() / 2);
        p1->setRotation(-angle);
        p2->setRotation(-angle);
        this->setTurretRotation(-angle);
        scene()->addItem(p1);
        scene()->addItem(p2);
    }
    else if(mType == Type::YELLOW)
    {
        auto x = getCenter().x() - mImgMissile.width() / 2 +
                 POINT_OF_MISSILE_DEPARTURE_OFFSET *
                 qCos(qDegreesToRadians(angle));
        auto y = getCenter().y() - mImgMissile.height() / 2 -
                 POINT_OF_MISSILE_DEPARTURE_OFFSET *
                 qSin(qDegreesToRadians(angle));
        Missile *p1 = new Missile(mGame, x, y, mMissileVelocity,
                                  getAttackRadius() -
                                  POINT_OF_MISSILE_DEPARTURE_OFFSET,
                                  mMissileDamage, mImgMissile);
        Missile *p2 = new Missile(mGame, x, y, mMissileVelocity,
                                  getAttackRadius() -
                                  POINT_OF_MISSILE_DEPARTURE_OFFSET,
                                  mMissileDamage, mImgMissile);
        Missile *p3 = new Missile(mGame, x, y, mMissileVelocity,
                                  getAttackRadius() -
                                  POINT_OF_MISSILE_DEPARTURE_OFFSET,
                                  mMissileDamage, mImgMissile);
        //10 - Угол вылета левого и правого снарядов относительно центрального
        p1->setRotation(-angle + 10);
        p2->setRotation(-angle);
        p3->setRotation(-angle - 10);
        this->setTurretRotation(-angle);
        scene()->addItem(p1);
        scene()->addItem(p2);
        scene()->addItem(p3);
    }
}

