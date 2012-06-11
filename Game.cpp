#include <GL/glfw.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include "Game.h"

Game::Game(const std::string& file, int w, int h):
            invalid(true),windowWidth(w), windowHeight(h),moves(0),
            mapFile(file), map(file),box(0,0,0,1,this){
    if (!glfwInit() || !glfwOpenWindow(windowWidth,windowHeight, 8,8,8,8,24,0,GLFW_WINDOW)){
        glfwTerminate();
        throw 1;
    }
}

Game::~Game(){
    glfwTerminate();
}


BloxorzMap& Game::getMap(){
    return map;
}
void Game::init(){
    std::cout<<"Initiating..";
    float aspectRatio = double(windowWidth)/windowHeight;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0,aspectRatio,1,50.0);
    
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glFrontFace(GL_CW);
 
    int r1,c1,r2,c2;
    map.init();
    map.getBox(r1,c1,r2,c2);
    box = Box(r1,c1,r2,c2,this);
    box.init();
    
    load();
    invalidate();
}

void Game::loop(){
    while (glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS){
        if (invalid){
            while (invalid){
                glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
                invalid = paint();
                glfwSwapBuffers();
            }
            
            //print the state!
            std::cout<<"---------STATE----------------\n";
            std::vector<std::vector<char> > state = map.state();
            int r1,c1,r2,c2;
            box.get(r1,c1,r2,c2);
            state[r1][c1] = '+';
            if (r2 != -1)
                state[r2][c2] = '+';
            for (int i=0; i<state.size(); i++){
                for (int j=0; j<state[i].size(); j++){
                    std::cout<<state[i][j];
                }
                std::cout<<std::endl;
            }
            
        }else{
            glfwSwapBuffers();
        }
        //glfwPollEvents();
        processKeys();
    }
}


void Game::invalidate(){
    invalid = true;
    std::cout<<"INVALIDATEDD!!\n";
}
bool Game::paint(){
    bool stillInvalid = false;
    
    glLoadIdentity();
#if 0
    static double angle = 0.0, radius=25;
    double lookX = 10, lookY=0, lookZ=10;
    gluLookAt(lookX + radius*cos(angle),15,lookZ + radius*sin(angle), lookX, lookY, lookZ, 0,1,0);
    angle += 0.01;
    stillInvalid = true;
#else
    gluLookAt(7, 12, 28, 7, 0, 0, 0, 1, 0);
#endif
    if (map.paint() && !stillInvalid){
        stillInvalid = true;
    }else{
        if (box.paint() && !stillInvalid)
            stillInvalid = true;
    }

    return stillInvalid;
}

void Game::processKeys(){
    if (glfwGetKey(GLFW_KEY_LEFT)==GLFW_PRESS){
        invalid = true;
        moves++;
        box.moveLeft();
    }
    else if (glfwGetKey(GLFW_KEY_RIGHT)==GLFW_PRESS){
        invalid = true;
        moves++;
        box.moveRight();
    }
    else if (glfwGetKey(GLFW_KEY_UP)==GLFW_PRESS){
        invalid = true;
        moves++;
        box.moveUp();
    }
    else if (glfwGetKey(GLFW_KEY_DOWN)==GLFW_PRESS){
        invalid = true;
        moves++;
        box.moveDown();
    }
    else if (glfwGetKey('R')==GLFW_PRESS){
        init();
        invalid = true;
    }
}

void Game::load(){
    map.load();
}
void Game::unload(){
    map.unload();
}

void Game::over(int status){
    if (status == 1){ //WIN!!
        std::cout<<"WIN!!\n";
        init();
        //may be more stuff here.. like load next level???
    }else{
        std::cout<<"YOU FELL OFF!!\n";
        init();
    }
}
