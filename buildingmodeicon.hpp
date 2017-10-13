#pragma once

#include <QGraphicsPixmapItem>
#include <QPixmap>

class BuildingModeIcon: public QGraphicsPixmapItem
{
public:
    explicit BuildingModeIcon(const QPixmap& img,
                              QGraphicsItem *parent = nullptr);
protected:
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
private:
    QPixmap mImage;
};
