#include"Monster.h"
#include<windows.h>

Monster::Monster(){

}

Monster::Monster(string name,int health,int attack,int defense,int exp,int gold,int level):Character(name,"monster",health,attack,defense,level){
    this->gold = gold;
    this->exp = exp;
    this->set_level(level);
}

void Monster::set_Monster(string name,int level){
    if(name == "dog")//average
    {
        this->set_max_HP(5+6*(level-1));
        this->set_current_HP(this->get_max_HP());
        this->set_attack(3+4*(level-1));
        this->set_defense(3+2*(level-1));
        this->set_level(level);
        this->exp = 9+level-1;
        this->gold = 3+2*(level-1);
    }
    else if(name == "cat")//attacker
    {
        this->set_max_HP(6+4*(level-1));
        this->set_current_HP(this->get_max_HP());
        this->set_attack(5+4*(level-1));
        this->set_defense(2+level-1);
        this->set_level(level);
        this->exp = 8+level-1;
        this->gold = 3+2*(level-1);
    }
    else if(name == "rat")//tank
    {
        this->set_max_HP(8+8*(level-1));
        this->set_current_HP(this->get_max_HP());
        this->set_attack(2+3*(level-1));
        this->set_defense(3+3*(level-1));
        this->set_level(level);
        this->exp = 10+level-1;
        this->gold = 5+3*(level-1);
    }
    else{
        this->set_level(100);
    }
}

void Monster::triggerEvent/*combat*/(Player* a){
    system("cls");
    char cmd;
    cout<<this->get_name()<<" appears!"<<endl;
    set_Monster(this->get_name(),a->get_level());
    while(1)
    {
        cout<<this->get_tag()<<" name: "<<this->get_name()<<endl;
        cout<<"health: "<<this->get_current_HP()<<" / "<<this->get_max_HP()<<endl;
        cout<<"attack: "<<this->get_attack()<<endl;
        cout<<"defense: "<<this->get_defense()<<endl<<endl;
        cout<<a->get_name()<<" : "<<endl;
        cout<<"health: "<<a->get_current_HP()<<" / "<<a->get_max_HP()<<endl;
        cout<<"attack: "<<a->get_attack()<<endl;
        cout<<"defense: "<<a->get_defense()<<endl<<endl;
        //actions
            int action;
            do{
                action = 1;
                cout<<"what to do next: "<<endl;
                cout<<"0 : attack"<<endl<<"1 : use item"<<endl<<"2 : retreat"<<endl;
                cin>>cmd;
                if(cmd=='0'){
                    system("cls");
                    int dmg = a->get_attack() - this->get_defense();
                    if(dmg <= 0) dmg = 1;
                    cout<<a->get_name()<<" deals "<<dmg<<" damage to "<<this->get_name()<<"!"<<endl;
                    this->set_current_HP(this->get_current_HP() - dmg);
                }
                else if(cmd=='1'){
                    if(a->useItem());
                    else action = 0;
                }
                else if(cmd=='2'){
                    if(this->get_name() == "Doggod"){
                        cout<<"there's no way to retreat..."<<endl;
                        cin.ignore();
                        cin.get();
                        system("cls");
                        break;
                    }
                    cout<<"Maybe I should retreat for now..."<<endl;
                    cin.ignore();
                    cin.get();
                    system("cls");
                    a->set_currentRoom(a->get_previousRoom());return;
                }
                else cout<<"it's invalid!"<<endl;
            }while(!isdigit(cmd) || (int)cmd<48 || (int)cmd>50 || !action);
        //win battle
        if(this->get_current_HP()<=0){
            //play victory sound
            PlaySound("Victory.wav",NULL,SND_LOOP | SND_ASYNC);
            cout<<this->get_name()<<" defeated! "<<this->exp<<" exp gained! "<<this->gold<<" gold gained!"<<endl;
            this->defeated = true;
            a->gold += this->gold;
            a->set_current_exp(a->get_current_exp()+this->exp);
            while(a->get_current_exp()>=a->get_max_exp()){
                a->set_current_exp(a->get_current_exp()-a->get_max_exp());
                a->level_up();
            }
            cin.ignore();
            cin.get();
            system("cls");
            return;
        }
        // monster deal damage to player
        int dmg = this->get_attack() - a->get_defense();
        if(dmg <= 0) dmg = 1;
        cout<<this->get_name()<<" deal "<<dmg<<" damage to "<<a->get_name()<<"!"<<endl<<endl;
        a->set_current_HP(a->get_current_HP() - dmg);
        // player dead
        if(a->get_current_HP()<=0){
            PlaySound("Dead.wav",NULL,SND_LOOP | SND_ASYNC);
            string dot = "......";
            string long_dot = ".............................................";
            for(size_t i = 0;i<dot.length();i++){
                cout<<dot[i];
                Sleep(500);
            }
            Sleep(500);
            for(size_t i = 0;i<dot.length();i++){
                cout<<dot[i];
                Sleep(500);
            }
            Sleep(500);
            for(size_t i = 0;i<long_dot.length();i++){
                cout<<long_dot[i];
                Sleep(100);
            }
            cout<<endl<<"Game Over"<<endl;
            return;
        }
    }
}
