#include "NPC.h"
NPC::NPC(){

}

NPC::NPC(string name, string script, vector<Item*> a):Character(name,"merchant",10,10,10,1){
    this->script = script;
    commodity = a;
}

void NPC::listCommodity(){
    for(size_t i = 0;i<commodity.size();i++)
    {
        cout<<i<<" "<<commodity[i]->get_name()<<" : "<<commodity[i]->get_price()+5<<" gold"<<endl;
    }
    cout<<commodity.size()<<" : back"<<endl;
}

void NPC::triggerEvent(Player* a){
    cout<<script<<" I'm "<<get_name()<<"!"<<endl;
    char cmd;
    int choose_item;
    int confirm;
    do{
        cout<<"do you want to buy or sell?"<<endl<<"0 : buy"<<endl<<"1 : sell"<<endl<<"2 : back"<<endl;
        cin>>cmd;
        system("cls");
        int temp;
        if(cmd=='0'){
            do{
                temp = (int)commodity.size();
                cout<<"what do you want to buy, little lad?"<<endl;
                listCommodity();
                cout<<endl<<"you have "<<a->gold<<" gold!"<<endl;
                do{
                    cin>>choose_item;
                    if(choose_item>temp) cout<<"it's invalid!"<<endl;
                }while(choose_item>temp);
                if(choose_item != temp){
                    cout<<"are you sure to buy "<<commodity[choose_item]->get_name()<<" ?"<<endl;
                    if(commodity[choose_item]->get_tag() != "supply"){
                        cout<<"health : "<<commodity[choose_item]->get_health()<<endl;
                        cout<<"attack : "<<commodity[choose_item]->get_attack()<<endl;
                        cout<<"defense : "<<commodity[choose_item]->get_defense()<<endl;
                    }
                    else{
                        cout<<"it recovers "<<commodity[choose_item]->get_health()<<" HP!"<<endl;
                    }
                    cout<<"0 : yes"<<endl<<"1 : no"<<endl;
                    cin>>confirm;
                    //buy
                    if(!confirm){
                        if(a->gold<(commodity[choose_item]->get_price()+5)){
                            system("cls");
                            cout<<"you are too poor to buy this!"<<endl<<endl;
                        }
                        else{
                            a->gold = a->gold - commodity[choose_item]->get_price() - 5;
                            a->inventory.push_back(commodity[choose_item]);
                            commodity.erase(commodity.begin()+choose_item);
                            system("cls");
                            cout<<"that's a good choice...hehe"<<endl<<endl;
                        }
                    }
                    else system("cls");
                }
                else system("cls");
            }while(choose_item != temp);
        }
        else if(cmd=='1'){
            do{
                temp = (int) a->inventory.size();
                cout<<"what do you want to sell, little lad?"<<endl;
                for(int i = 0;i<temp;i++){
                    cout<<i<<" "<<a->inventory[i]->get_name()<<" : "<<a->inventory[i]->get_price()<<" gold"<<endl;
                }
                cout<<temp<<" back"<<endl;
                do{
                    cin>>choose_item;
                    if(choose_item>temp) cout<<"it's invalid!"<<endl;
                }while(choose_item>temp);
                if(choose_item != temp){
                    cout<<"are you sure to sell "<<a->inventory[choose_item]->get_name()<<" ?"<<endl<<"0 : yes"<<endl<<"1 : no"<<endl;
                    cin>>confirm;
                    //sell
                    if(!confirm){
                        system("cls");
                        cout<<"very good! hehe..."<<endl<<endl;
                        a->gold+=a->inventory[choose_item]->get_price();
                        a->inventory.erase(a->inventory.begin()+choose_item);
                    }
                }
                else system("cls");
            }while(choose_item != temp);
        }
        else if(cmd=='2'){
            cout<<"thanks for your visit, hehehe..."<<endl;
            cin.ignore();
            cin.get();
            system("cls");
        }
        else
            cout<<"it's invalid!"<<endl;
    }while(cmd!='2');
}

/* Set & Get function*/
void NPC::setScript(string a){
    this->script = a;
}
void NPC::setCommodity(vector<Item*> a){
    this->commodity = a;
}
string NPC::getScript(){
    return this->script;
}
vector<Item*> NPC::getCommodity(){
    return this->commodity;
}
