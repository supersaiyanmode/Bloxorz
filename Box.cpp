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
        std::cout<<"Loaded texture: "<<s<<std::endl;
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
        glDisable(GL_TEXTURE_2D);
        
        glBegin(GL_LINES);
            glColor3f(1,0,0);   glVertex3f(0,0,0); glVertex3f(100,0,0);
            glColor3f(0,1,0);   glVertex3f(0,0,0); glVertex3f(0,100,0);
            glColor3f(0,0,1);glVertex3f(0,0,0); glVertex3f(0,0,1000);
        glEnd();
        glColor3f(1,1,1);
        glEnable(GL_TEXTURE_2D);
        for (int i = 5; i >= 0; i--) {
            glBindTexture(GL_TEXTURE_2D, loadTexture("images/BoxTexture.bmp"));
            glBegin(type);
                glNormal3fv(&n[i][0]);
                glTexCoord2f(1,0);  glVertex3fv(&v[faces[i][0]][0]);
                glTexCoord2f(1,1);  glVertex3fv(&v[faces[i][1]][0]);
                glTexCoord2f(0,1);  glVertex3fv(&v[faces[i][2]][0]);
                glTexCoord2f(0,0);  glVertex3fv(&v[faces[i][3]][0]);
            glEnd();
        }
    }
}

extern double CELL_WIDTH;

Box::Box(int r1, int c1, int r2, int c2):
        row1(r1), col1(c1), row2(r2), col2(c2),
        falling(false),animating(0){
    init();
}

Box::~Box(){
    
}
    
void Box::init(){
    extern double CELL_WIDTH;
    //make the box!
    if (row2 == -1){
        //box = Box(row1*CELL_WIDTH, 0, col1*CELL_WIDTH, 0, 0, 0);
        newPosX = posX = col1*CELL_WIDTH;
        newPosY = posY = CELL_WIDTH*1.5;
        newPosZ = posZ = row1*CELL_WIDTH;
        newRotX = rotX = 0;
        //newRotY = rotY = 0;
        newRotZ = rotZ = 0;
        newAlign = align = ALIGN_Y;
    }else{
        if (row1==row2){
            //box = Box(CELL_WIDTH*(col1+col2)/2.0, 0, CELL_WIDTH*row1, 0, 0, 90);
            newPosX = posX = CELL_WIDTH*(col1+col2)/2.0;
            newPosY = posY = CELL_WIDTH;
            newPosZ = posZ = row1*CELL_WIDTH;
            newRotX = rotX = 0;
            //newRotY = rotY = 0;
            newRotZ = rotZ = 90;
            newAlign = align = ALIGN_X;
        }
        else{
            //box = Box(CELL_WIDTH*col1, 0, CELL_WIDTH*(row1+row2)/2.0, 90, 0, 0);
            newPosX = posX = CELL_WIDTH*col1;
            newPosY = posY = CELL_WIDTH;
            newPosZ = posZ = CELL_WIDTH*(row1+row2)/2.0;
            newRotX = rotX = 90;
            //newRotY = rotY = 0;
            newRotZ = rotZ = 0;
            newAlign = align = ALIGN_Z;
        }
    }
}

