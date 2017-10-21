#include "buildingmodeicon.hpp"

#include <QGraphicsPixmapItem>

BuildingModeIcon::BuildingModeIcon(Tower::Type towerType,
                                   QGraphicsItem *parent):
    QGraphicsPixmapItem(parent), mTowerType{towerType}
{
    switch(mTowerType)
    {
        case Tower::Type::RED:
            this->setPixmap(QPixmap(":/gfx/redTowerIcon.png"));
            break;
        case Tower::Type::GREEN:
            this->setPixmap(QPixmap(":/gfx/greenTowerIcon.png"));
            break;
        case Tower::Type::YELLOW:
            this->setPixmap(QPixmap(":/gfx/yellowTowerIcon.png"));
            break;
    }
}
