#include <iostream>
using namespace std;

class Room{
public:
    Room *left_room;
    Room *right_room;
    int index;
  //add constructor or functions if you need
};

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */
    int num,step;
    char cmd;
    Room *TP;
    TP = new Room;
    TP->left_room = nullptr;
    cin>>num;
    for(int i = num;i>0;i--)
    {
        if(i==num)
        {
            cin>>TP->index;
        }
        else
        {
            Room *New,*cur;
            New = new Room;
            cin>>New->index;
            cur = TP;
            cur->right_room = New;
            New->left_room = cur;
            New->right_room = nullptr;
            TP = New;
        }
    }
    while(TP->left_room != nullptr)
    {
        TP = TP->left_room;
    }
    cin>>step;
    cout<<TP->index<<' ';
    while(step--)
    {
        cin>>cmd;
        if(cmd == 'l')
            if(TP->left_room == nullptr)
                cout<<-1<<' ';
            else
            {
                TP = TP->left_room;
                cout<<TP->index<<' ';
            }
        else
            if(TP->right_room == nullptr)
                cout<<-1<<' ';
            else
            {
                TP = TP->right_room;
                cout<<TP->index<<' ';
            }
    }
    return 0;
}
