#include"Room.h"

Room::Room(){

}

void Room::init(){
    this->upRoom = nullptr;
    this->downRoom = nullptr;
    this->leftRoom = nullptr;
    this->rightRoom = nullptr;
    if(this->index == 0){
        this->x = 0;
        this->y = 0;
        this->object = nullptr;
    }
    this->isExit = 0;
}
