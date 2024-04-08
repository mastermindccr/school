#include <GL/gl.h>
#include <GL/glut.h>
#include "headers/controller.h"
using namespace std;

Controller::Controller(int ball_num, float row_len, float col_len){
    menu = new Menu();
    game = new Game(ball_num, row_len, col_len);
    pause = new Pause();
    view = 0;
}

void Controller::show(){
    if(view==0){
        menu->show(game->high_score);
    }
    else if(view==1){
        help();
    }
    else if(view==2){
        game->show();
    }
    else if(view==3){
        pause->show();
    }
}

void Controller::help(){
    vector<RGB> colors = {{1, 0, 0}, {1, 1, 0}, {1, 1, 1}, {1, 0, 1}};
    vector<string> explanation = {"general, get 5pts if catch", "faster, get 10pts if catch", "bomb, die if catch", "booster, basket wider if catch"};
    for (int i = 0; i < colors.size();i++)
    {
        glPushMatrix();
        glColor3f(colors[i].r, colors[i].g, colors[i].b);
        glTranslatef(2, 8-2*i, 0);
        glutSolidSphere(0.5, 25, 25);
        glPopMatrix();

        glColor3f((float)173/255, (float)216/255, (float)230/255);
        glRasterPos2f(3.5, 8-2*i);
        for (int j = 0; j < explanation[i].size(); j++) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, explanation[i][j]);
        }
    }

    string back = "Press Esc to go back";
    glColor3f((float)173/255, (float)216/255, (float)230/255);
    glRasterPos2f(5-0.15*(float(back.size())/2), 0.5);
    for (int i = 0; i < back.size(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, back[i]);
    }
}

void Controller::menu_select(){
    if(menu->choice==0){ // game
        game->init();
        view = 2;
    }
    else if(menu->choice==1){ // help
        view = 1;
    }
    else{ // quit
        exit(0);
    }
}

void Controller::pause_select(){
    if(pause->choice==0){ // resume
        view = 2;
    }
    else if(pause->choice==1){ // restart
        game->init();
        view = 2;
    }
    else{ // main menu
        view = 0;
    }
}