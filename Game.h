#ifndef GAME_H
#define GAME_H

#include <string>

#include "BloxorzMap.h"
#include "Box.h"

class Game{
    int windowWidth, windowHeight;
    BloxorzMap map;
    Box box;
public:
    Game(const std::string&, int, int);
    ~Game();
    
    void init();
    void loop();
    void paint();
    void processKeys();
};

#endif 