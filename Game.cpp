#include <GL/glfw.h>
#include <math.h>
#include <stdio.h>
#include "Game.h"

Game::Game(const std::string& file, int w, int h):
            windowWidth(w), windowHeight(h),map(file),box(0,0,0,1){
    if (!glfwInit() || !glfwOpenWindow(windowWidth,windowHeight, 8,8,8,8,24,0,GLFW_WINDOW)){
        glfwTerminate();
        throw 1;
    }
    int r1,c1,r2,c2;
    map.getBox(r1,c1,r2,c2);
    box = Box(r1,c1,r2,c2);
}

Game::~Game(){
    glfwTerminate();
}

void Game::init(){
    float aspectRatio = double(windowWidth)/windowHeight;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(50.0,aspectRatio,1,50.0);
    
    glMatrixMode(GL_MODELVIEW);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    
    map.init();
}

void Game::loop(){
    while (glfwGetKey(GLFW_KEY_ESC) != GLFW_PRESS){
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        paint();
        glfwSwapBuffers();
        glfwPollEvents();
        processKeys();
    }
}

void Game::paint(){
    glLoadIdentity();
#if 0
    static double angle = 0.0, radius=25;
    double lookX = 10, lookY=0, lookZ=10;
    gluLookAt(lookX + radius*cos(angle),15,lookZ + radius*sin(angle), lookX, lookY, lookZ, 0,1,0);
    angle += 0.01;
#else
    gluLookAt(7, 12, 28, 7, 0, 0, 0, 1, 0);
#endif
    map.paint();
    box.paint();
}

void Game::processKeys(){
    if (glfwGetKey(GLFW_KEY_LEFT)==GLFW_PRESS)
        box.moveLeft();
    else if (glfwGetKey(GLFW_KEY_RIGHT)==GLFW_PRESS)
        box.moveRight();
    else if (glfwGetKey(GLFW_KEY_UP)==GLFW_PRESS)
        box.moveUp();
    else if (glfwGetKey(GLFW_KEY_DOWN)==GLFW_PRESS)
        box.moveDown();
}

