#include "Record.h"
#include <iomanip>
#include <windows.h>

Record::Record(){

}

void Record::saveToFile(int& up, int& down, int& left, int& right, Player* p, vector<Room*>& r, vector<vector<string>>& a, vector<vector<int>>& b){
    ofstream playerfile;
    ofstream roomfile;
    ofstream mapfile;
    saveRooms(r,roomfile);
    savePlayer(p,playerfile);
    saveMap(up,down,left,right,a,b,mapfile);
}

void Record::loadFromFile(int& up,int& down, int& left, int& right, int& width, int& length, Player* p, vector<Room*>& r, vector<vector<string>>& a, vector<vector<int>>& b){
    ifstream playerfile;
    ifstream roomfile;
    ifstream mapfile;
    loadRooms(r,roomfile);
    loadPlayer(p,r,playerfile);
    loadMap(up,down,left,right,width,length,a,b,mapfile);
}

void Record::savePlayer(Player* p, ofstream& o){
    o.open("playerfile.txt",ios::trunc);
    o<<p->get_name()<<endl;
    o<<p->get_level()<<" "; //level
    o<<p->get_current_HP()<<" "; //current HP
    o<<p->get_current_exp()<<" "; //current exp
    o<<p->gold<<" ";//gold
    //equipment
    o<<p->get_weapon()->get_name()<<" "; //weapon name
    o<<p->get_armor()->get_name()<<" "; //armor name
    //room
    o<<p->get_currentRoom()->index<<" "; //current room index
    o<<p->get_previousRoom()->index<<" "; //previous room index
    o<<p->inventory.size()<<" "; //size of inventory
    for(int i = 0;i<(int)p->inventory.size();i++){
        o<<p->inventory[i]->get_name()<<" "; // item's name
    }
    o.close();
}

void Record::saveRooms(vector<Room*>& r, ofstream& o){
    o.open("roomfile.txt",ios::trunc);
    for(int i = 0;i<(int)r.size();i++){
        o<<r[i]->index<<" "; //index
        o<<r[i]->x<<" "; //x coordinate
        o<<r[i]->y<<" "; //y coordinate
        // which room does this room connected to
        if(r[i]->upRoom == nullptr) o<<-1<<" "; //up room
        else o<<r[i]->upRoom->index<<" ";
        if(r[i]->downRoom == nullptr) o<<-1<<" "; //down room
        else o<<r[i]->downRoom->index<<" ";
        if(r[i]->leftRoom == nullptr) o<<-1<<" "; //left room
        else o<<r[i]->leftRoom->index<<" ";
        if(r[i]->rightRoom == nullptr) o<<-1<<" "; //right room
        else o<<r[i]->rightRoom->index<<" ";
        //what is in the room
        if(dynamic_cast<Item*>(r[i]->object)){ //item
            Item* temp = static_cast<Item*>(r[i]->object);
            o<<temp->get_tag()<<" ";
            o<<temp->get_name()<<" ";
            o<<temp->looted<<" ";
        }
        else if(dynamic_cast<Monster*>(r[i]->object)){ //monster
            Monster* temp = static_cast<Monster*>(r[i]->object);
            o<<temp->get_tag()<<" ";
            o<<temp->get_name()<<" ";
            o<<temp->defeated<<" ";
        }
        else if(dynamic_cast<NPC*>(r[i]->object)){ //NPC
            NPC* temp = static_cast<NPC*>(r[i]->object);
            o<<temp->get_tag()<<" ";
            o<<temp->commodity.size()<<" ";
            for(int i = 0;i<(int)temp->commodity.size();i++){
                o<<temp->commodity[i]->get_name()<<" ";
            }
        }
        else{ // nothing
            o<<"NULL ";
        }
        o<<endl;
    }
    o.close();
}

void Record::saveMap(int& u,int& d, int& l, int& r, vector<vector<string>>& a,vector<vector<int>>& b, ofstream& o){
    o.open("mapfile.txt",ios::trunc);
    o<<a.size()<<" ";
    o<<a[0].size()<<" ";
    o<<u<<" "<<d<<" "<<l<<" "<<r<<endl;
    for(int i = 0;i<(int)a.size();i++){
        for(int j = 0;j<(int)a[0].size();j++){
            o<<b[i][j]<<" ";
        }
        o<<endl;
    }
    for(int i = 0;i<(int)a.size();i++){
        for(int j = 0;j<(int)a[0].size();j++){
            o<<a[i][j];
            if(a[i][j]!="\0")
                o<<" ";
        }
        o<<endl;
    }
    o.close();
}

void Record::loadPlayer(Player* p,vector<Room*>& r, ifstream& i){
    i.open("playerfile.txt");
    int level,current_HP,current_exp,gold;
    int c_room,p_room;
    int size_of_inventory;
    string str_of_name,line;
    string name,tag;
    stringstream str;
    getline(i,str_of_name);
    p->set_name(str_of_name);
    getline(i,line);
    str<<line;
    str>>level>>current_HP>>current_exp>>gold;
    for(int i = 1;i<level;i++){
        p->level_up();
    } //level
    p->set_current_HP(current_HP); //current HP
    p->set_current_exp(current_exp); //current exp
    p->gold = gold;//gold
    //set weapon
    str>>name;
    p->init_weapon(which_item(name));
    //set armor
    str>>name;
    p->init_armor(which_item(name));
    //set current and previous rooms
    str>>c_room>>p_room;
    p->set_currentRoom(r[c_room]); //current room index
    p->set_previousRoom(r[p_room]); //previous room index
    //set inventory
    str>>size_of_inventory;

    for(int i = 0;i<size_of_inventory;i++){
        str>>name;
        p->inventory.push_back(which_item(name)); // item's name
    }
    i.close();
}

