#pragma once

#include <QPixmap>
#include "tower.hpp"

class QGraphicsPixmapItem;
class QGraphicsSceneMouseEvent;

class BuildingModeIcon: public QGraphicsPixmapItem
{
public:
    explicit BuildingModeIcon(Tower::Type towerType,
                              QGraphicsItem *parent = nullptr);
    auto getTowerType() const noexcept { return mTowerType; }
private:
    Tower::Type mTowerType;
    QPixmap mImage;
};
