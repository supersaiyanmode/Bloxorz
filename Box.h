#ifndef BOX_H
#define BOX_H

class Box {
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
    void paint();
    
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
};

#endif