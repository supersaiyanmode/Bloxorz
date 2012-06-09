#include <string>
#include <iostream>
#include <map>
#include <stdio.h>
#include <stdlib.h>
#include <GL/gl.h>

#include "Box.h"
#include "soil/SOIL.h"

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
            glBindTexture(GL_TEXTURE_2D, loadTexture("images/BoxTexture.bmp"));
            glBegin(type);
                glNormal3fv(&n[i][0]);
                glTexCoord2f(1,0);  glVertex3fv(&v[faces[i][0]][0]);
                glTexCoord2f(1,1);  glVertex3fv(&v[faces[i][1]][0]);
                glTexCoord2f(1,0);  glVertex3fv(&v[faces[i][2]][0]);
                glTexCoord2f(0,0);  glVertex3fv(&v[faces[i][3]][0]);
            glEnd();
        }
    }
}

extern double CELL_WIDTH;

Box::Box(int r1, int c1, int r2, int c2):falling(false),animating(0){
    extern double CELL_WIDTH;
    //make the box!
    if (r2 == -1){
        //box = Box(r1*CELL_WIDTH, 0, c1*CELL_WIDTH, 0, 0, 0);
        newPosX = posX = c1*CELL_WIDTH;
        newPosY = posY = CELL_WIDTH*1.5;
        newPosZ = posZ = r1*CELL_WIDTH;
        newRotX = rotX = 0;
        //newRotY = rotY = 0;
        newRotZ = rotZ = 0;
        newAlign = align = ALIGN_Y;
    }else{
        if (r1==r2){
            //box = Box(CELL_WIDTH*(c1+c2)/2.0, 0, CELL_WIDTH*r1, 0, 0, 90);
            newPosX = posX = CELL_WIDTH*(c1+c2)/2.0;
            newPosY = posY = CELL_WIDTH;
            newPosZ = posZ = r1*CELL_WIDTH;
            newRotX = rotX = 0;
            //newRotY = rotY = 0;
            newRotZ = rotZ = 90;
            newAlign = align = ALIGN_X;
        }
        else{
            //box = Box(CELL_WIDTH*c1, 0, CELL_WIDTH*(r1+r2)/2.0, 90, 0, 0);
            newPosX = posX = CELL_WIDTH*c1;
            newPosY = posY = CELL_WIDTH;
            newPosZ = posZ = CELL_WIDTH*(r1+r2)/2.0;
            newRotX = rotX = 90;
            //newRotY = rotY = 0;
            newRotZ = rotZ = 0;
            newAlign = align = ALIGN_Z;
        }
    }
}

Box::~Box(){
    
}
    
void Box::init(){
    
}

void Box::paint(){
    glPushMatrix();
    if (animating){
#define X(A,B,r) ((A) + ((B)-(A))*(r))
        glTranslated(X(posX,newPosX,animating/10.0),
                     X(posY,newPosY,animating/10.0),
                     X(posZ,newPosZ,animating/10.0));
        glRotated(X(rotX,newRotX,animating/10.0), 1, 0, 0);
        glRotated(X(rotZ,newRotZ,animating/10.0), 0, 0, 1);
        glScaled(1,2,1);
        drawBox(CELL_WIDTH, GL_QUADS);
        animating = (animating+1)%10;
        if (!animating){
            posX = newPosX; posY = newPosY; posZ = newPosZ;
            rotX = newRotX; /*rotY = newRotY;*/ rotZ = newRotZ;
            align = newAlign;
            std::cout<<"("<<posX<<","<<posY<<","<<posZ<<") | ";
            std::cout<<"["<<align<<"]\n";
        }
#undef X
    }else{
        glTranslated(posX, posY, posZ);
        glRotated(rotX, 1, 0, 0);
        glRotated(rotZ, 0, 0, 1);
        glScaled(1,2,1);
        drawBox(CELL_WIDTH, GL_QUADS);
    }
    glPopMatrix();
}


