#include "game.hpp"

#include "tower.hpp"
#include "missile.hpp"
#include "enemy.hpp"
#include <QApplication>
#include <QDesktopWidget>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QGraphicsLineItem>
#include <QGraphicsPixmapItem>
#include <QPixmap>
#include <QLineF>
#include <QList>
#include <QTimer>
#include <QMouseEvent>
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
    this->setMouseTracking(true);

    //==============Enemy route===================//
    mEnemyRoute << QPointF(WINDOW_WIDTH, 0) <<
                   QPointF(2 * WINDOW_WIDTH / 3, WINDOW_HEIGHT / 4) <<
                   QPointF(WINDOW_WIDTH / 2, 3 * WINDOW_HEIGHT / 4) <<
                   QPointF(0, WINDOW_HEIGHT);
    createRoad();

    //=======================BUTTONS==========================//=
    mRedTowerBuildButton = new BuildingModeIcon(Tower::Type::RED);
    mRedTowerBuildButton->setPos(0,0);
    mScene->addItem(mRedTowerBuildButton);

    mGreenTowerBuildButton = new BuildingModeIcon(Tower::Type::GREEN);
    mGreenTowerBuildButton->setPos(0, 70);
    mScene->addItem(mGreenTowerBuildButton);

    mYellowTowerBuildButton = new BuildingModeIcon(Tower::Type::YELLOW);
    mYellowTowerBuildButton->setPos(0, 140);
    mScene->addItem(mYellowTowerBuildButton);

    mBase = new QGraphicsPixmapItem(QPixmap(":/gfx/Base.png"));
    mBase->setPos(0, WINDOW_HEIGHT - mBase->pixmap().height());
    mScene->addItem(mBase);
    mPlayerScore = new TextItem<int>(QString("SCORE"), 0, 70, 0,
                                     QFont("times", 30), Qt::darkGreen);
    mScene->addItem(mPlayerScore);
    mGameStatusLabel = new QGraphicsTextItem("GAME STATUS: PLAY");
    mGameStatusLabel->setFont(QFont("times", 27));
    mGameStatusLabel->setPos(WINDOW_WIDTH - mGameStatusLabel->boundingRect().width() - 5,
                             WINDOW_HEIGHT - mGameStatusLabel->boundingRect().height() - 5);
    mGameStatusLabel->setDefaultTextColor(Qt::blue);
    mScene->addItem(mGameStatusLabel);

    mEnemyTimer = new QTimer;
    connect(mEnemyTimer, SIGNAL(timeout()), this, SLOT(createEnemy()));
    mEnemyTimer->start(ENEMY_CREATION_TIMER_DELAY);
}

void Game::run()
{
    this->show();
    this->centralize();
}

void Game::keyReleaseEvent(QKeyEvent *event)
{
    switch(event->key())
    {
        case Qt::Key_Space:
            if(mStatus == Status::PLAY)
            {
                if(mEnemyTimer->isActive())
                {
                    mEnemyTimer->stop();
                }
                mStatus = Status::PAUSE;
                updateStatusLabel();
            }
            else if(mStatus == Status::PAUSE)
            {
                if(!mEnemyTimer->isActive())
                {
                    mEnemyTimer->start(ENEMY_CREATION_TIMER_DELAY);
                }
                mStatus = Status::PLAY;
                updateStatusLabel();
            }
        break;
        case Qt::Key_N:
            startNewGame();
            break;
        default:
            QGraphicsView::keyReleaseEvent(event);
        break;
    }
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
        pen.setWidth(ROAD_WIDTH);
        section->setPen(pen);
        mScene->addItem(section);
    }
}

void Game::startNewGame()
{
    if(mEnemyTimer->isActive())
        mEnemyTimer->stop();
    mPlayerScore->reset();
    mEnemyCounter = 0u;
    mNumOfKilledEnemies = 0;
    deactivateBuildMode();
    auto items = mScene->items();
    for(auto &e: items)
        if(typeid(*e) == typeid(Enemy) || typeid(*e) == typeid(Missile) ||
           typeid(*e) == typeid(Tower))
            delete e;
    connect(mEnemyTimer, SIGNAL(timeout()), this, SLOT(createEnemy()));
    mEnemyTimer->start(ENEMY_CREATION_TIMER_DELAY);
    mStatus = Status::PLAY;
    updateStatusLabel();
}

