#include <iostream>

#include "Game.h"

int main(){
    try{
        Game g("map.txt", 600,600);
        g.init();
        g.loop();
    }catch (...){
        std::cout<<"Bummer! Game cant continue..\n";
    }
    return 0;
}