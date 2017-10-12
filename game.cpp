#include "game.hpp"
#include "tower.hpp"
#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsScene>
#include <QPixmap>
#include <QLineF>
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
    QPixmap towerImage(":/gfx/grayTower.png");
    mTower = new Tower(150, 150, towerImage, 120, 30);
    mScene->addItem(mTower);
    //==============Enemy route===================
    mEnemyRoute << QPointF(0, WINDOW_HEIGHT) <<
                   QPointF(WINDOW_WIDTH / 2, 3 * WINDOW_HEIGHT / 4) <<
                   QPointF(2 * WINDOW_WIDTH / 3, WINDOW_HEIGHT / 4) <<
                   QPointF(WINDOW_WIDTH, 0);
    createRoad();
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
        pen.setWidth(40);
        section->setPen(pen);
        mScene->addItem(section);
    }
}

void Game::mouseReleaseEvent(QMouseEvent *event)
{

}

