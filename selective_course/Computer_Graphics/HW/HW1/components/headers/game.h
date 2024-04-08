#ifndef GAME_H_INCLUDED
#define GAME_H_INCLUDED
#include <vector>
#include <string>
#include "ball.h"
#include "bowl.h"
using namespace std;

class Game{
    public:
        vector<Ball*> balls;
        Bowl* bowl;
        int ball_num;
        float row_len;
        float col_len;
        int high_score;
        int score;
        int status; // 0: playing, 1: game over
        int time;

        Game(int ball_num, float row_len, float col_len);
        void init();
        void show();
        void show_text(string text, float x_pos, float y_pos);
};

#endif