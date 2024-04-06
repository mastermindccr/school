#include "Player.h"

vector<Item*> inventory;

Player::Player(){

}

Player::Player(string name,int max_health,int attack,int defense,int gold,Item *a,Item *b):Character(name,"Player",max_health,attack,defense,1){
    this->set_max_exp(15);
    this->set_current_exp(0);
    this->weapon = a;
    this->armor = b;
    this->gold = gold;
    set_max_HP(get_max_HP() + weapon->get_health() + armor->get_health());
    set_current_HP(get_max_HP());
    set_attack(get_attack() + weapon->get_attack() + armor->get_attack());
    set_defense(get_defense() + weapon->get_defense() + armor->get_defense());
}

void Player::addItem(Item *a){
    inventory.push_back(a);
}

bool Player::useItem(){
    int choose_item,confirm = 0;
        do{
            cout<<"use which item?"<<endl;
            for(size_t i = 0;i<inventory.size();i++)
            {
                cout<<i<<" : "<<inventory[i]->get_name()<<endl;
            }
            cout<<inventory.size()<<" : back"<<endl;
            //fool-proof
            do{
                cin>>choose_item;
                if(choose_item > (int)inventory.size() || choose_item < 0)
                    cout<<"it's invalid!"<<endl;
            }while(choose_item > (int)inventory.size() || choose_item < 0);
            //back
            if(choose_item == (int)inventory.size()){
                return false;
            }
            //item:Supply
            else if(inventory[choose_item]->get_tag() == "supply")
            {
                cout<<"it recovers "<<inventory[choose_item]->get_health()<<" HP,use it?"<<endl;
                cout<<"0 : yes"<<endl<<"1 : no"<<endl;
                do
                {   cin>>confirm;
                    if(confirm!=0 && confirm!=1)
                        cout<<"it's invalid!"<<endl;
                }while(confirm!=0 && confirm!=1);
            }
        }while(confirm);
        system("cls");
        //use Supply
        if(inventory[choose_item]->get_tag() == "supply"){
            system("cls");
            cout<<inventory[choose_item]->get_health()<<" HP recovered!"<<endl;
            if(get_current_HP()+inventory[choose_item]->get_health() > get_max_HP()){
                cout<<"HP maxed out!"<<endl;
                set_current_HP(get_max_HP());
            }
            else set_current_HP(get_current_HP() + inventory[choose_item]->get_health());
            inventory.erase(inventory.begin()+choose_item);
        }
        //use equipment
        else {
            cout<<"tag : "<<inventory[choose_item]->get_tag()<<endl;
            cout<<"name : "<<inventory[choose_item]->get_name()<<endl;
            cout<<"health : "<<inventory[choose_item]->get_health()<<endl;
            cout<<"attack : "<<inventory[choose_item]->get_attack()<<endl;
            cout<<"defense : "<<inventory[choose_item]->get_defense()<<endl;
            cout<<"do you want to equip it?"<<endl<<"0 : yes"<<endl<<"1 : no"<<endl;
            int equip;
            do{
                cin>>equip;
                if(equip == 0){
                    if(inventory[choose_item]->get_tag() == "weapon"){
                        set_weapon(inventory[choose_item]);
                    }
                    else{
                        set_armor(inventory[choose_item]);
                    }

                }
                else if(equip == 1){
                    return false;
                }
                else{
                    cout<<"it's invalid!"<<endl;
                }
            }while(equip!=0 && equip!=1);
        }
        cout<<endl;
        return true;
}

void Player::IncreaseStatus(int health,int attack,int defense){
    this->set_max_HP(get_max_HP()+health);
    this->set_attack(get_attack()+attack);
    this->set_defense(get_defense()+defense);
}

void Player::changeRoom(Room* a){
    this->currentRoom = a;
}

void Player::triggerEvent/*check_info*/(){
    cout<<"info: "<<endl;
    cout<<"name : "<<this->get_name()<<endl;
    cout<<"HP : "<< this->get_current_HP()<<" / "<< this->get_max_HP()<<endl;
    cout<<"attack : "<<this->get_attack()<<endl;
    cout<<"defense : "<<this->get_defense()<<endl;
    cout<<"weapon : "<<this->get_weapon()->get_name()<<endl;
    cout<<"armor : "<<this->get_armor()->get_name()<<endl;
    cout<<"gold : "<<this->gold<<endl;
    cout<<"level "<<this->get_level()<<" exp : "<<this->get_current_exp()<<" / "<<this->get_max_exp()<<endl;
    cin.ignore();
    cin.get();
}
/*(getter & setter)*/

void Player::init_weapon(Item *a){
    this->set_max_HP(this->get_max_HP() + a->get_health());
    this->set_attack(this->get_attack() + a->get_attack());
    this->set_defense(this->get_defense() + a->get_defense());
    this->weapon = a;
}

void Player::init_armor(Item* a){
    this->set_max_HP(this->get_max_HP()+a->get_health());
    this->set_attack(this->get_attack()+a->get_attack());
    this->set_defense(this->get_defense()+a->get_defense());
    this->armor = a;
}

void Player::set_weapon(Item* a){
    addItem(this->weapon);
    for(size_t i = 0;i<inventory.size();i++){
        if(inventory[i]->get_name() == a->get_name()){
            inventory.erase(inventory.begin() + i);
            break;
        }
    }
    this->set_max_HP(this->get_max_HP() + a->get_health() - this->weapon->get_health());
    this->set_attack(this->get_attack() + a->get_attack() - this->weapon->get_attack());
    this->set_defense(this->get_defense() + a->get_defense() - this->weapon->get_defense());
    this->weapon = a;
}

void Player::set_armor(Item* a){
    addItem(this->armor);
    for(size_t i = 0;i<inventory.size();i++){
        if(inventory[i]->get_name() == a->get_name()){
            inventory.erase(inventory.begin() + i);
            break;
        }
    }
    this->set_max_HP(this->get_max_HP() + a->get_health() - this->armor->get_health());
    this->set_attack(this->get_attack() + a->get_attack() - this->armor->get_attack());
    this->set_defense(this->get_defense() + a->get_defense() - this->armor->get_defense());
    this->armor = a;
}

void Player::level_up(){
    this->set_level(this->get_level()+1);
    this->max_exp = exp(this->get_level()+1);
    this->set_max_HP(this->get_max_HP()+5);
    this->set_current_HP(this->get_max_HP());
    this->set_attack(this->get_attack()+3);
    this->set_defense(this->get_defense()+2);
    cout<<"level up! your HP is recovered to max!"<<endl;
}

void Player::set_max_exp(int a){
    this->max_exp = a;
}

void Player::set_current_exp(int a){
    this->current_exp = a;
}

int Player::get_max_exp(){
    return this->max_exp;
}

int Player::get_current_exp(){
    return this->current_exp;
}

Item* Player::get_weapon(){
    return this->weapon;
}

Item* Player::get_armor(){
    return this->armor;
}

void Player::set_currentRoom(Room* a){
    this->currentRoom = a;
}

void Player::set_previousRoom(Room* a){
    this->previousRoom = a;
}

Room* Player::get_currentRoom(){
    return this->currentRoom;
}

Room* Player::get_previousRoom(){
    return this->previousRoom;
}
