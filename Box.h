#ifndef BOX_H
#define BOX_H

class Game;

class Box {
    int row1, col1, row2, col2;
    
    double posX, posY, posZ;    //current position
    double rotX, rotZ;
    double newPosX, newPosY, newPosZ; //next position in case of animation
    double newRotX, newRotZ;
    double deltaFulcrumX, deltaFulcrumY, deltaFulcrumZ; //point about which the box turns
    
    int falling;        //if falling when won.
    int animating;      //0 if not animating, else progress [1-10]
    
    
    enum {ANIMATION_STEPS = 20};
    enum Alignment{ALIGN_X, ALIGN_Y, ALIGN_Z} align, newAlign; //alignment of the box
    
    Game* game;         //Used for calling functions during state-change
public:
    Box(int,int,int,int, Game*);
    ~Box();
    
    void init();
    bool paint();
    
    void moveLeft();
    void moveRight();
    void moveUp();
    void moveDown();
    
    void get(int&, int&, int&, int&);
    double getXLocation();      //Used for moving the camera along with the box.
};

#endif