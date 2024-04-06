#ifndef OBJECT_H_INCLUDED
#define OBJECT_H_INCLUDED
#include<string>
using namespace std;

class Object{
public:
    Object();
    Object(string,string);
    virtual void triggerEvent(Object*);
    /* getter & setter */
    void set_name(string);
    void set_tag(string);
    string get_name();
    string get_tag();
private:
    string name;
    string tag;
};


#endif // OBJECT_H_INCLUDED
