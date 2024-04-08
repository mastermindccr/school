#ifndef VERTEX_H_INCLUDED
#define VERTEX_H_INCLUDED

#include <GL/gl.h>
#include <GL/glut.h>

class Vertex{
    public:
        double x;
        double y;
        double z;
        Vertex(double x, double y, double z);
        Vertex operator-(Vertex a);
        Vertex operator*(Vertex a);
        double operator^(Vertex a);
        void normalize();
};

#endif