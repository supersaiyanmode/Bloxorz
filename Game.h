#ifndef GAME_H
#define GAME_H

#include <string>

#include "BloxorzMap.h"
#include "Box.h"

class Game{
    bool invalid;
    int windowWidth, windowHeight;
    int moves;
    std::string mapFile;
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