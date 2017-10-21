#pragma once

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QPointF>

class Game;
class QTimer;

class Missile : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    explicit Missile(Game *game, float posX, float posY, float velocity, float maxDistance,
                     int damage, const QPixmap &img,
                     QGraphicsItem *parent = nullptr);
    explicit Missile(Game *game, const QPointF &pos, float velocity, float maxDistance,
                     int damage, const QPixmap &img,
                     QGraphicsItem *parent = nullptr);
    inline auto getDamage() const noexcept { return mDamage; }
signals:
    void maxFlightDistance();
public slots:
    void move();
private:
    enum { MOVE_TIMER_DELAY = 50 };
    Game *mGame;
    QPointF mInitPos;
    float mVelocity;
    float mMaxDistance;
    QTimer *mTimer;
    int mDamage;
    void handleCollisionsWithEnemies();
private slots:
    void destroy();
};
