#include "Dungeon.h"
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <iomanip>
#include <windows.h>
#include <mmsystem.h>
#define const_room 30
Dungeon::Dungeon(){

}

void Dungeon::createPlayer(){
    cout<<"Please enter your name:";
    string name;
    cin>>name;
    Item* w = new Item("cherished_knife","weapon",0,2,0,0);
    Item* a = new Item("ragged_clothes","armor",0,0,2,0);
    Item* b = new Item("bandage","supply",10,0,0,15);
    player = new Player(name,20,5,3,0,w,a);
    player->set_currentRoom(rooms[0]);
    player->set_previousRoom(rooms[0]);
    player->addItem(b);
    PlaySound("Ruins.wav",NULL,SND_LOOP | SND_ASYNC);
}

void Dungeon::createMap(){
    rooms.resize(const_room);
    srand(time(NULL));
    // Weapons
    Item* dagger = new Item("dagger","weapon",0,5,0,20);
    Item* long_sword = new Item("long_sword","weapon",0,10,0,35);
    Item* royal_sword = new Item("royal_sword","weapon",0,15,0,50);
    Item* l_sword = new Item("legendary_sword","weapon",5,30,5,150);
    // Armors
    Item* oakenshield = new Item("oaken_shield","armor",5,0,2,20);
    Item* heavy_armor = new Item("heavy_armor","armor",15,0,5,40);
    Item* royal_shield = new Item("royal_shield","armor",20,0,15,50);
    Item* l_armor = new Item("legendary_armor","armor",30,5,30,150);
    // Commodities
    vector<Item*> commodities = {dagger,long_sword,l_sword,oakenshield,heavy_armor,l_armor};
    for(int i = 0;i<3;i++) commodities.push_back(new Item("bandage","supply",10,0,0,15));
    for(int i = 0;i<2;i++) commodities.push_back(new Item("health_potion","supply",30,0,0,40));
    for(int i = 0;i<1;i++) commodities.push_back(new Item("powerful_health_potion","supply",100,0,0,100));
    NPC* n = new NPC("merchant","Greetings!",commodities);

    int num_of_items = 0;
    int num_of_NPC = 0;

    for(int i = 0;i<const_room;i++){
        rooms[i] = new Room;
        rooms[i]->index = i;
        rooms[i]->init();
        if(i == 0) continue;
        if(i == const_room - 1){
            rooms[i]->isExit = 1;
            rooms[i]->object = new Monster("Doggod",500,45,30,0,0,0);
            break;
        }
        int con;
        //put object in rooms
        do{
            int ra = rand();
            con = 1;
            if(ra%6==0 || ra%6==1 || ra%6==2)/*monster*/{
                //choose monster
                if(ra%3==0)/*dog*/{
                    rooms[i]->object = new Monster("dog",5,3,3,9,3,1);
                }
                else if(ra%3==1)/*cat*/{
                    rooms[i]->object = new Monster("cat",6,5,2,8,3,1);
                }
                else if(ra%3==2)/*rat*/{
                    rooms[i]->object = new Monster("rat",8,2,3,10,5,1);
                }
            }
             else if(ra%6==3)/*item*/{
                 //choose items
                 if(num_of_items < 6){
                    if(num_of_items < 2){
                        rooms[i]->object = new Item("bandage","supply",10,0,0,15);
                    }
                    else if(num_of_items < 4){
                        rooms[i]->object = new Item("health_potion","supply",30,0,0,40);
                    }
                    else if(num_of_items < 5){
                        rooms[i]->object = royal_sword;
                    }
                    else{
                        rooms[i]->object = royal_shield;
                    }
                    num_of_items++;
                 }
                 else con = 0;
             }
            else if(ra%6==4)/*NPC*/{
                if(num_of_NPC < 1){
                    rooms[i]->object = n;
                    num_of_NPC++;
                }
                else con = 0;
            }
            else/*empty*/{
                rooms[i]->object = nullptr;
            }
        }while(!con);
    }
    //random map create
    int room_num = 1;
    while(room_num<const_room){
        bool repeat = 0;
        int index = rand()%(room_num);
        int direction = rand()%4;
        switch(direction){
            case 0: {
                if(rooms[index]->upRoom == nullptr){
                    for(int i = 0;i<room_num;i++){
                        if(rooms[index]->x == rooms[i]->x && rooms[index]->y+1 == rooms[i]->y){
                            repeat = 1;
                            break;
                        }
                    }
                    if(!repeat){
                        rooms[index]->upRoom = rooms[room_num];
                        rooms[room_num]->downRoom = rooms[index];
                        rooms[room_num]->x = rooms[index]->x;
                        rooms[room_num]->y = rooms[index]->y+1;
                        room_num++;
                    }
                }
                break;
            }
            case 1: {
                if(rooms[index]->downRoom == nullptr){
                    for(int i = 0;i<room_num;i++){
                    if(rooms[index]->x == rooms[i]->x && rooms[index]->y-1 == rooms[i]->y){
                        repeat = 1;
                        break;
                    }
                }
                    if(!repeat){
                        rooms[index]->downRoom = rooms[room_num];
                        rooms[room_num]->upRoom = rooms[index];
                        rooms[room_num]->x = rooms[index]->x;
                        rooms[room_num]->y = rooms[index]->y-1;
                        room_num++;
                    }
                }
                break;
            }
            case 2: {
                if(rooms[index]->leftRoom == nullptr){
                    for(int i = 0;i<room_num;i++){
                    if(rooms[index]->x-1 == rooms[i]->x && rooms[index]->y == rooms[i]->y){
                        repeat = 1;
                        break;
                        }
                    }
                    if(!repeat){
                        rooms[index]->leftRoom = rooms[room_num];
                        rooms[room_num]->rightRoom = rooms[index];
                        rooms[room_num]->x = rooms[index]->x-1;
                        rooms[room_num]->y = rooms[index]->y;
                        room_num++;
                    }
                }
                break;
            }
            case 3: {
                if(rooms[index]->rightRoom == nullptr){
                    for(int i = 0;i<room_num;i++){
                    if(rooms[index]->x+1 == rooms[i]->x && rooms[index]->y == rooms[i]->y){
                        repeat = 1;
                        break;
                        }
                    }
                    if(!repeat){
                        rooms[index]->rightRoom = rooms[room_num];
                        rooms[room_num]->leftRoom = rooms[index];
                        rooms[room_num]->x = rooms[index]->x+1;
                        rooms[room_num]->y = rooms[index]->y;
                        room_num++;
                    }
                }
                break;
            }
        }
    }
    //save the map into vector
    l = 0,r = 0,u = 0,d = 0;
    for(int i = 0;i<const_room;i++){
        if(rooms[i]->x <= l) l = rooms[i]->x;
        if(rooms[i]->x >= r) r = rooms[i]->x;
        if(rooms[i]->y >= u) u = rooms[i]->y;
        if(rooms[i]->y <= d) d = rooms[i]->y;
    }
    length = 2*r-2*l+1;
    width = 2*u-2*d+1;
    _map.resize(width,vector<string>(length));
    show_map.resize(width,vector<int>(length));
    for(int i = 0;i<width;i++){
        for(int j = 0;j<length;j++){
            _map[i][j] = "\0";
            show_map[i][j] = 2;
        }
    }
    for(int i = 0;i<const_room;i++){
        _map[2*(u - rooms[i]->y)][2*(rooms[i]->x - l)] = to_string(rooms[i]->index);
        show_map[2*(u - rooms[i]->y)][2*(rooms[i]->x - l)] = 0;

        if(rooms[i]->upRoom != nullptr){
            _map[2*(u - rooms[i]->y)-1][2*(rooms[i]->x - l)] = "|";
            show_map[2*(u - rooms[i]->y)-1][2*(rooms[i]->x - l)] = 0;
        }
        if(rooms[i]->downRoom != nullptr){
            _map[2*(u - rooms[i]->y)+1][2*(rooms[i]->x - l)] = "|";
            show_map[2*(u - rooms[i]->y)+1][2*(rooms[i]->x - l)] = 0;
        }
        if(rooms[i]->leftRoom != nullptr){
            _map[2*(u - rooms[i]->y)][2*(rooms[i]->x - l)-1] = "¡Ð";
            show_map[2*(u - rooms[i]->y)][2*(rooms[i]->x - l)-1] = 0;
        }
        if(rooms[i]->rightRoom != nullptr){
            _map[2*(u - rooms[i]->y)][2*(rooms[i]->x - l)+1] = "¡Ð";
            show_map[2*(u - rooms[i]->y)][2*(rooms[i]->x - l)+1] = 0;
        }
    }

    show_map[2*(u - rooms[0]->y)][2*(rooms[0]->x - l)] = 1;//index itself
    //other direction
    if(rooms[0]->upRoom != nullptr)
        show_map[2*(u - rooms[0]->y)-1][2*(rooms[0]->x - l)] = 1;
    if(rooms[0]->downRoom != nullptr)
        show_map[2*(u - rooms[0]->y)+1][2*(rooms[0]->x - l)] = 1;
    if(rooms[0]->leftRoom != nullptr)
        show_map[2*(u - rooms[0]->y)][2*(rooms[0]->x - l)-1] = 1;
    if(rooms[0]->rightRoom != nullptr)
        show_map[2*(u - rooms[0]->y)][2*(rooms[0]->x - l)+1] = 1;
}

