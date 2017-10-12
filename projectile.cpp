#include "projectile.hpp"
#include <qmath.h>
#include <QTimer>

Projectile::Projectile(float posX, float posY, float velocity,
                       const QPixmap &img, QGraphicsItem *parent):
    mVelocity{velocity}
{
    this->setPixmap(img);
    mTimer = new QTimer(this);
    connect(mTimer, SIGNAL(timeout()), this, SLOT(move()));
    mTimer->start(50);
}

void Projectile::move()
{
    this->setPos(mVelocity * qSin(qDegreesToRadians(this->rotation())),
                 mVelocity * qCos(qDegreesToRadians(this->rotation())));
}
