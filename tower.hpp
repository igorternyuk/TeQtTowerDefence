#pragma once

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsPolygonItem>
#include <QPointF>
#include <projectile.hpp>

class Tower : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Tower(float posX, float posY, const QPixmap& img,
                   float attackRadius, unsigned int numOfPolygonSides,
                   QObject *parent = nullptr);
    explicit Tower(QPointF pos, const QPixmap& img,
                   float attackRadius, unsigned int numOfPolygonSides,
                   QObject *parent = nullptr);

private:
    QGraphicsPolygonItem *mAttackArea;
};
