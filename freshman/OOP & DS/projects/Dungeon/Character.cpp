#include"Character.h"

Character::Character():Object(){
}

Character::Character(string name,string tag,int max_health,int attack,int defense,int level){
    this->set_name(name);
    this->set_tag(tag);
    this->max_HP = max_health;
    this->current_HP = max_health;
    this->attack = attack;
    this->defense = defense;
    this->level = level;
}

void Character::set_max_HP(int a){
    this->max_HP = a;
}

void Character::set_current_HP(int a){
    this->current_HP = a;
}

void Character::set_attack(int a){
    this->attack = a;
}

void Character::set_defense(int a){
    this->defense = a;
}

void Character::set_level(int a){
    this->level = a;
}

int Character::get_max_HP(){
    return this->max_HP;
}

int Character::get_current_HP(){
    return this->current_HP;
}

int Character::get_attack(){
    return this->attack;
}

int Character::get_defense(){
    return this->defense;
}

int Character::get_level(){
    return this->level;
}
