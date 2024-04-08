#ifndef FACE_H_INCLUDED
#define FACE_H_INCLUDED

#include <GL/gl.h>
#include <GL/glut.h>

class Face{
    public:
        int x;
        int y;
        int z;
        Face(int x, int y, int z);
};

#endif