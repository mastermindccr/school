#ifndef MENU_H_INCLUDED
#define MENU_H_INCLUDED
#include <string>
#include <vector>
using namespace std;

class Menu{
    public:
        int choice;
        vector<string> choices;

        Menu();
        void show(int high_score);
        void move(int key);
};

#endif