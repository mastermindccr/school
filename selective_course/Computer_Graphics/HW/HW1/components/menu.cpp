#include <GL/gl.h>
#include <GL/glut.h>
#include "headers/menu.h"

Menu::Menu(){
    choice = 0;
    choices = {"New Game", "Help", "Quit"};
}

void Menu::show(int high_score){
    string text = "highest score: " + to_string(high_score);
    glColor3f((float)50/255, (float)216/255, (float)100/255);
    glRasterPos2f(5-0.15*(float(text.size())/2), 9);
    for (int i = 0; i < text.size(); i++) {
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, text[i]);
    }

    for (int i = 0; i < choices.size();i++){
        if(choice==i){
            glColor3f((float)81/255, (float)145/255, (float)255/255);
        }
        else{
            glColor3f((float)173/255, (float)216/255, (float)230/255);
        }
        
        glRasterPos2f(5-0.15*(float(choices[i].size())/2), 7-(float(7-3)*i/(choices.size()-1)));
        
        for (int j = 0; j < choices[i].size(); j++) {
            glutBitmapCharacter(GLUT_BITMAP_8_BY_13, choices[i][j]);
        }
    }
}

void Menu::move(int key){
    if(key==GLUT_KEY_UP && choice>0){
        choice--;
    }
    else if(key==GLUT_KEY_DOWN && choice<choices.size()-1){
        choice++;
    }
}