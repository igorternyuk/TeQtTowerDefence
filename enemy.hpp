#pragma once

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPointF>
#include <QList>
#include "game.hpp"

class Game;
class QTimer;

class Enemy: public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Enemy(Game *game, const QPointF& pos, float velocity,
                   const QList<QPointF>& mPoints, int price,
                   QGraphicsItem *parent = nullptr);
    ~Enemy();
    inline auto getPrice() const noexcept { return mPrice; }
    void hit(float damage);
signals:
    void awardPlayer(int price);
    void noHealth();
public slots:
    void move();
    void destroy();
private:    
    enum
    {
        MIN_DISTANCE_TO_TARGET = 5,
        TIMER_DELAY = 250
    };
    Game *mGame;
    float mHealth{100};    
    float mVelocity;
    const QList<QPointF>& mPoints;
    int mPrice;
    std::size_t mCurrentTargetIndex{1u};
    QTimer *mTimer;

    void rotateTo(const QPointF& target);
};
