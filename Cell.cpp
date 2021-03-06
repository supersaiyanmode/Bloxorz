#include <map>
#include <algorithm>
#include <string>
#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <GL/gl.h>

#include "soil/SOIL.h"
#include "Cell.h"
#include "loadTexture.h"

GLuint Cell::cellFloorList = 0;
bool Cell::useGLList = true;

extern double CELL_WIDTH;
namespace {    
    const GLfloat n[6][3]= {  /* Normals for the 6 faces of a cube. */
        {-1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {1.0, 0.0, 0.0},
        {0.0, -1.0, 0.0}, {0.0, 0.0, 1.0}, {0.0, 0.0, -1.0}
    };
    const GLfloat n_[6][3]= {  /* Normals for the 6 faces of a cube. */
        {1.0, 0.0, 0.0}, {0.0, -1.0, 0.0}, {-1.0, 0.0, 0.0},
        {0.0, 1.0, 0.0}, {0.0, 0.0, -1.0}, {0.0, 0.0, 1.0}
    };
    
    const GLint faces[6][4] = {/* Vertex indices for the 6 faces of cube. */
        {0, 1, 2, 3}, {3, 2, 6, 7}, {5, 4, 7, 6},
        {4, 5, 1, 0}, {1, 5, 6, 2}, {4, 0, 3, 7}
    };

#if 0                          
                          
     4        5           
                          
  0       1                

                          
     7        6           
                          
  3       2               
                          
#endif
    const GLfloat v[8][3] = {
        {-1,-1,1},{-1,-1,-1},{-1,1,-1},{-1,1,1},
        {1,-1,1},{1,-1,-1},{1,1,-1},{1,1,1}
    };
    
    void drawBox(GLfloat size, std::string frontTextureFile){
        double tileThickness = 0.2;
        glTranslated(0,(CELL_WIDTH-CELL_WIDTH*tileThickness/2)/2.0,0);
        glScaled(1,tileThickness,1);
        for (int i = 5; i >= 0; i--) {
            if (i==1)
                glBindTexture(GL_TEXTURE_2D, loadTexture(frontTextureFile));
            else
                glBindTexture(GL_TEXTURE_2D, loadTexture("images/furnitureDark.bmp"));
            glBegin(GL_QUADS);
            glNormal3fv(&n[i][0]);
            glTexCoord2f(1,0);  glVertex3fv(&v[faces[i][0]][0]);
            glTexCoord2f(1,1);  glVertex3fv(&v[faces[i][1]][0]);
            glTexCoord2f(0,1);  glVertex3fv(&v[faces[i][2]][0]);
            glTexCoord2f(0,0);  glVertex3fv(&v[faces[i][3]][0]);
            glEnd();
        }
    }
    
    std::pair<char, int> tempMap[] = {
        std::make_pair('.', CELL_BAD),
        std::make_pair('X', CELL_FLOOR),
        std::make_pair(' ', CELL_HOLE),
        std::make_pair('+', CELL_FLOOR)
    };
}

const std::map<char, int> CELL_MAP(tempMap, tempMap+ sizeof(tempMap)/sizeof(tempMap[0]));
double CELL_WIDTH = 2.0;

Cell::Cell(char t, int rowNo, int colNo){
    std::map<char, int>::const_iterator it = CELL_MAP.find(t);
    if (it == CELL_MAP.end()){
        cellType = CELL_BAD;
        return;
    }
    cellType = it->second;
    
    //random location to have the raise-up animation during start up.
    newPosX = initPosX = colNo * CELL_WIDTH;
    initPosY = -5.0 - rand()%5;
    newPosZ = initPosZ = rowNo * CELL_WIDTH;
}

void Cell::init(){
    
}

int Cell::type() const{
    return cellType;
}
void Cell::setType(int t){
    cellType = t;
}

void Cell::load(){
    loadingDir = 1;
    loading = 1;
}

void Cell::unload(){
    loadingDir = -1;
    loading = STEPS;
}

void Cell::setUseList(bool b){
    useGLList = b;
}
    

bool Cell::paint(){
    //std::cout<<"Inside cell::Paint!!\n";
    if (!(cellType&CELL_FLOOR))
        return false;

    bool stillInvalid;
    
    glPushMatrix();
#define X(a, b, r) ((a) + ((b)-(a))*(r))
    
    if (loadingDir){
        stillInvalid = true;
        
        glTranslated(initPosX, X(initPosY,newPosY,loading/double(STEPS)), initPosZ);
        if (++loading == STEPS+1){
            loadingDir = 0;
            stillInvalid = false;
        }
    }else{
        glTranslated(newPosX, newPosY, newPosZ);
        stillInvalid = false;
    }
    
    if (cellType & CELL_FLOOR){ //draw the floor..
            if (useGLList){
                if (!cellFloorList){
                    cellFloorList = glGenLists(1);
                    glNewList(cellFloorList, GL_COMPILE);
                        drawBox(CELL_WIDTH,"images/furnitureLight.bmp");
                    glEndList();
                }
                glCallList(cellFloorList);
            }else{
                drawBox(CELL_WIDTH,"images/furnitureLight.bmp");
            }
    }
    
    glPopMatrix();
    return stillInvalid;
}

Cell::~Cell(){
    
}