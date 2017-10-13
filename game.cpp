#include "game.hpp"

#include "tower.hpp"
#include "enemy.hpp"
#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsScene>
#include <QPixmap>
#include <QLineF>
#include <QTimer>
#include <QGraphicsLineItem>
#include <QGraphicsPixmapItem>
#include <QDebug>

Game::Game(QWidget *parent):
    QGraphicsView(parent)
{
    mScene = new QGraphicsScene;
    mScene->setBackgroundBrush(QColor(170, 251, 136));
    this->setScene(mScene);
    this->setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    this->setWindowTitle(mWindowTitle);
    mScene->setSceneRect(0,0,WINDOW_WIDTH, WINDOW_HEIGHT);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);

    //==============Enemy route===================//
    mEnemyRoute << QPointF(WINDOW_WIDTH, 0) <<
                   QPointF(2 * WINDOW_WIDTH / 3, WINDOW_HEIGHT / 4) <<
                   QPointF(WINDOW_WIDTH / 2, 3 * WINDOW_HEIGHT / 4) <<
                   QPointF(0, WINDOW_HEIGHT);
    createRoad();

    //=============Towers=========================//
    QPixmap towerImage(":/gfx/tower.png");
    QPixmap turrentImage(":/gfx/greenTowerTurret.png");
    QPixmap missileImage(":/gfx/greenTowerMissile.png");

    Tower *gTower = new Tower(Tower::Type::GREEN, 150, 150, towerImage, turrentImage,
                            missileImage, 200, 1000);

    Tower *rTower = new Tower(Tower::Type::RED, 600, 180, QPixmap(":/gfx/tower.png"),
                                    QPixmap(":/gfx/redTowerTurret.png"),
                                    QPixmap(":/gfx/redTowerMissile.png"),
                                    200, 1000);

    Tower *yTower = new Tower(Tower::Type::YELLOW, 560, 450, QPixmap(":/gfx/tower.png"),
                                    QPixmap(":/gfx/yellowTowerTurret.png"),
                                    QPixmap(":/gfx/yellowTowerMissile.png"),
                                    250, 1000);
    mScene->addItem(gTower);
    mScene->addItem(rTower);
    mScene->addItem(yTower);


    mEnemyTimer = new QTimer;
    connect(mEnemyTimer, SIGNAL(timeout()), this, SLOT(createEnemy()));
    mEnemyTimer->start(2000);
}

void Game::run()
{
    this->show();
    this->centralize();
}

void Game::centralize()
{
    QRect myScreenGeometry = QApplication::desktop()->geometry() ;
    int dx = (myScreenGeometry.width() - this->width()) / 2;
    int dy = (myScreenGeometry.height() - this->height()) / 2;
    this->move(dx, dy);
}

void Game::createRoad()
{
    for(std::size_t i{0u}; i < mEnemyRoute.size() - 1; ++i)
    {
        QLineF line(mEnemyRoute[i], mEnemyRoute[i + 1]);
        QGraphicsLineItem *section = new QGraphicsLineItem(line);
        QPen pen;
        pen.setColor(Qt::darkGray);
        pen.setWidth(60);
        section->setPen(pen);
        mScene->addItem(section);
    }
}

void Game::createEnemy()
{
    static std::size_t enemyCounter{0u};
    if(enemyCounter >= mMaxNumOfEnemies)
    {
        disconnect(mEnemyTimer, SIGNAL(timeout()), this, SLOT(createEnemy()));
        return;
    }

    Enemy *enemy = new Enemy(QPointF(WINDOW_WIDTH, 0), 10, mEnemyRoute);
    ++enemyCounter;
    mScene->addItem(enemy);
}

void Game::mouseReleaseEvent(QMouseEvent *event)
{

}

