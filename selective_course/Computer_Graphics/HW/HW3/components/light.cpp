#include "headers/light.h"

Light::Light(GLfloat ar, GLfloat ag, GLfloat ab, GLfloat dr, GLfloat dg, GLfloat db, GLfloat pr, GLfloat pg, GLfloat pb){
    ambient[0] = ar;
    ambient[1] = ag;
    ambient[2] = ab;
    diffuse[0] = dr;
    diffuse[1] = dg;
    diffuse[2] = db;
    position[0] = pr;
    position[1] = pg;
    position[2] = pb;
}