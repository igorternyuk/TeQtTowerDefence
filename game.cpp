#include "game.hpp"
#include "tower.hpp"
#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsScene>
#include <QPixmap>
#include <QGraphicsPixmapItem>
#include <QDebug>

Game::Game(QWidget *parent):
    QGraphicsView(parent)
{
    mScene = new QGraphicsScene;
    this->setScene(mScene);
    this->setFixedSize(WINDOW_WIDTH, WINDOW_HEIGHT);
    this->setWindowTitle(mWindowTitle);
    mScene->setSceneRect(0,0,WINDOW_WIDTH, WINDOW_HEIGHT);
    this->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    this->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    QPixmap towerImage(":/gfx/tower.png");
    mTower = new Tower(150, 150, towerImage, 80, 7);
    mScene->addItem(mTower);
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

void Game::mouseReleaseEvent(QMouseEvent *event)
{

}

