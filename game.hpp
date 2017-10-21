#pragma once

#include "tower.hpp"
#include "buildingmodeicon.hpp"
#include "texItem.hpp"
#include <QWidget>
#include <QGraphicsView>
#include <QList>
#include <QPointF>

/*
 * 1) Дать башням и пулям максимальную дальность
 * 2) Дать врагам показатель здровья
 * 3) Дать пулям коэффциент ущерба
 * 4) Ввести систему очков для игрока
 * 5)Если хоть один враг проходит в нижний левый угол - поражение
 *
 * We will create 7 objects:
 * 3-buttons for activating build mode -> connect to game slot - activate buils mode
 * players score text label -> connect ot enemy noHealth signal to increase score
 * game state text label -> game has direct accsess to it
 * base
 * cursor
*/
class QGraphicsScene;
class QGraphicsPixmapItem;
class QTimer;

class Game: public QGraphicsView
{
    Q_OBJECT
public:
    enum { WINDOW_WIDTH = 800, WINDOW_HEIGHT = 600 };
    enum class Status
    {
        PLAY,
        PAUSE,
        VICTORY,
        DEFEAT
    };
    explicit Game(QWidget *parent = nullptr);
    inline Status getStatus() const noexcept { return mStatus; }
    void run();
    void checkGameStatus();
protected:
    virtual void keyReleaseEvent(QKeyEvent *event) override;
    virtual void mouseMoveEvent(QMouseEvent * event) override;
    virtual void mouseReleaseEvent(QMouseEvent *event) override;
private:
    enum
    {
        MAX_NUM_OF_ENEMIES = 30,
        ENEMY_CREATION_TIMER_DELAY = 2000,
        ENEMY_VELOCITY = 10,
        SCRORE_FOR_ONE_KILLED_ENEMY = 5,
        ROAD_WIDTH = 60
    };
    const QString mWindowTitle{"TeQtTowerDefence"};
    Status mStatus{ Status::PLAY };
    std::size_t mNumOfKilledEnemies{0};
    std::size_t mEnemyCounter{0u};
    QGraphicsScene *mScene;
    QList<QPointF> mEnemyRoute;
    QTimer *mEnemyTimer;
    bool mIsBuildModeActive { false };
    Tower::Type mTypeOfNextTower;
    BuildingModeIcon *mRedTowerBuildButton, *mGreenTowerBuildButton,
                     *mYellowTowerBuildButton;
    TextItem<int> *mPlayerScore;
    QGraphicsTextItem *mGameStatusLabel;
    QGraphicsPixmapItem *mBase { nullptr };
    QGraphicsPixmapItem *mCursor { nullptr };

    void centralize();
    void createRoad();
    void startNewGame();
    void updateStatusLabel();
    void activateBuildMode(Tower::Type typeOfTowerToBeBuilt);
    void deactivateBuildMode();
    void createTower(Tower::Type type, const QPointF& pos);
private slots:
    void createEnemy();
    void increasePlayerScore(int increment);
};

