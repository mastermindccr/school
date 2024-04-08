#include "components/headers/object.h"
#include <GL/gl.h>
#include <GL/glut.h>
#include <algorithm>
#include <iostream>
#define row 10
#define col 10
#define depth 10

using namespace std;

tup torso_scale = {2.5, 5.0, 1.5};
tup head_scale = {0.75, 0.75, 1.0};
tup uarm_scale = {1.25, 0.75, 0.75};
tup darm_scale = {1.5, 0.5, 0.5};
tup uleg_scale = {0.8, 1.2, 0.75};
tup dleg_scale = {0.4, 1.5, 0.5};

Object* torso = new Object("torso", 5, 5, 0, 0, 0, 0, torso_scale);
Object* head = new Object("head", 0, 3.2, 0, 0, 0, 0, head_scale);
Object* ularm = new Object("uarm", -torso_scale.x/2, (torso_scale.y-uarm_scale.y)/2, 0, -uarm_scale.x/2, 0, 0, uarm_scale);
Object* dlarm = new Object("darm", -uarm_scale.x/2, 0, 0, -darm_scale.x/2, 0, 0, darm_scale);
Object* urarm = new Object("uarm", torso_scale.x/2, (torso_scale.y-uarm_scale.y)/2, 0, uarm_scale.x/2, 0, 0, uarm_scale);
Object* drarm = new Object("darm", uarm_scale.x/2, 0, 0, darm_scale.x/2, 0, 0, darm_scale);
Object* ulleg = new Object("uleg", -(torso_scale.x-uleg_scale.x)/2, -torso_scale.y/2, 0, 0, -uleg_scale.y/2, 0, uleg_scale);
Object* dlleg = new Object("dleg", 0, -uleg_scale.y/2, 0, 0, -dleg_scale.y/2, 0, dleg_scale);
Object* urleg = new Object("uleg", (torso_scale.x-uleg_scale.x)/2, -torso_scale.y/2, 0, 0, -uleg_scale.y/2, 0, uleg_scale);
Object* drleg = new Object("dleg", 0, -uleg_scale.y/2, 0, 0, -dleg_scale.y/2, 0, dleg_scale);

int mode = 0; // 0: none, 1: torso, 2: ularm, 3: dlarm ...
int axis = 0; // 0: x, 1: y, 2: z
string explanation;
Object *tmp;

void init(){
    torso->getChildren({head, ularm, urarm, ulleg, urleg});
    ularm->getChildren({dlarm});
    urarm->getChildren({drarm});
    ulleg->getChildren({dlleg});
    urleg->getChildren({drleg});

    glClearColor(0, 0, 0, 1);
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, row, 0, col, -depth/2, depth/2);
    glMatrixMode(GL_MODELVIEW);

    explanation = "select: ";
    if(mode == 0){
        explanation += "none";
    }
    else if(mode == 1){
        explanation += "torso";
        tmp = torso;
    }
    else if(mode == 2){
        explanation += "left upper arm";
        tmp = ularm;
    }
    else if(mode == 3){
        explanation += "left lower arm";
        tmp = dlarm;
    }
    else if(mode == 4){
        explanation += "right upper arm";
        tmp = urarm;
    }
    else if(mode == 5){
        explanation += "right lower arm";
        tmp = drarm;
    }
    else if(mode == 6){
        explanation += "left upper leg";
        tmp = ulleg;
    }
    else if(mode == 7){
        explanation += "left lower leg";
        tmp = dlleg;
    }
    else if(mode == 8){
        explanation += "right upper leg";
        tmp = urleg;
    }
    else {
        explanation += "right lower leg";
        tmp = drleg;
    }
    explanation += ", axis: ";
    if(axis==0){
        explanation += "x";
    }
    else if(axis==1){
        explanation += "y";
    }
    else{
        explanation += "z";
    }
    glRasterPos2f(5-explanation.size()/11.0, 9.5);
    for (int i = 0; i < explanation.size(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, explanation[i]);
    }

    glLoadIdentity();
    torso->render();
    glutSwapBuffers();
    glutPostRedisplay();
};

void keyboard(unsigned char key, int x, int y){
    if(48<=key && key<=57){
        mode = key - 48;
    }
    else if(96<=key && key<=105){
        mode = key - 96;
    }
    else if(key=='x'){
        axis = 0;
    }
    else if(key=='y'){
        axis = 1;
    }
    else if(key=='z'){
        axis = 2;
    }
    else{
        return;
    }
}

void special_keyboard(int key, int x, int y){
    if(key==GLUT_KEY_LEFT){
        if(tmp==nullptr)
            return;
        if(axis==0 && tmp->ax<90){
            tmp->ax += 2;
        }
        else if(axis==1 && tmp->ay<90){
            tmp->ay += 2;
        }
        else if(axis==2 && tmp->az<90){
            tmp->az += 2;
        }
    }
    else if(key==GLUT_KEY_RIGHT){
        if(tmp==nullptr)
            return;
        if(axis==0 && tmp->ax>-90){
            tmp->ax -= 2;
        }
        else if(axis==1 && tmp->ay>-90){
            tmp->ay -= 2;
        }
        else if(axis==2 && tmp->az>-90){
            tmp->az -= 2;
        }
    }
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
    glutMainLoop();
    return 0;
}