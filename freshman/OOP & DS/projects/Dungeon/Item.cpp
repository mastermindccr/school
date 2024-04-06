#include "Item.h"

Item::Item():Object(){

};

Item::Item(string name,string tag,int health,int attack,int defense,int price):Object(name,tag){
    this->health = health;
    this->attack = attack;
    this->defense = defense;
    this->price = price;
}

void Item::triggerEvent(Player* a){
    cout<<this->get_name()<<" discovered!"<<endl;
    a->inventory.push_back(this);
    looted = true;
}

void Item::set_health(int health){
    this->health = health;
}

void Item::set_attack(int attack){
    this->attack = attack;
}

void Item::set_defense(int defense){
    this->defense = defense;
}

void Item::set_price(int a){
    this->price = a;
}

int Item::get_health(){
    return this->health;
}

int Item::get_attack(){
    return this->attack;
}

int Item::get_defense(){
    return this->defense;
}

int Item::get_price(){
    return this->price;
}
