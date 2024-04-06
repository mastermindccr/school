#ifndef ROOM_H_INCLUDED
#define ROOM_H_INCLUDED
#include<vector>
#include"object.h"

class Room{
public:
    Room();
    Room(Object*);
    void init();
    bool isExit;
    int index;
    int x;
    int y;
    Object* object;

    Room* upRoom;
    Room* downRoom;
    Room* leftRoom;
    Room* rightRoom;
};


#endif // ROOM_H_INCLUDED
