#pragma once

#include <QWidget>
#include <QGraphicsView>
#include <QList>
#include <QPointF>

/*
 * 1) Дать башням и пулям максимальную дальность
 * 2) Дать врагам показатель здровья
 * 3) Дать пулям коэффциент ущерба
 * 4) Ввести систему очков для игрока
 * 5)Если хоть один враг проходи в нижний левый угол - поражение
*/
class QGraphicsScene;
class Tower;

class Game: public QGraphicsView
{
    Q_OBJECT
public:
    enum { WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600 };
    explicit Game(QWidget *parent = nullptr);
    void run();
protected:
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
private:
    const QString mWindowTitle{"TeQtTowerDefence"};
    QGraphicsScene *mScene;
    Tower *mTower;
    QList<QPointF> mEnemyRoute;
    void centralize();
    void createRoad();
    void createEnemy(const QPointF& pos);
    void createEnemies();
};

