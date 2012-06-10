#ifndef BOX_H
#define BOX_H

class Box {
    int row1, col1, row2, col2;
    
    double posX, posY, posZ;
    double rotX, rotZ;
    double newPosX, newPosY, newPosZ;
    double newRotX, newRotZ;
    
    bool falling;
    int animating; //0 if not animating, else progress [1-10]
    
    enum Alignment{ALIGN_X, ALIGN_Y, ALIGN_Z} align, newAlign;
public:
    Box(int,int,int,int);
    ~Box();
    
    void init();
    bool paint();
    
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    
    void get(int&, int&, int&, int&);
};

#endif