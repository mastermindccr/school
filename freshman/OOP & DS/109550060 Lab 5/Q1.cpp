#include <vector>
#include <iostream>
using namespace std;

class Object{
public:
    virtual void response() = 0;

    virtual ~Object() = default;
};

class IceMonster : public Object{
    //add constructor or functions if you need
public:
    void response()
    {
        cout<<"fire attack"<<endl;
    }
};

class FireMonster : public Object{
    //add constructor or functions if you need
public:
    void response()
    {
        cout<<"ice attack"<<endl;
    }
};

class Treasure : public Object{
    //add constructor or functions if you need
public:
    void response()
    {
        cout<<"ya"<<endl;
    }
};


class Room{
public:
    Room *up_room;
    Room *down_room;
    Room *left_room;
    Room *right_room;
    int index;
    Object *o;
    void init()
    {
        this->up_room = nullptr;
        this->down_room = nullptr;
        this->left_room = nullptr;
        this->right_room = nullptr;
    }
    void set_o(char p,IceMonster *i,FireMonster *f,Treasure *t)
    {
    	if(p == 'i') this->o = i;
    	else if(p == 'f') this->o = f;
    	else if(p == 't') this->o = t;
    	else this->o = nullptr;
	}
	void set_neighbor(vector<Room*> a,int u,int d,int l,int r)
	{
		if(u != -1)
        {
            this->up_room = a[u];
        }
        if(d != -1)
        {
            this->down_room = a[d];
        }
        if(l != -1)
        {
            this->left_room = a[l];
        }
        if(r != -1)
        {
            this->right_room = a[r];
        }
	}
	
	Room* move(char order,vector<Room*> a)
	{
		Room* b;
		if(order == 'u')
        {
            if(this->up_room!=nullptr)
            {
                b = this->up_room;
                cout<<b->index<<" ";
                if(a[b->index]->o!=nullptr)
                    a[b->index]->o->response();
                else cout<<endl;
                a[b->index]->o = nullptr;
            }
            else
            {
            	cout<<-1<<endl;
				return this;
			}
        }
        else if(order == 'd')
        {
            if(this->down_room!=nullptr)
            {
                b = this->down_room;
                cout<<b->index<<" ";
                if(a[b->index]->o!=nullptr)
                    a[b->index]->o->response();
                else cout<<endl;
                a[b->index]->o = nullptr;
            }
            else
            {
            	cout<<-1<<endl;
				return this;
			}
        }
        else if(order == 'l')
        {
            if(this->left_room!=nullptr)
            {
                b = this->left_room;
                cout<<b->index<<" ";
                if(a[b->index]->o!=nullptr)
                    a[b->index]->o->response();
                else cout<<endl;
                a[b->index]->o = nullptr;
            }
            else
            {
            	cout<<-1<<endl;
				return this;
			}
        }
        else if(order == 'r')
        {
            if(this->right_room!=nullptr)
            {
                b = this->right_room;
                cout<<b->index<<" ";
                if(a[b->index]->o!=nullptr)
                    a[b->index]->o->response();
                else cout<<endl;
                a[b->index]->o = nullptr;
            }
            else
            {
            	cout<<-1<<endl;
				return this;
			}
        }
        return b;
	}
    //add constructor or functions if you need
};

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    int m,n;
    char order;
    vector<Room*> rooms;
    IceMonster ice;
    FireMonster fire;
    Treasure trash;
    cin>>m>>n;
    rooms.resize(m);
    Room *TP;
    TP = new Room;
    for(int i = 0;i<m;i++)
    {
        rooms[i] = new Room;
        rooms[i]->init();
    }
    for(int i = 0;i<m;i++)
    {
        char property;
        int u,d,l,r;
        rooms[i]->index = i;
        cin>>property>>u>>d>>l>>r;
        
        rooms[i]->set_o(property,&ice,&fire,&trash);
        
        rooms[i]->set_neighbor(rooms,u,d,l,r);
    }
    TP = rooms[0];
    cout<<0<<endl;
    while(n--)
    {
        cin>>order;
        TP = TP->move(order,rooms);
    }
    return 0;
}
