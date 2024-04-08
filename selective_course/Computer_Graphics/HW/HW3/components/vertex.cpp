#include "headers/vertex.h"
#include <cmath>

Vertex::Vertex(double x, double y, double z){
    this->x = x;
    this->y = y;
    this->z = z;
}

Vertex Vertex::operator -(Vertex a){
    return Vertex(x - a.x, y - a.y, z - a.z);
}

Vertex Vertex::operator *(Vertex a){
    return Vertex(y * a.z - z * a.y, z * a.x - x * a.z, x * a.y - y * a.x);
}

double Vertex::operator ^(Vertex a){
    return x * a.x + y * a.y + z * a.z;
}

void Vertex::normalize(){
    double length = sqrt(x * x + y * y + z * z);
    x /= length;
    y /= length;
    z /= length;
}