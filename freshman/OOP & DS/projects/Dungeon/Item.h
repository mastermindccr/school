#ifndef ITEM_H_INCLUDED
#define ITEM_H_INCLUDED
#include"Player.h"
#include"Object.h"
#include<string>
class Player;
class Item : public Object{
public:
    Item();
    Item(string,string,int,int,int,int);
    void triggerEvent(Player*);
    bool looted;
    /* getter & setter */
    void set_health(int);
    void set_attack(int);
    void set_defense(int);
    void set_price(int);

    int get_health();
    int get_attack();
    int get_defense();
    int get_price();
private:
    int health;
    int attack;
    int defense;
    int price;
};


#endif // ITEM_H_INCLUDED
