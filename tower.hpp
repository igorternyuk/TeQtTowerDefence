#pragma once

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPointF>

class Game;
class QGraphicsEllipseItem;
class QTimer;

class Tower : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT
public:
    enum class Type
    {
        GREEN,
        RED,
        YELLOW
    };

    explicit Tower(Game *game, Type type, float posX, float posY,
                   QObject *parent = nullptr);
    explicit Tower(Game *game, Type type, const QPointF &pos,
                   QObject *parent = nullptr);
    auto distanceTo(const QPointF& target) const;
    QPointF getCenter() const noexcept;
    inline float getAttackRadius() const noexcept { return mAttackRadius; }
    inline QPointF getCurrentTarget() const noexcept { return mCurrentTarget; }
    inline void setTurretRotation(float angle) { mTurret->setRotation(angle );}
private:
    enum
    {
        POINT_OF_MISSILE_DEPARTURE_OFFSET = 48
    };
    Game *mGame;
    Type mType;
    float mAttackRadius;
    QPixmap mImgMissile;
    float mMissileVelocity;
    int mMissileDamage;
    int mShootingTimerDelay;
    QGraphicsEllipseItem *mAttackArea;
    QGraphicsPixmapItem *mTurret;    
    bool mHasTarget{true};
    QPointF mCurrentTarget;
    QTimer *mTimer;

    virtual void fire();

private slots:
    void aquireTarget();


};
