#pragma once

#include <QObject>
#include <QGraphicsPixmapItem>
#include <QPointF>

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

    explicit Tower(Type type, float posX, float posY, const QPixmap& imgBase,
                   const QPixmap& imgTurret, const QPixmap& imgMissile,
                   float attackRadius, int timerDelay, QObject *parent = nullptr);
    explicit Tower(Type type, const QPointF &pos, const QPixmap& imgBase,
                   const QPixmap& imgTurret, const QPixmap &imgMissile,
                   float attackRadius,
                   int timerDelay, QObject *parent = nullptr);
    auto distanceTo(const QPointF& target) const;
    QPointF getCenter() const noexcept;
    inline float getAttackRadius() const noexcept { return mAttackRadius; }
    inline QPointF getCurrentTarget() const noexcept { return mCurrentTarget; }
    inline void setTurretRotation(float angle) { mTurret->setRotation(angle );}
private:
    Type mType;
    float mAttackRadius;
    QPixmap mImgMissile;
    QGraphicsEllipseItem *mAttackArea;
    QGraphicsPixmapItem *mTurret;    
    bool mHasTarget{true};
    QPointF mCurrentTarget{0,600};
    QTimer *mTimer;

    virtual void fire();

private slots:
    void aquireTarget();


};