bool Box::paint(){
    glPushMatrix();
    const int ANIMATION_STEPS = 50;
    bool stillInvalid;
    if (animating){
#define X(A,B,r) ((A) + ((B)-(A))*(r))
        /*glTranslated(X(posX,newPosX,double(animating)/ANIMATION_STEPS),
                     X(posY,newPosY,double(animating)/ANIMATION_STEPS),
                     X(posZ,newPosZ,double(animating)/ANIMATION_STEPS));
        glRotated(X(rotX,newRotX,double(animating)/ANIMATION_STEPS), 1, 0, 0);
        glRotated(X(rotZ,newRotZ,double(animating)/ANIMATION_STEPS), 0, 0, 1);*/
        
        glTranslated(posX, posY, posZ);
//         glTranslated(X(posX,newPosX,double(animating)/ANIMATION_STEPS),
//                      X(posY,newPosY,double(animating)/ANIMATION_STEPS),
//                      X(posZ,newPosZ,double(animating)/ANIMATION_STEPS));
        
        glTranslated(deltaFulcrumX, deltaFulcrumY, deltaFulcrumZ);
        glRotated(X(0,newRotX-rotX,double(animating)/ANIMATION_STEPS), 1, 0, 0);
        glRotated(X(0,newRotZ-rotZ,double(animating)/ANIMATION_STEPS), 0, 0, 1);
        glTranslated(-deltaFulcrumX, -deltaFulcrumY, -deltaFulcrumZ);
        
        glScaled(1+(align==ALIGN_X),1+(align==ALIGN_Y),1+(align==ALIGN_Z));
        drawBox(CELL_WIDTH, GL_QUADS);
        animating = (animating+1)%ANIMATION_STEPS;
        if (!animating){
            /*posX = newPosX; posY = newPosY; posZ = newPosZ;
            rotX = newRotX; rotZ = newRotZ;
            init();
            align = newAlign;*/
            std::cout<<"("<<posX<<","<<posY<<","<<posZ<<") | ";
            std::cout<<"("<<deltaFulcrumX<<","<<deltaFulcrumY<<","<<deltaFulcrumZ<<") | ";
            init();
            stillInvalid = false;
        }else
            stillInvalid = true;
#undef X
    }else{
        glTranslated(posX, posY, posZ);
        //glRotated(rotX, 1, 0, 0);
        //glRotated(rotZ, 0, 0, 1);
        glScaled(1+(align==ALIGN_X),1+(align==ALIGN_Y),1+(align==ALIGN_Z));
        drawBox(CELL_WIDTH, GL_QUADS);
        stillInvalid = false;
    }
    glPopMatrix();
    return stillInvalid;
}


