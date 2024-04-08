#include <GL/gl.h>
#include <GL/glut.h>
#include <algorithm>
#include <iostream>
#include "components/headers/controller.h"

#define row 10
#define col 10
#define ball_num 6

using namespace std;

Controller* ctrl = new Controller(ball_num, row, col);

void init(){
    glClearColor(0, 0, 0, 1);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, row, 0, col, -2, 2);
    glMatrixMode(GL_MODELVIEW);
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    ctrl->show();
    glutSwapBuffers();
    glutPostRedisplay();
};

void keyboard(unsigned char key, int x, int y){
    if(ctrl->view==0){
        if(key==13 || key==108){
            ctrl->menu_select();
        }
    }
    else if(ctrl->view==1){
        if(key==27){
            ctrl->view = 0;
        }
    }
    else if(ctrl->view==2){
        if(!ctrl->game->status){
            if(key=='p' || key==27) ctrl->view = 3;
        }
        else{
            if(key=='Y' || key=='y'){
                ctrl->game->init();
                ctrl->view = 2;
            }
            else if(key=='N' || key=='n'){
                ctrl->view = 0;
            }
        }
    }
    else if(ctrl->view==3){
        if(key==13 || key==108){
            ctrl->pause_select();
            ctrl->pause->choice = 0;
        }
    }
}

void special_keyboard(int key, int x, int y){
    if(ctrl->view==0){
        ctrl->menu->move(key);
    }
    else if(ctrl->view==2){
        if(!ctrl->game->status) ctrl->game->bowl->move(key, row);
    }
    else if(ctrl->view==3){
        ctrl->pause->move(key);
    }
}

void timer(int value){
    if(ctrl->view==2 && ctrl->game->time==0){
        ctrl->game->status = 1;
    }
    if(ctrl->view==2 && !ctrl->game->status){
        ctrl->game->time--;
    }
    glutTimerFunc(1000, timer, 0);
}

int main(int argc, char* argv[]){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Test");
    init();
    glutDisplayFunc(display);
    glEnable(GL_DEPTH_TEST);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(special_keyboard);
    glutTimerFunc(0, timer, 0);
    glutMainLoop();
    return 0;
}