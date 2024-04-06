#ifndef MONSTER_H_INCLUDED
#define MONSTER_H_INCLUDED
#include"Character.h"
#include"Player.h"
#include<string>
using namespace std;

class Monster:public Character{
public:
    Monster();
    Monster(string,int,int,int,int,int,int);
    void set_Monster(string,int);
    bool defeated;
    void triggerEvent(Player*);
    int exp;
    int gold;
};

#endif // MONSTER_H_INCLUDED
