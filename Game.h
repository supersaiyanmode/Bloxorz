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
    
    BloxorzMap& getMap();
    
    void load(); //typically the animation of the map
    void unload();
    void over(int); //param is the status of gameover..
    
    
    void init();
    void loop();
    bool paint(); //returns if its still invalid..
    void invalidate();
    void processKeys();
};

#endif 