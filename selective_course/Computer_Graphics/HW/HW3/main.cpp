#include <GL/gl.h>
#include <GL/glut.h>
#include <fstream>
#include <sstream>
#include <vector>
#include "components/headers/dino.h"
#include "components/headers/light.h"
#define viewport 0.6

using namespace std;

Dino* dino;

void init(){
    glClearColor(0, 0, 0, 1);
    glEnable(GL_NORMALIZE);
    glEnable(GL_LIGHTING);
    glShadeModel(GL_SMOOTH);

    vector<Light*> lights;
    lights.push_back(new Light(0.2, 0.3, 0.2, 0.6, 0.8, 0.6, 1.0, -0.6, 0.3));
    lights.push_back(new Light(0.3, 0.2, 0.3, 0.6, 0.8, 0.6, -1.0, -0.6, 0.3));
    lights.push_back(new Light(0.2, 0.2, 0.2, 0.2, 0.2, 0.2, 0.0, -1.0, 0.7));

    for (int i = 0; i < lights.size();i++){
        glLightfv(0x4000 + i, GL_AMBIENT, lights[i]->ambient);
        glLightfv(0x4000 + i, GL_DIFFUSE, lights[i]->diffuse);
        glLightfv(0x4000 + i, GL_POSITION, lights[i]->position);
        glEnable(0x4000 + i);
    }

    ifstream file;
    file.open("Dino.obj");
    dino = new Dino(file);
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    dino->draw();

    glutSwapBuffers();
    glutPostRedisplay();
    
};

void reshape(int width, int height){
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-viewport, viewport, -viewport, viewport, -viewport, viewport);
    glMatrixMode(GL_MODELVIEW);
}

void motion(int x, int y){
    dino->calculate_rotate(x, y);
}

void mouse(int button, int state, int x, int y){
    if(button==GLUT_LEFT_BUTTON && state==GLUT_DOWN){
        dino->set_last(x, y);
    }
    if(button==GLUT_LEFT_BUTTON && state==GLUT_UP){ // release -> apply rotation matrix to record
        dino->record_rotate(x, y);
    }
}

int main(int argc, char* argv[]){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
    glutInitWindowSize(500, 500);
    glutCreateWindow("Dino");
    init();
    glutDisplayFunc(display);
    glutMouseFunc(mouse);
    glutMotionFunc(motion);
    glutReshapeFunc(reshape);
    glEnable(GL_DEPTH_TEST);
    glutMainLoop();
    return 0;
}