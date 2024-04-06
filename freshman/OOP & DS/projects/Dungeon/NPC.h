#ifndef NPC_H_INCLUDED
#define NPC_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include "Player.h"
#include "Character.h"
#include "Item.h"

using namespace std;

class NPC: public Character
{
private:
    string script;
public:
    NPC();
    NPC(string, string, vector<Item*>);
    vector<Item*> commodity;
    void listCommodity(); /*print all the Item in this NPC*/

    void triggerEvent(Player*);

    /* Set & Get function*/
    void setScript(string);
    void setCommodity(vector<Item*>);
    string getScript();
    vector<Item*> getCommodity();
};


#endif // NPC_H_INCLUDED
