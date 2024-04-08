#include <GL/gl.h>
#include <GL/glut.h>
#include "headers/game.h"
using namespace std;

Game::Game(int ball_num, float row_len, float col_len){
    this->ball_num = ball_num;
    this->row_len = row_len;
    this->col_len = col_len;
    high_score = 0;
}

void Game::init(){
    bowl = new Bowl(row_len / 2, 0.6, 1);
    balls.clear();
    for (int i = 0; i < ball_num;i++){
        balls.push_back(new Ball(row_len, col_len, i));
    }
    time = 60;
    score = 0;
    status = 0;
}

void Game::show(){
    if(!status){
        show_text(to_string(time), 5 - 0.15 * (float(to_string(time).size()) / 2), 9.5);
        bowl->draw();
        for (int i = 0; i < ball_num;i++){
            balls[i]->draw(status, score, bowl->x, bowl->width, bowl->height);
        }
        string text = "points: " + to_string(score);
        glColor3f((float)173/255, (float)216/255, (float)230/255);
        show_text(text, 8, 9.5);
    }
    else{
        if(score>high_score) high_score = score;
        string text = "final score: " + to_string(score);
        glColor3f((float)150/255, (float)140/255, (float)100/255);
        show_text(text, 5 - 0.15 * (float(text.size()) / 2), 8);

        text = "highest score: " + to_string(high_score);
        glColor3f((float)50/255, (float)216/255, (float)100/255);
        show_text(text, 5 - 0.15 * (float(text.size()) / 2), 6.5);

        text = "Restart?";
        glColor3f((float)173/255, (float)216/255, (float)230/255);
        show_text("Restart?", 5 - 0.15 * (float(text.size()) / 2), 4);

        text = "Y for Yes / N for No";
        show_text(text, 5 - 0.15 * (float(text.size()) / 2), 3);
    }
}

void Game::show_text(string text, float x_pos, float y_pos){
    glRasterPos2f(x_pos, y_pos);

    for (int i = 0; i < text.size(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, text[i]);
    }
}