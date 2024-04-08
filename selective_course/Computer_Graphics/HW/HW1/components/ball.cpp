#include <GL/gl.h>
#include <GL/glut.h>
#include <cstdlib>
#include <ctime>
#include "headers/ball.h"

Ball::Ball(float row_len, float col_len, int i){
    srand (time(0)+7*i);
    (*this).row_len = row_len;
    (*this).col_len = col_len;
    init();
}

void Ball::init(){
    rand();
    x = (float)rand() * row_len / RAND_MAX;
    y = col_len + (float)rand() * col_len / RAND_MAX;
    type = rand() % 50;
    if(type<=30){ // general
        speed = 0.0012+(float)rand() * 0.0008 / RAND_MAX;
        point = 5;
        color = {1, 0, 0};
    }
    else if(type<=37){ // fast
        speed = 0.002+(float)rand() * 0.001 / RAND_MAX;
        point = 10;
        color = {1, 1, 0};
    }
    else if(type<=47){ // bomb
        speed = 0.001+(float)rand() * 0.0005 / RAND_MAX;
        point = -1;
        color = {1, 1, 1};
    }
    else{ // big basket
        speed = 0.0025+(float)rand() * 0.0005 / RAND_MAX;
        point = 100;
        color = {1, 0, 1};
    }
}

void Ball::draw(int& status, int& total, float center, float& width, float& height){
    if(center-width<=x && x<=center+width && y<=height){ // touched
        if(point==100){
            width += 0.2;
        }
        else if(point==-1){
            status = 1;
        }
        else{
            total += point;
        }
        init();
    }
    if(y<0){
        init();
    }
    glPushMatrix();
    glColor3f(color.r, color.g, color.b);
    glTranslatef(x, y, 0);
    glutSolidSphere(0.5, 25, 25);
    y -= speed;
    glPopMatrix();
}