#include <GL/glfw.h>
#include <GL/glext.h>
#include <math.h>
#include <stdio.h>
#include <iostream>
#include <sstream>
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
    float aspectRatio = double(windowWidth)/windowHeight;
    moves = 0;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0,aspectRatio,1,50.0);
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glFrontFace(GL_CW);

    glMatrixMode(GL_MODELVIEW);
 
    int r1,c1,r2,c2;
    map.init();
    map.getBox(r1,c1,r2,c2);
    box = Box(r1,c1,r2,c2,this);
    box.init();
    
    load();
    invalidate();
    
    mustInit = false;
}

void Game::loop(){
    while (glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS && glfwGetWindowParam(GLFW_OPENED)){
        if (invalid || mustInit){
            if (mustInit)
                init();
            
            while (invalid){
                glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
                invalid = paint();
                glfwSwapBuffers();
            }
            
//             //print the state!
//             std::cout<<"---------STATE----------------\n";
//             std::vector<std::vector<char> > state = map.state();
//             int r1,c1,r2,c2;
//             box.get(r1,c1,r2,c2);
//             state[r1][c1] = '+';
//             if (r2 != -1)
//                 state[r2][c2] = '+';
//             for (int i=0; i<state.size(); i++){
//                 for (int j=0; j<state[i].size(); j++){
//                     std::cout<<state[i][j];
//                 }
//                 std::cout<<std::endl;
//             }
//             //invalid = true;
        }else{
            glfwSwapBuffers();
        }
        //glfwPollEvents();
        processKeys();
    }
}


void Game::invalidate(){
    invalid = true;
}
bool Game::paint(){
    bool stillInvalid = false;
    
    glLoadIdentity();
    
    gluLookAt(box.getXLocation(), 12, 30, box.getXLocation()+5, 0, -10, 0, 1, 0);

    if (map.paint() && !stillInvalid){
        stillInvalid = true;
    }else{
        if (box.paint() && !stillInvalid)
            stillInvalid = true;
    }
    std::stringstream ss;
    ss<<"Bloxorz [Moves: "<<moves<<"]";
    glfwSetWindowTitle(ss.str().c_str());

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
        mustInit = true;
        invalid = true;
        //may be more stuff here.. like load next level???
    }else{
        std::cout<<"YOU FELL OFF!!\n";
        init();
    }
}

void Game::requestInit(){
    mustInit = true;
}