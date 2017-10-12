#include "tower.hpp"
#include <qmath.h>
#include <QGraphicsScene>
#include <QVector>
#include <QPolygonF>

Tower::Tower(QPointF pos, const QPixmap& img,
             float attackRadius, unsigned int numOfPolygonSides,
             QObject *parent):
    Tower(pos.x(), pos.y(), img, attackRadius, numOfPolygonSides, parent)
{}

Tower::Tower(float posX, float posY, const QPixmap &img, float attackRadius,
             unsigned int numOfPolygonSides, QObject *parent):
    QObject(parent), QGraphicsPixmapItem()
{
    this->setPixmap(img);
    this->setPos(posX, posY);
    QVector<QPointF> points;
    for(std::size_t i{0u}; i < numOfPolygonSides; ++i)
    {
        QPointF point(attackRadius * qCos(360 / numOfPolygonSides * i),
                      attackRadius * qSin(360 / numOfPolygonSides * i));
        points << point;
    }
    QPolygonF polygon(points);
    mAttackArea = new QGraphicsPolygonItem(polygon, this);
    mAttackArea->setPos(img.width() / 2, img.height() / 2);
}
