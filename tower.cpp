#include "tower.hpp"

#include "projectile.hpp"
#include <qmath.h>
#include <QGraphicsScene>
#include <QVector>
#include <QPolygonF>
#include <QDebug>
#include <QTimer>


Tower::Tower(QPointF pos, const QPixmap& img,
             float attackRadius, unsigned int numOfPolygonSides,
              QObject *parent):
    Tower(pos.x(), pos.y(), img, attackRadius, numOfPolygonSides,
          parent)
{}

Tower::Tower(float posX, float posY, const QPixmap &img, float attackRadius,
             unsigned int numOfPolygonSides,
             QObject *parent):
    QObject(parent), QGraphicsPixmapItem()
{
    this->setPixmap(img);
    this->setPos(posX, posY);
    QVector<QPointF> points;
    for(std::size_t i{0u}; i < numOfPolygonSides; ++i)
    {
        auto angle = qDegreesToRadians(360.f / numOfPolygonSides * i);
        QPointF point(attackRadius * qCos(angle), attackRadius * qSin(angle));
        points << point;
    }
    mTurret = new QGraphicsPixmapItem(QPixmap(":/gfx/grayTowerGun.png"),this);
    //mTurret->setPos((img.width() - mTurret->pixmap().width()) / 2,
    //                (img.height() - mTurret->pixmap().height()) / 2);
    mTurret->setTransformOriginPoint(mTurret->pixmap().width() / 2,
                                     mTurret->pixmap().height() / 2);
    QPolygonF polygon(points);
    mAttackArea = new QGraphicsPolygonItem(polygon, this);
    mAttackArea->setPos(img.width() / 2, img.height() / 2);
    mAttackArea->setPen(Qt::DashLine);

    mTimer = new QTimer;
    connect(mTimer, SIGNAL(timeout()), this, SLOT(attackTarget()));
    mTimer->start(2000);
}

void Tower::attackTarget()
{
    QLineF firingLine{getCenter(), mTarget};
    auto angle = firingLine.angle();
    qDebug() << "angle = " << angle;
    Projectile *p = new Projectile(5, mImageProjectile);
    p->setPos(getCenter().x() - p->pixmap().width() / 2,
              getCenter().y() - p->pixmap().height() / 2);
    p->setRotation(-angle);
    p->setPos(p->x() + 48 * qCos(qDegreesToRadians(angle)),
              p->y() - 48 * qSin(qDegreesToRadians(angle)));
    mTurret->setRotation(-angle);
    scene()->addItem(p);
}

QPointF Tower::getCenter()
{
    return QPointF{x() + pixmap().width() / 2, y() + pixmap().height() / 2};
}

