#ifndef CELL_H
#define CELL_H

enum CellType{
    CELL_HOLE=1,
    CELL_FLOOR=2,
    CELL_SWITCH=4
};

class Cell {
private:
    int cellType;
    double posX, posZ;
    
    void paintCell();
public:
    
    Cell(char, int, int);
    ~Cell();
    
    int type() const;
    void setType(int);
    
    void init();
    void paint();
};

#endif