void Box::moveLeft(){
    animating = 1;
    switch (align){
        case ALIGN_X:
            newPosX = posX - 1.5*CELL_WIDTH;
            newPosY = posY + 0.5*CELL_WIDTH;
            newPosZ = posZ;
            
            deltaFulcrumX = -CELL_WIDTH;
            deltaFulcrumY = -CELL_WIDTH/2.0;
            deltaFulcrumZ = 0;
            
            newAlign = ALIGN_Y;
            
            col1 = std::min(col1, col2)-1;
            row2 = col2 = -1;
            break;
        case ALIGN_Y:
            newPosX = posX - 1.5*CELL_WIDTH;
            newPosY = posY - 0.5*CELL_WIDTH;
            newPosZ = posZ;
            
            deltaFulcrumX = -CELL_WIDTH/2.0;
            deltaFulcrumY = -CELL_WIDTH;
            deltaFulcrumZ = 0;
            
            newAlign = ALIGN_X;
            
            col2 = --col1 - 1;
            row2 = row1;
            break;
        case ALIGN_Z:
            newPosX = posX - CELL_WIDTH;
            newPosY = posY;
            newPosZ = posZ;
            
            deltaFulcrumX = -CELL_WIDTH/2.0;
            deltaFulcrumY = -CELL_WIDTH/2.0;
            deltaFulcrumZ = 0;
            
            newAlign = ALIGN_Z; //no change!
            col1--;
            col2--;
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
            
//             deltaFulcrumX = -CELL_WIDTH;
//             deltaFulcrumY = -CELL_WIDTH/2.0;
            deltaFulcrumX = -CELL_WIDTH/2.0;
            deltaFulcrumY = -CELL_WIDTH;
            deltaFulcrumZ = 0;//-CELL_WIDTH;
            
            newAlign = ALIGN_Y;
            
            col1 = std::max(col1, col2) + 1;
            row2 = col2 = -1;
            break;
        case ALIGN_Y:
            newPosX = posX + 1.5*CELL_WIDTH;
            newPosY = posY - 0.5*CELL_WIDTH;
            newPosZ = posZ;
            
            deltaFulcrumX = CELL_WIDTH/2.0;
            deltaFulcrumY = -CELL_WIDTH;
            deltaFulcrumZ = 0;
            
            newAlign = ALIGN_X;
            
            col2 = ++col1 + 1;
            row2 = row1;
            break;
        case ALIGN_Z:
            newPosX = posX + CELL_WIDTH;
            newPosY = posY;
            newPosZ = posZ;
            
            deltaFulcrumX = CELL_WIDTH/2.0;
            deltaFulcrumY = -CELL_WIDTH/2.0;
            deltaFulcrumZ = 0;
            
            newAlign = ALIGN_Z; //no change!
            
            col1++;
            col2++;
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
            
            deltaFulcrumX = 0;
            deltaFulcrumY = -CELL_WIDTH/2.0;
            deltaFulcrumZ = -CELL_WIDTH/2.0;
            
            newAlign = ALIGN_X;
            
            row1--;
            row2--;
            break;
        case ALIGN_Y:
            newPosX = posX;
            newPosY = posY - 0.5*CELL_WIDTH;
            newPosZ = posZ - 1.5*CELL_WIDTH;
            
            deltaFulcrumX = 0;
            deltaFulcrumY = -CELL_WIDTH;
            deltaFulcrumZ = -CELL_WIDTH/2.0;
            
            newAlign = ALIGN_Z;
            
            row2 = --row1 - 1;
            col2 = col1;
            break;
        case ALIGN_Z:
            newPosX = posX;
            newPosY = posY + 0.5*CELL_WIDTH;
            newPosZ = posZ - 1.5*CELL_WIDTH;
            
            deltaFulcrumX = 0;
            deltaFulcrumY = -CELL_WIDTH/2.0;
            deltaFulcrumZ = -CELL_WIDTH;
            
            newAlign = ALIGN_Y; //no change!
            
            row1 = std::min(row1,row2) - 1;
            row2 = col2 = -1;
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
            
            deltaFulcrumX = CELL_WIDTH/2.0;
            deltaFulcrumY = 0;//-CELL_WIDTH/2.0;
            deltaFulcrumZ = CELL_WIDTH/2.0;
            
            newAlign = ALIGN_X;
            
            row1++;
            row2++;
            break;
        case ALIGN_Y:
            newPosX = posX;
            newPosY = posY - 0.5*CELL_WIDTH;
            newPosZ = posZ + 1.5*CELL_WIDTH;
            
            deltaFulcrumX = 0;
            deltaFulcrumY = -CELL_WIDTH;
            deltaFulcrumZ = CELL_WIDTH/2.0;
            
            newAlign = ALIGN_Z;
            
            row2 = ++row1 + 1;
            col2 = col1;
            break;
        case ALIGN_Z:
            newPosX = posX;
            newPosY = posY + 0.5*CELL_WIDTH;
            newPosZ = posZ + 1.5*CELL_WIDTH;
            
            deltaFulcrumX = 0;
            deltaFulcrumY = -CELL_WIDTH/2.0;
            deltaFulcrumZ = CELL_WIDTH*1.5; //EXTRA MINUS
            
            newAlign = ALIGN_Y; //no change!
            
            row1 = std::max(row2,row1) + 1;
            row2 = col2 = -1;
            break;
    }
    newRotX = rotX + 90;
    //newRotY = rotY;
    newRotZ = rotZ;
    std::cout<<"("<<posX<<","<<posY<<","<<posZ<<") -> ";
    std::cout<<"("<<newPosX<<","<<newPosY<<","<<newPosZ<<") | ";
    std::cout<<"["<<align<<"] -> ["<<newAlign<<"]\n";
}

void Box::get(int&a, int&b, int&c, int&d){
    a = row1;
    b = col1;
    c = row2;
    d = col2;
}