void Game::checkGameStatus()
{
    auto baseColliders = mBase->collidingItems();
    for(auto &e: baseColliders)
    {
        if(typeid(*e) == typeid(Enemy))
        {
            mStatus = Status::DEFEAT;
            updateStatusLabel();
            if(mIsBuildModeActive) deactivateBuildMode();
        }
    }

   if(mNumOfKilledEnemies >= MAX_NUM_OF_ENEMIES)
   {
       mStatus = Status::VICTORY;
       updateStatusLabel();
       if(mIsBuildModeActive) deactivateBuildMode();
   }
}

void Game::updateStatusLabel()
{
    switch(mStatus)
    {
        case Status::PLAY:
            mGameStatusLabel->setPlainText("GAME STATUS: PLAY");
        break;
        case Status::PAUSE:
            mGameStatusLabel->setPlainText("GAME STATUS: PAUSED");
        break;
        case Status::VICTORY:
            mGameStatusLabel->setPlainText("GAME STATUS: YOU WON!!!");
        break;
        case Status::DEFEAT:
            mGameStatusLabel->setPlainText("GAME STATUS: YOU LOST!");
        break;
    }
    mGameStatusLabel->setPos(WINDOW_WIDTH - mGameStatusLabel->boundingRect().width() - 5,
                             WINDOW_HEIGHT - mGameStatusLabel->boundingRect().height() - 5);
}

void Game::createEnemy()
{
    if(mEnemyCounter >= MAX_NUM_OF_ENEMIES)
    {
        disconnect(mEnemyTimer, SIGNAL(timeout()), this, SLOT(createEnemy()));
        return;
    }

    Enemy *enemy = new Enemy(this, QPointF(WINDOW_WIDTH, 0), ENEMY_VELOCITY,
                             mEnemyRoute, SCRORE_FOR_ONE_KILLED_ENEMY);
    connect(enemy, SIGNAL(awardPlayer(int)), this, SLOT(increasePlayerScore(int)));
    ++mEnemyCounter;
    mScene->addItem(enemy);
}

void Game::increasePlayerScore(int increment)
{
    ++mNumOfKilledEnemies;
    mPlayerScore->increase(increment);
    checkGameStatus();
}

void Game::activateBuildMode(Tower::Type typeOfTowerToBeBuilt)
{
    if(!mIsBuildModeActive && !mCursor)
    {
        mIsBuildModeActive = true;
        mTypeOfNextTower = typeOfTowerToBeBuilt;
        switch(typeOfTowerToBeBuilt)
        {
            case Tower::Type::RED:
                mCursor = new QGraphicsPixmapItem(QPixmap(":/gfx/redCursor.png"));
                break;
            case Tower::Type::GREEN:
                mCursor = new QGraphicsPixmapItem(QPixmap(":/gfx/greenCursor.png"));
                break;
            case Tower::Type::YELLOW:
                mCursor = new QGraphicsPixmapItem(QPixmap(":/gfx/yellowCursor.png"));
                break;
        }
        mCursor->setPos(-100, -100);
        mScene->addItem(mCursor);
    }
}

void Game::deactivateBuildMode()
{
    mIsBuildModeActive = false;
    mScene->removeItem(mCursor);
    delete mCursor;
    mCursor = nullptr;
}

void Game::createTower(Tower::Type type, const QPointF &pos)
{
    Tower *newTower = new Tower(this, type, pos);
    mScene->addItem(newTower);
}

void Game::mouseReleaseEvent(QMouseEvent *event)
{
    if(mStatus != Status::PLAY) return;
    if(mIsBuildModeActive && mCursor &&
       event->button() == Qt::MouseButton::LeftButton)
    {
        auto colliders = mCursor->collidingItems();
        for(const auto& c: colliders)
        {
            if(typeid(*c) == typeid(Tower))
                return;
        }
        createTower(mTypeOfNextTower, event->pos());
        deactivateBuildMode();
    }
    else
    {
        if(!items().empty())
        {
            QPointF scenePoint = mapToScene(event->pos());
            QGraphicsItem *releasedItem = mScene->itemAt(scenePoint, transform());
            BuildingModeIcon* releasedIcon = dynamic_cast<BuildingModeIcon*>(releasedItem);
            if(releasedIcon && event->button() == Qt::MouseButton::LeftButton)
            {
                activateBuildMode(releasedIcon->getTowerType());
            }
        }
    }
}

void Game::mouseMoveEvent(QMouseEvent *event)
{
    if(mStatus != Status::PLAY) return;
    if(mIsBuildModeActive && mCursor)
    {
        mCursor->setPos(event->x()/* - mCursor->pixmap().width() / 2*/,
                        event->y()/* - mCursor->pixmap().height() / 2)*/);
    }
}
