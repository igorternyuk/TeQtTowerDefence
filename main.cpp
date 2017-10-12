#include <QApplication>
#include "game.hpp"
#include <memory>
#include <stdexcept>
#include <iostream>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyle("fusion");

   try
    {
        Game *game = new Game;
        game->run();
    }
    catch(std::exception &ex)
    {
        std::cout << ex.what() << std::endl;
    }
    return a.exec();
}
