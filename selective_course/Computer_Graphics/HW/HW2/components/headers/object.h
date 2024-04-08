#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED

#include <string>
#include <vector>
using namespace std;

typedef struct {
    double x;
    double y;
    double z;
} tup;

class Object {
    public:
        Object(string tag, double px, double py, double pz, double tx, double ty, double tz, tup scales);
        double px;
        double py;
        double pz;
        double tx;
        double ty;
        double tz;
        double rx;
        double ry;
        double rz;
        double ax;
        double ay;
        double az;
        tup scales;
        string tag;
        vector<Object*> children;
        void getChildren(vector<Object*> children);
        void render();
};

#endif