void Record::loadRooms(vector<Room*>& r, ifstream& i){
    i.open("roomfile.txt");
    stringstream str;
    string line;
    int index,x,y,up,down,left,right;
    string tag,name;
    bool cleared;
    int size_of_commodity;
    while(getline(i,line)){
        str.clear();
        str.str("");
        str<<line;

        str>>index>>x>>y>>up>>down>>left>>right>>tag;
        //set index and neighbors
        r[index]->index = index;
        r[index]->x = x;
        r[index]->y = y;
        if(up!=-1)
            r[index]->upRoom = r[up];
        if(down!=-1)
            r[index]->downRoom = r[down];
        if(left!=-1)
            r[index]->leftRoom = r[left];
        if(right!=-1)
            r[index]->rightRoom = r[right];
        //set objects
        if(tag == "NULL"){
            r[index]->object = nullptr;
        }
        else if(tag == "monster"){
            str>>name>>cleared;
            r[index]->object = which_monster(name);
            static_cast<Monster*>(r[index]->object)->defeated = cleared;
        }
        else if(tag == "supply"){
            str>>name>>cleared;
            r[index]->object = which_item(name);
            static_cast<Item*>(r[index]->object)->looted = cleared;
        }
        else{
            str>>size_of_commodity;
            vector<Item*> commodities;
            for(int i = 0;i<size_of_commodity;i++){
                str>>name;
                commodities.push_back(which_item(name));
            }
            r[index]->object = new NPC("merchant","Greetings!",commodities);
            for(int i = 0;i<(int)static_cast<NPC*>(r[index]->object)->commodity.size();i++){
            }
        }
    }
    i.close();
}

void Record::loadMap(int& u,int& d, int& l, int& r, int& width,int& length,vector<vector<string>>& a,vector<vector<int>>& b, ifstream& _i){
    _i.open("mapfile.txt");
    stringstream str;
    string line;
    getline(_i,line);
    str<<line;
    int index;
    string content;

    str>>width>>length>>u>>d>>l>>r;
    //resize maps
    a.resize(width,vector<string>(length));
    b.resize(width,vector<int>(length));
    //show map
    for(int i = 0;i<width;i++){
        getline(_i,line);
        str.clear();
        str.str("");
        str<<line;
        for(int j = 0;j<length;j++){
            str>>index;
            b[i][j] = index;
        }
    }
    // _map
    for(int i = 0;i<width;i++){
        getline(_i,line);
        str.clear();
        str.str("");
        str<<line;
        for(int j = 0;j<length;j++){
            if(b[i][j]!=2 && str>>content){
                a[i][j] = content;
            }
            else{
                a[i][j] = "\0";
            }
        }
    }
    _i.close();
}

Item* Record::which_item(string name){
    //weapon
    if(name == "dagger")
        return new Item("dagger","weapon",0,5,0,20); //dagger
    else if(name == "long_sword")
        return new Item("long_sword","weapon",0,10,0,35); //long_sword
    else if(name == "royal_sword")
        return new Item("royal_sword","weapon",0,15,0,50); //royal sword
    else if(name == "legendary_sword")
        return new Item("legendary_sword","weapon",5,30,5,150); //legendary sword
    else if(name == "cherished_knife")
        return new Item("cherished_knife","weapon",0,2,0,0); //cherished knife
    //armor
    else if(name == "oaken_shield")
        return new Item("oaken_shield","armor",5,0,2,20); //oaken shield
    else if(name == "heavy_armor")
        return new Item("heavy_armor","armor",15,0,5,40); //heavy armor
    else if(name == "royal_shield")
        return new Item("royal_shield","armor",20,0,15,50); //royal shield
    else if(name == "legendary_armor")
        return new Item("legendary_armor","armor",30,5,30,150); //legendary armor
    else if(name == "ragged_clothes")
        return new Item("ragged_clothes","armor",0,0,2,0); //ragged clothes
    //supply
    else if(name == "bandage")
        return new Item("bandage","supply",10,0,0,15); //bandage
    else if(name == "health_potion")
        return new Item("health_potion","supply",30,0,0,40); //health potion
    else
        return new Item("powerful_health_potion","supply",100,0,0,100); //powerful health potion
}

Monster* Record::which_monster(string name){
    if(name == "Doggod")
        return new Monster("Doggod",500,45,30,0,0,0); //Doggod
    else if(name == "dog")
        return new Monster("dog",5,3,3,9,3,1); //dog
    else if(name == "cat")
        return new Monster("cat",6,5,2,8,3,1); //cat
    else
        return new Monster("rat",8,2,3,10,5,1); //rat
}