void Dungeon::printmap(){
    for(int i = 0;i<width;i++){
        for(int j = 0;j<length;j++){
            if(show_map[i][j] != 1)
                cout<<setw(2)<<"";
            else
                cout<<setw(2)<<_map[i][j];
        }
        cout<<endl;
    }
}

void Dungeon::handleMovement(){
    player->set_previousRoom(player->get_currentRoom());
    char cmd;
    int counts = 0;
    int up = -1,down = -1,left = -1,right = -1;
    if(player->get_currentRoom()->upRoom != nullptr) up = counts++;
    if(player->get_currentRoom()->downRoom != nullptr) down = counts++;
    if(player->get_currentRoom()->leftRoom != nullptr) left = counts++;
    if(player->get_currentRoom()->rightRoom != nullptr) right = counts++;
    cout<<"move to where:"<<endl;
    if(up!=-1) cout<<up<<" : up"<<endl;
    if(down!=-1) cout<<down<<" : down"<<endl;
    if(left!=-1) cout<<left<<" : left"<<endl;
    if(right!=-1) cout<<right<<" : right"<<endl;
    do{
        cin>>cmd;
        while(!isdigit(cmd)) cin>>cmd;
        if((int)cmd-48==up) {
            if(player->get_currentRoom()->upRoom->isExit){
                int enter;
                PlaySound(NULL,0,0);
                cout<<"Warning! Danger's ahead! Are you sure to proceed?"<<endl<<"0 : yes"<<endl<<"1 : no"<<endl;
                cin>>enter;
                if(enter){
                    PlaySound("Ruins.wav",NULL,SND_LOOP | SND_ASYNC);
                    return;
                }
            }
            player->set_currentRoom(player->get_currentRoom()->upRoom);
        }
        else if((int)cmd-48==down) {
            if(player->get_currentRoom()->downRoom->isExit){
                int enter;
                PlaySound(NULL,0,0);
                cout<<"Warning! Danger's ahead! Are you sure to proceed?"<<endl<<"0 : yes"<<endl<<"1 : no"<<endl;
                cin>>enter;
                if(enter){
                    PlaySound("Ruins.wav",NULL,SND_LOOP | SND_ASYNC);
                    return;
                }
            }
            player->set_currentRoom(player->get_currentRoom()->downRoom);
        }
        else if((int)cmd-48==left) {
            if(player->get_currentRoom()->leftRoom->isExit){
                int enter;
                PlaySound(NULL,0,0);
                cout<<"Warning! Danger's ahead! Are you sure to proceed?"<<endl<<"0 : yes"<<endl<<"1 : no"<<endl;
                cin>>enter;
                if(enter){
                    PlaySound("Ruins.wav",NULL,SND_LOOP | SND_ASYNC);
                    return;
                }
            }
            player->set_currentRoom(player->get_currentRoom()->leftRoom);
        }
        else if((int)cmd-48==right) {
            if(player->get_currentRoom()->rightRoom->isExit){
                int enter;
                PlaySound(NULL,0,0);
                cout<<"Warning! Danger's ahead! Are you sure to proceed?"<<endl<<"0 : yes"<<endl<<"1 : no"<<endl;
                cin>>enter;
                if(enter){
                    PlaySound("Ruins.wav",NULL,SND_LOOP | SND_ASYNC);
                    return;
                }
            }
            player->set_currentRoom(player->get_currentRoom()->rightRoom);
        }
        else cout<<"it's invalid!"<<endl;
    }while((int)cmd-48>=counts || (int)cmd-48<0);
    show_map[2*(u - player->get_currentRoom()->y)][2*(player->get_currentRoom()->x - l)] = 1;
    if(player->get_currentRoom()->upRoom != nullptr)
        show_map[2*(u - player->get_currentRoom()->y)-1][2*(player->get_currentRoom()->x - l)] = 1;
    if(player->get_currentRoom()->downRoom != nullptr)
        show_map[2*(u - player->get_currentRoom()->y)+1][2*(player->get_currentRoom()->x - l)] = 1;
    if(player->get_currentRoom()->leftRoom != nullptr)
        show_map[2*(u - player->get_currentRoom()->y)][2*(player->get_currentRoom()->x - l)-1] = 1;
    if(player->get_currentRoom()->rightRoom != nullptr)
        show_map[2*(u - player->get_currentRoom()->y)][2*(player->get_currentRoom()->x - l)+1] = 1;
    handleEvent(player->get_currentRoom()->object);
}

