#ifndef BLOXORZMAP_H
#define BLOXORZMAP_H
#include <string>
#include <vector>

#include "Cell.h"
#include "Box.h"

class BloxorzMap {
    std::string mapFile;
    std::vector<std::string> mapStr;
    std::vector<std::vector<Cell> > map;
public:
    BloxorzMap(const std::string&);
    
    Cell get(int, int) const;
    void getBox(int &, int&, int&, int&) const;
    
    void init();
    void paint();
};

#endif