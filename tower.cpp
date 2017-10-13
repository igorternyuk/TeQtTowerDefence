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


Tower::Tower(Type type, const QPointF &pos, const QPixmap &imgBase,
             const QPixmap &imgTurret, const QPixmap &imgMissile,
             float attackRadius, int timerDelay, QObject *parent):
    Tower(type, pos.x(), pos.y(), imgBase, imgTurret, imgMissile,
          attackRadius, timerDelay, parent)
{}

auto Tower::distanceTo(const QPointF &target) const
{
    QLineF line(pos(), target);
    return line.length();
}

Tower::Tower(Type type, float posX, float posY, const QPixmap& imgBase,
             const QPixmap& imgTurret, const QPixmap &imgMissile,
             float attackRadius,
             int timerDelay, QObject *parent):
    QObject(parent), QGraphicsPixmapItem(), mType{type},
    mAttackRadius{attackRadius}, mImgMissile{imgMissile}
{
    this->setPixmap(imgBase);
    this->setPos(posX, posY);
    mTurret = new QGraphicsPixmapItem(imgTurret,this);
    mTurret->setTransformOriginPoint(mTurret->pixmap().width() / 2,
                                     mTurret->pixmap().height() / 2);
    mAttackArea = new QGraphicsEllipseItem(-attackRadius, -attackRadius,
                                           2 * attackRadius,
                                           2 * attackRadius, this);
    mAttackArea->setPos(imgBase.width() / 2, imgBase.height() / 2);
    mAttackArea->setPen(Qt::DashLine);

    mTimer = new QTimer;
    connect(mTimer, SIGNAL(timeout()), this, SLOT(aquireTarget()));
    mTimer->start(timerDelay);
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
    if(!mHasTarget) return;
    QLineF firingLine{getCenter(), getCurrentTarget()};
    auto angle = firingLine.angle();
    //qDebug() << "angle = " << angle;

    if(mType == Type::GREEN)
    {
        auto x = getCenter().x() - mImgMissile.width() / 2 +
                 48 * qCos(qDegreesToRadians(angle));
        auto y = getCenter().y() - mImgMissile.height() / 2 -
                 48 * qSin(qDegreesToRadians(angle));
        Missile *p = new Missile(x, y, 8, getAttackRadius() - 48, 50, mImgMissile);
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
        //qDebug() << "angle1 = " << line1.angle();
        line1.translate(getCenter());
        QLineF line2{QLineF::fromPolar(40,angle - qRadiansToDegrees(qAsin(5.f/40)))};
        line2.translate(getCenter());
        //qDebug() << "angle2 = " << line2.angle();
        //qDebug() << "line1.p2() = " << line1.p2();
        //qDebug() << "line2.p2() = " << line2.p2();
        Missile *p1 = new Missile(line1.p2(), 8, getAttackRadius() - 48, 25, mImgMissile);
        Missile *p2 = new Missile(line2.p2(), 8, getAttackRadius() - 48, 25, mImgMissile);
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
                 48 * qCos(qDegreesToRadians(angle));
        auto y = getCenter().y() - mImgMissile.height() / 2 -
                 48 * qSin(qDegreesToRadians(angle));
        Missile *p1 = new Missile(x, y, 8, getAttackRadius() - 48, 30, mImgMissile);
        Missile *p2 = new Missile(x, y, 8, getAttackRadius() - 48, 30, mImgMissile);
        Missile *p3 = new Missile(x, y, 8, getAttackRadius() - 48, 30, mImgMissile);
        p1->setRotation(-angle + 10);
        p2->setRotation(-angle);
        p3->setRotation(-angle - 10);
        this->setTurretRotation(-angle);
        scene()->addItem(p1);
        scene()->addItem(p2);
        scene()->addItem(p3);
    }
}

