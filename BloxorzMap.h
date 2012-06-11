#ifndef BLOXORZMAP_H
#define BLOXORZMAP_H
#include <string>
#include <vector>

#include "Cell.h"
#include "Box.h"

class BloxorzMap {
    int loading;
    
    GLuint mapList;
    
    std::string mapFile;
    std::vector<std::string> mapStr;
    std::vector<std::vector<Cell> > map;
public:
    BloxorzMap(const std::string&);
    
    Cell get(int, int) const;
    void getBox(int &, int&, int&, int&) const;
    
    void load(); //animation
    void unload();
    
    void init();
    bool paint();
    
    std::vector<std::vector<char> > state();
};

#endif