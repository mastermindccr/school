#ifndef CONTROLLER_H_INCLUDED
#define CONTROLLER_H_INCLUDED

#include <vector>
#include <string>
#include "menu.h"
#include "game.h"
#include "pause.h"

using namespace std;

class Controller{
    public:
        int view; // 0: menu, 1: help, 2: game, 3: pause
        Menu *menu;
        Game *game;
        Pause *pause;

        Controller(int ball_num, float row_len, float col_len);
        void show();
        void help();
        void menu_select();
        void pause_select();
};

#endif