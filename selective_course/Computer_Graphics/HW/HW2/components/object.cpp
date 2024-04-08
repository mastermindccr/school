#include "headers/object.h"
#include <GL/gl.h>
#include <GL/glut.h>

Object::Object(string tag, double px, double py, double pz, double tx, double ty, double tz, tup scales){
    this->tag = tag;
    this->px = px;
    this->py = py;
    this->pz = pz;
    this->tx = tx;
    this->ty = ty;
    this->tz = tz;
    this->scales = scales;
    this->rx = 0;
    this->ry = 0;
    this->rz = 0;
    this->ax = 0;
    this->ay = 0;
    this->az = 0;
}

void Object::getChildren(vector<Object*> children){
    this->children = children;
}

void Object::render(){
    glPushMatrix();
    glTranslatef(px, py, pz);
    glRotatef(ax, 1, 0, 0);
    glRotatef(ay, 0, 1, 0);
    glRotatef(az, 0, 0, 1);
    glTranslatef(tx, ty, tz);
    if(tag=="head"){
        glColor3f(0, 0, 1);
        glutSolidSphere(1.2, 25, 25);
    }
    else if(tag=="uarm"){
        glColor3f(1, 0, 0);
        glPushMatrix();
        glScalef(scales.x, scales.y, scales.z);
        glutSolidCube(1);
        glPopMatrix();
    }
    else if(tag=="darm"){
        glColor3f(1, 1, 0);
        glPushMatrix();
        glScalef(scales.x, scales.y, scales.z);
        glutSolidCube(1);
        glPopMatrix();
    }
    else if(tag=="uleg"){
        glColor3f(1, 0, 0);
        glPushMatrix();
        glScalef(scales.x, scales.y, scales.z);
        glutSolidCube(1);
        glPopMatrix();
    }
    else if(tag=="dleg"){
        glColor3f(1, 1, 0);
        glPushMatrix();
        glScalef(scales.x, scales.y, scales.z);
        glutSolidCube(1);
        glPopMatrix();
    }
    else{ // torso
        glColor3f(0, 1, 0);
        glPushMatrix();
        glScalef(scales.x, scales.y, scales.z);
        glutSolidCube(1);
        glPopMatrix();
    }

    for (auto &i : children){
        i->render();
    }
    glPopMatrix();
}