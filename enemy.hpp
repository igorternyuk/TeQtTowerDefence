#pragma once

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPointF>
#include <QList>

class QTimer;
class Enemy: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Enemy(const QPointF& pos, float velocity,
                   const QList<QPointF>& mPoints,
                   QGraphicsItem *parent = nullptr);
    ~Enemy();
    void hit(float damage);
signals:
    void noHealth();
public slots:
    void move();
    void destroy();
private:
    enum
    {
        MIN_DISTANCE_TO_TARGET = 5,
        TIMER_DELAY = 500
    };
    float mHealth{100};
    float mVelocity;
    const QList<QPointF>& mPoints;
    std::size_t mCurrentTargetIndex{1u};
    QTimer *mTimer;

    void rotateTo(const QPointF& target);
};
