#ifndef DINO_H_INCLUDED
#define DINO_H_INCLUDED

#include <GL/gl.h>
#include <GL/glut.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include "vertex.h"
#include "face.h"

using namespace std;

class Dino {
    public:
        double x_min;
        double y_min;
        double z_min;
        double x_max;
        double y_max;
        double z_max;
        double max;

        double rx;
        double ry;
        double rz;
        double angle;

        bool moving = false;

        double last_x;
        double last_y;
        Vertex v1 = Vertex(0, 0, 0);
        Dino(ifstream& file);
        vector<Vertex> position;
        vector<Face> faces;

        void draw();
        void rotate(int x, int y);
        void set_last(int x, int y);
        void calculate_rotate(int x, int y);
        void record_rotate(int x, int y);
        vector<double> normalized_coord(int x, int y);
};

#endif