void Dungeon::handleEvent(Object* a){
    system("cls");
    char cmd;
    if(dynamic_cast<Monster*>(a)){
        if(dynamic_cast<Monster*>(a)->defeated){
        cout<<"there is nothing left here..."<<endl;
        }
        else{
            if(dynamic_cast<Monster*>(a)->get_name() != "Doggod")
                PlaySound("Normal Fight.wav",NULL,SND_LOOP | SND_ASYNC);
            else
                PlaySound("Boss Fight.wav",NULL,SND_LOOP | SND_ASYNC);
            static_cast<Monster*>(a)->triggerEvent(player);
            if(player->get_current_HP()>0)
                PlaySound("Ruins.wav",NULL,SND_LOOP | SND_ASYNC);
        }
    }
    else if(dynamic_cast<Item*>(a)){
        if(dynamic_cast<Item*>(a)->looted){
            cout<<"there is nothing left here..."<<endl;
        }
        else{
            cout<<"there's something glittering... pick it up?"<<endl<<"0 : yes"<<endl<<"1 : no"<<endl;
            do{
                cin>>cmd;
                if(cmd=='0'){
                    system("cls");
                    static_cast<Item*>(a)->triggerEvent(player);
                }
                else if(cmd=='1');
                else cout<<"it's invalid!"<<endl;
            }while(cmd!='0' && cmd!='1');
        }
    }
    else if(dynamic_cast<NPC*>(a)){
        cout<<"there's a shop... go inside?"<<endl<<"0 : yes"<<endl<<"1 : no"<<endl;
        cin>>cmd;
        if(cmd=='0'){
            system("cls");
            PlaySound("Merchant.wav",NULL,SND_LOOP | SND_ASYNC);
            static_cast<NPC*>(a)->triggerEvent(player);
            if(player->get_current_HP()>0)
                PlaySound("Ruins.wav",NULL,SND_LOOP | SND_ASYNC);
        }
    }
    else
        cout<<"there's nothing here..."<<endl;
}

