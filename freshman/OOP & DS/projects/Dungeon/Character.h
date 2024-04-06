#ifndef CHARACTER_H_INCLUDED
#define CHARACTER_H_INCLUDED
#include"Object.h"
#include<iostream>
#include<string>
#include<vector>

using namespace std;

class Character : public Object{
public:
    Character();
    Character(string,string,int,int,int,int);
    /* getter & setter*/
    void set_max_HP(int);
    void set_current_HP(int);
    void set_attack(int);
    void set_defense(int);
    void set_level(int);

    int get_max_HP();
    int get_current_HP();
    int get_attack();
    int get_defense();
    int get_level();
private:
    int max_HP;
    int current_HP;
    int attack;
    int defense;
    int level;
};

#endif // CHARACTER_H_INCLUDED
