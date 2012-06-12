#ifndef BLOXORZMAP_H
#define BLOXORZMAP_H
#include <string>
#include <vector>

#include "Cell.h"
#include "Box.h"

class BloxorzMap {
    int loading;        //loading 0 - steps, for initial animation of maps
    
    GLuint mapList;     //GL list for drawing the map
    
    std::string mapFile;
    std::vector<std::string> mapStr;
    std::vector<std::vector<Cell> > map;
public:
    BloxorzMap(const std::string&);
    
    Cell get(int, int) const;   //Get the cell at location r,c
    void getBox(int &, int&, int&, int&) const; //get the location of the box
    
    void load(); //animation
    void unload();
    
    void init();
    bool paint();
    
    double width() const;
    double height() const;
    
    std::vector<std::vector<char> > state();    //return the state of the map and box.
};

#endif