void Dungeon::startGame(){
    fstream file("playerfile.txt");
    record = Record();
    char cmd,confirm = '0';
    if(file.good()){
        do{
        cout<<"0 continue"<<endl<<"1 new game"<<endl;
        cin>>cmd;
            if(cmd == '0'){
                rooms.resize(const_room);
                for(int i = 0;i<const_room;i++){
                    rooms[i] = new Room;
                    rooms[i]->init();
                }
                rooms[const_room-1]->isExit = 1;
                player = new Player("player",20,5,3,0,new Item("cherished_knife","weapon",0,2,0,0),new Item("ragged_clothes","armor",0,0,2,0));
                record.loadFromFile(u,d,l,r,width,length,player,rooms,_map,show_map);
                PlaySound("Ruins.wav",NULL,SND_LOOP | SND_ASYNC);
            }
            else{
                cout<<"are you sure to start a new game?"<<endl<<"0 yes"<<endl<<"1 no"<<endl;
                cin>>confirm;
                if(confirm=='0'){
                    system("cls");
                    createMap();
                    createPlayer();
                }
            }
        }while(confirm != '0');
    }
    else{
        createMap();
        createPlayer();
    }

    //story here

    system("cls");
    chooseAction();
}

void Dungeon::chooseAction(){
    char cmd;
    cout<<"This is room "<<player->get_currentRoom()->index<<endl;
    cout<<"what to do next: "<<endl;
    cout<<"0 : check info and status"<<endl;
    cout<<"1 : use/check item"<<endl;
    cout<<"2 : move"<<endl;
    cout<<"3 : look map"<<endl;
    cout<<"4 : save file"<<endl;
    cout<<"5 : exit game"<<endl;
    do{
        cin>>cmd;
        if(cmd=='0'){
            system("cls");
            player->triggerEvent();
        }
        else if(cmd=='1'){
            if(!player->useItem());
        }
        else if(cmd=='2'){
            handleMovement();
        }
        else if(cmd=='3'){
            system("cls");
            printmap();
        }
        else if(cmd=='4'){
            record.saveToFile(u,d,l,r,player,rooms,_map,show_map);
            system("cls");
            cout<<"file saved!"<<endl;
        }
        else if(cmd=='5'){
            return;
        }
        else{
            cout<<"it's invalid!"<<endl;
        }
    }while(!isdigit(cmd) || (int)cmd<48 || (int)cmd>53 );
    if(!checkGameLogic()) chooseAction();
}

