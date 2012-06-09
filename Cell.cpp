#include <map>
#include <algorithm>
#include <string>
#include <iostream>
#include <cstdio>
#include <GL/gl.h>

#include "soil/SOIL.h"
#include "Cell.h"

namespace {
    bool fileExists(std::string fileName){
        FILE* img = NULL;
        img = fopen(fileName.c_str(),"rb");
        if (img) return fclose(img),true;
        return false;
    }
    GLuint loadTexture(std::string s){
        if (!fileExists(s)){
            std::cout<<"Image file: "<<s<<" doesn't exist!"<<std::endl;
            exit(1);
        }
        //use a static map!
        static std::map<std::string, GLuint> textureMap;
        if (textureMap.find(s) != textureMap.end())
            return textureMap[s];
            
        GLuint texture = SOIL_load_OGL_texture(s.c_str(),SOIL_LOAD_AUTO,SOIL_CREATE_NEW_ID,
                SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB |
                SOIL_FLAG_COMPRESS_TO_DXT
            );
        glBindTexture( GL_TEXTURE_2D, texture ); //bind the texture to it's array
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        
        //add to map
        textureMap[s] = texture;
        return texture;
    }
    
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
    
    void drawBox(GLfloat size, GLenum type){
        for (int i = 5; i >= 0; i--) {
            if (i==1)
                glBindTexture(GL_TEXTURE_2D, loadTexture("images/furnitureLight.bmp"));
            else
                glBindTexture(GL_TEXTURE_2D, loadTexture("images/furnitureDark.bmp"));
            glBegin(type);
            glNormal3fv(&n[i][0]);
            glTexCoord2f(1,0);  glVertex3fv(&v[faces[i][0]][0]);
            glTexCoord2f(1,1);  glVertex3fv(&v[faces[i][1]][0]);
            glTexCoord2f(1,0);  glVertex3fv(&v[faces[i][2]][0]);
            glTexCoord2f(0,0);  glVertex3fv(&v[faces[i][3]][0]);
            glEnd();
        }
    }
    
    std::pair<char, int> tempMap[] = {
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
        cellType = -1;
        return;
    }
    cellType = it->second;
    
    posX = colNo * CELL_WIDTH;
    posZ = rowNo * CELL_WIDTH;
}

void Cell::init(){
    glFrontFace(GL_CW);
}

int Cell::type() const{
    return cellType;
}
void Cell::setType(int t){
    cellType = t;
}
    

void Cell::paint(){
    glEnable(GL_DEPTH_TEST);
    glPushMatrix();
    glTranslated(posX, 0, posZ);
    
    if (cellType & CELL_FLOOR){ //draw the floor..
        drawBox(CELL_WIDTH,GL_QUADS);
    }
    glPopMatrix();
}

Cell::~Cell(){
    
}