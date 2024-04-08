#include "headers/dino.h"
#include <cmath>

class Vertex;

Dino::Dino(ifstream& file){
    x_min = y_min = z_min = x_max = y_max = z_max = max = rx = ry = 0;
    string prefix;
    string line;
    while(getline(file, line)){
        stringstream ss(line);
        ss >> prefix;
        if(prefix=="v"){
            double x, y, z;
            ss >> x >> y >> z;
            position.push_back(Vertex(x, y, z));
            if(x<x_min)
                x_min = x;
            if(x>x_max)
                x_max = x;
            if(y<y_min)
                y_min = y;
            if(y>y_max)
                y_max = y;
            if(z<z_min)
                z_min = z;
            if(z>z_max)
                z_max = z;
        }
        else{
            int x, y, z;
            ss >> x >> y >> z;
            faces.push_back(Face(x, y, z));
        }
    }
    if(x_max-x_min>max)
        max = x_max - x_min;
    if(y_max-y_min>max)
        max = y_max - y_min;
    if(z_max-z_min>max)
        max = z_max - z_min;
}

void Dino::draw(){
    glPushMatrix();
    glScaled(1/max, 1/max, 1/max);
    if(moving) glRotated(angle, rx, ry, rz);
    glBegin(GL_TRIANGLES);
    for (auto &i : faces){
        Vertex a = position[i.x - 1];
        Vertex b = position[i.y - 1];
        Vertex c = position[i.z - 1];

        Vertex v1 = b - a;
        Vertex v2 = c - a;
        Vertex normal = v1 * v2;

        glNormal3d(normal.x, normal.y, normal.z);
        glVertex3d(a.x, a.y, a.z);
        glVertex3d(b.x, b.y, b.z);
        glVertex3d(c.x, c.y, c.z);
        
    }
    glEnd();
    glPopMatrix();
}

void Dino::calculate_rotate(int x, int y){
    vector<double> tmp = normalized_coord(x, y);
    double dist = sqrt(tmp[0] * tmp[0] + tmp[1] * tmp[1]);
    double z = cos((3.14 / 2.0) * ((dist < 1.0) ? dist : 1.0));

    Vertex v2 = Vertex(tmp[0], tmp[1], z);
    v2.normalize();

    Vertex n = v1 * v2;
    Vertex d = v2 - v1;

    angle = 90.0 * sqrt(d.x*d.x + d.y*d.y + d.z*d.z);
    rx = n.x;
    ry = n.y;
    rz = n.z;
}

void Dino::record_rotate(int x, int y){
    glRotated(angle, rx, ry, rz);
    moving = false;
    angle = 0.0;
}

void Dino::set_last(int x, int y){
    vector<double> tmp = normalized_coord(x, y);
    double dist = sqrt(tmp[0] * tmp[0] + tmp[1] * tmp[1]);
    double z = cos((3.14 / 2.0) * ((dist < 1.0) ? dist : 1.0));
    v1 = Vertex(tmp[0], tmp[1], z);
    v1.normalize();
    moving = true;
}

vector<double> Dino::normalized_coord(int x, int y){
    return {(double)x / 250.0 - 1, 1 - (double)y / 250.0};
}