#pragma once

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPixmap>

class QTimer;

class Projectile : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Projectile(float velocity,
                        const QPixmap &img, QGraphicsItem *parent = nullptr);
public slots:
    void move();
private:
    float mVelocity;
    QTimer *mTimer;
};
