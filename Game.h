#ifndef GAME_H
#define GAME_H

#include <string>

#include "BloxorzMap.h"
#include "Box.h"

class Game{
    int windowWidth, windowHeight;
    bool invalid;
    BloxorzMap map;
    Box box;
public:
    Game(const std::string&, int, int);
    ~Game();
    
    void init();
    void loop();
    bool paint(); //returns if its still invalid..
    void processKeys();
};

#endif 