void Box::moveLeft(){
    animating = 1;
    switch (align){
        case ALIGN_X:
            newPosX = posX - 1.5*CELL_WIDTH;
            newPosY = posY + 0.5*CELL_WIDTH;
            newPosZ = posZ;
            newAlign = ALIGN_Y;
            break;
        case ALIGN_Y:
            newPosX = posX - 1.5*CELL_WIDTH;
            newPosY = posY - 0.5*CELL_WIDTH;
            newPosZ = posZ;
            newAlign = ALIGN_X;
            break;
        case ALIGN_Z:
            newPosX = posX - CELL_WIDTH;
            newPosY = posY;
            newPosZ = posZ;
            newAlign = ALIGN_Z; //no change!
            break;
    }
    newRotX = rotX;
    //newRotY = rotY;
    newRotZ = rotZ + 90;
    std::cout<<"("<<posX<<","<<posY<<","<<posZ<<") -> ";
    std::cout<<"("<<newPosX<<","<<newPosY<<","<<newPosZ<<") | ";
    std::cout<<"["<<align<<"] -> ["<<newAlign<<"]\n";
}

void Box::moveRight(){
    animating = 1;
    switch (align){
        case ALIGN_X:
            newPosX = posX + 1.5*CELL_WIDTH;
            newPosY = posY + 0.5*CELL_WIDTH;
            newPosZ = posZ;
            newAlign = ALIGN_Y;
            break;
        case ALIGN_Y:
            newPosX = posX + 1.5*CELL_WIDTH;
            newPosY = posY - 0.5*CELL_WIDTH;
            newPosZ = posZ;
            newAlign = ALIGN_X;
            break;
        case ALIGN_Z:
            newPosX = posX + CELL_WIDTH;
            newPosY = posY;
            newPosZ = posZ;
            newAlign = ALIGN_Z; //no change!
            break;
    }
    newRotX = rotX;
    //newRotY = rotY;
    newRotZ = rotZ - 90;
    std::cout<<"("<<posX<<","<<posY<<","<<posZ<<") -> ";
    std::cout<<"("<<newPosX<<","<<newPosY<<","<<newPosZ<<") | ";
    std::cout<<"["<<align<<"] -> ["<<newAlign<<"]\n";
}
void Box::moveUp(){
    animating = 1;
    switch (align){
        case ALIGN_X:
            newPosX = posX;
            newPosY = posY;
            newPosZ = posZ - CELL_WIDTH;
            newAlign = ALIGN_X;
            break;
        case ALIGN_Y:
            newPosX = posX;
            newPosY = posY - 0.5*CELL_WIDTH;
            newPosZ = posZ - 1.5*CELL_WIDTH;
            newAlign = ALIGN_Z;
            break;
        case ALIGN_Z:
            newPosX = posX;
            newPosY = posY + 0.5*CELL_WIDTH;
            newPosZ = posZ - 1.5*CELL_WIDTH;
            newAlign = ALIGN_Y; //no change!
            break;
    }
    newRotX = rotX - 90;
    //newRotY = rotY;
    newRotZ = rotZ;
    std::cout<<"("<<posX<<","<<posY<<","<<posZ<<") -> ";
    std::cout<<"("<<newPosX<<","<<newPosY<<","<<newPosZ<<") | ";
    std::cout<<"["<<align<<"] -> ["<<newAlign<<"]\n";
}

void Box::moveDown(){
    animating = 1;
    switch (align){
        case ALIGN_X:
            newPosX = posX;
            newPosY = posY;
            newPosZ = posZ + CELL_WIDTH;
            newAlign = ALIGN_X;
            break;
        case ALIGN_Y:
            newPosX = posX;
            newPosY = posY - 0.5*CELL_WIDTH;
            newPosZ = posZ + 1.5*CELL_WIDTH;
            newAlign = ALIGN_Z;
            break;
        case ALIGN_Z:
            newPosX = posX;
            newPosY = posY + 0.5*CELL_WIDTH;
            newPosZ = posZ + 1.5*CELL_WIDTH;
            newAlign = ALIGN_Y; //no change!
            break;
    }
    newRotX = rotX + 90;
    //newRotY = rotY;
    newRotZ = rotZ;
    std::cout<<"("<<posX<<","<<posY<<","<<posZ<<") -> ";
    std::cout<<"("<<newPosX<<","<<newPosY<<","<<newPosZ<<") | ";
    std::cout<<"["<<align<<"] -> ["<<newAlign<<"]\n";
}