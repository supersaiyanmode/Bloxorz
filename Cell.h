#ifndef CELL_H
#define CELL_H

#include <GL/gl.h>

enum CellType{
    CELL_BAD=0,
    CELL_HOLE=1,
    CELL_FLOOR=2,
    CELL_SWITCH=4
};

class Cell {    
    int cellType;
    double initPosX, initPosY, initPosZ;
    double newPosX, newPosY, newPosZ;
    
    int loading; //count from 1 to 10;
    int loadingDir;
    
    static bool useGLList;
    static GLuint cellFloorList;
    
    enum {STEPS = 5};
    
    void paintCell();
public:
    
    Cell(char, int, int);
    ~Cell();
    
    void load();
    void unload();
    
    static void setUseList(bool);
    
    int type() const;
    void setType(int);
    
    void init();
    bool paint();
};

#endif