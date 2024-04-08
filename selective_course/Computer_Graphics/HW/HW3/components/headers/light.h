#define LIGHT_H_INCLUDED
#ifndef LIGHT_H_INCLUDE

#include <vector>
#include <GL/gl.h>
#include <GL/glut.h>

using namespace std;

class Light{
    public:
        GLfloat ambient[3];
        GLfloat diffuse[3];
        GLfloat position[3];
        Light(GLfloat ar, GLfloat ag, GLfloat ab, GLfloat dr, GLfloat dg, GLfloat db, GLfloat pr, GLfloat pg, GLfloat pb);
};

#endif