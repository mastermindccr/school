#include <GL/gl.h>
#include <GL/glut.h>
#include "headers/bowl.h"

Bowl::Bowl(float x, float width, float height){
    (*this).x = x;
    (*this).width = width;
    (*this).height = height;
}

void Bowl::draw(){
    glPushMatrix();
    glTranslatef(x, height, 0);
    glColor3f(0, 1, 0);
    glRotatef(90, 1, 0, 0);
    glutWireCone(width, height, 25, 25);
    glPopMatrix();
}

void Bowl::move(int key, int row_len){
    if(key==GLUT_KEY_LEFT && x>0.2){
        x -= 0.2;
    }
    else if(key==GLUT_KEY_RIGHT && x<row_len-0.2){
        x += 0.2;
    }
}