bool Dungeon::checkGameLogic(){
    if(player->get_current_HP()<=0){
        char cmd;
        cout<<endl<<"0 : load saved file"<<endl<<"1 : exit game"<<endl;
        do{
            cin>>cmd;
            if(cmd=='0'){
                player = new Player("player",20,5,3,0,new Item("cherished_knife","weapon",0,2,0,0),new Item("ragged_clothes","armor",0,0,2,0));
                record.loadFromFile(u,d,l,r,width,length,player,rooms,_map,show_map);
                PlaySound("Ruins.wav",NULL,SND_LOOP | SND_ASYNC);
                system("cls");
            }
            else if(cmd=='1') return true;
            else cout<<"it's invalid!"<<endl;
        }while(cmd!='0' && cmd!='1');
    }
    else if(player->get_currentRoom()->isExit){
        //victory sound;
        PlaySound("Congrats.wav",NULL,SND_LOOP | SND_ASYNC);
        cout<<"Congrats "<<player->get_name()<<"! you've completed the game!"<<endl;
        cin.ignore();
        cin.get();
        return true;
    }
    return false;
}

/* Deal with the whole game process */
void Dungeon::runDungeon(){
    PlaySound(TEXT("Bravely Default.wav"),NULL,SND_LOOP | SND_ASYNC);
    startGame();
}
