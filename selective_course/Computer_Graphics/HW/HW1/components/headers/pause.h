#ifndef PAUSE_H_INCLUDED
#define PAUSE_H_INCLUDED
#include <string>
#include <vector>
using namespace std;

class Pause{
    public:
        int choice; // 0: resume, 1: menu
        vector<string> choices;

        Pause();
        void show();
        void move(int key);
};

#endif