#ifndef PLAYER_H_INCLUDED
#define PLAYER_H_INCLUDED
#include"Character.h"
#include"Room.h"
#include"Item.h"
#include"exp.h"
#include<string>
#include<vector>
class Item;
class Player: public Character{
public:
    Player();
    Player(string,int,int,int,int,Item*,Item*);
    void addItem(Item*);
    bool useItem();
    void IncreaseStatus(int,int,int);
    void changeRoom(Room*);
    void triggerEvent();
    vector<Item*> inventory;
    /*(getter & setter)*/
    void set_currentRoom(Room*);
    void set_previousRoom(Room*);
    void init_weapon(Item*);
    void init_armor(Item*);
    void set_weapon(Item*);
    void set_armor(Item*);
    void set_max_exp(int);
    void set_current_exp(int);
    void level_up();
    int get_max_exp();
    int get_current_exp();
    Item* get_weapon();
    Item* get_armor();
    Room* get_currentRoom();
    Room* get_previousRoom();
    int gold;
private:
    Room* currentRoom;
    Room* previousRoom;
    Item* weapon;
    Item* armor;
    int max_exp;
    int current_exp;
};


#endif // PLAYER_H_INCLUDED
