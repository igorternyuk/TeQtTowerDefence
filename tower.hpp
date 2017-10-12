#pragma once

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QGraphicsPolygonItem>
#include <QPointF>

class QTimer;

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
    QGraphicsPixmapItem *mTurret;
    QPointF mTarget{800, 0};
    bool mHasTarget{true};
    //:/gfx/grayTower.png
    QPixmap mImageProjectile{":/gfx/missile1.png"};
    QTimer *mTimer;
private slots:
    void attackTarget();
    QPointF getCenter();
};
