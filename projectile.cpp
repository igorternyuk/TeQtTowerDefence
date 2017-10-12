#include "projectile.hpp"
#include <qmath.h>
#include <QTimer>
#include <QDebug>

Projectile::Projectile(float velocity, const QPixmap &img,
                       QGraphicsItem *parent):
    mVelocity{velocity}
{
    this->setPixmap(img);
    this->setTransformOriginPoint(img.width() / 2, img.height() / 2);
    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(move()));
    mTimer->start(50);
}

void Projectile::move()
{
    //qDebug() << "Projectile is moving";
    this->setPos(x() + mVelocity * qCos(qDegreesToRadians(rotation())),
                 y() + mVelocity * qSin(qDegreesToRadians(rotation())));
}
