#ifndef RECORD_H_INCLUDED
#define RECORD_H_INCLUDED

#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include "Item.h"
#include "Monster.h"
#include "NPC.h"
#include "Player.h"

using namespace std;

/* This is the record system. Get the information of the  */
/* player and rooms then save them to (a) file(s). Notice */
/* that using pass by reference can prevent sending the   */
/* whole vector to the function.                          */

class Record
{
private:
    void savePlayer(Player*, ofstream&);
    void saveRooms(vector<Room*>&, ofstream&);
    void saveMap(int&,int&, int&, int&, vector<vector<string>>&, vector<vector<int>>&, ofstream&);
    void loadPlayer(Player*,vector<Room*>&, ifstream&);
    void loadRooms(vector<Room*>&, ifstream&);
    void loadMap(int&,int&, int&, int&, int&, int&, vector<vector<string>>&, vector<vector<int>>&, ifstream&);

public:
    Record();
    void saveToFile(int&,int&, int&, int&, Player*, vector<Room*>&, vector<vector<string>>&, vector<vector<int>>&);
    void loadFromFile(int&,int&, int&, int&, int&, int&, Player*, vector<Room*>&, vector<vector<string>>&, vector<vector<int>>&);
    Item* which_item(string);
    Monster* which_monster(string);

};

#endif // RECORD_H_INCLUDED
