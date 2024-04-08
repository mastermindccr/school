#ifndef BALL_H_INCLUDED
#define BALL_H_INCLUDED

typedef struct {
    int r;
    int g;
    int b;
} RGB;

class Ball{
    public:
        float row_len;
        float col_len;
        int type;
        float x;
        float y;
        float speed;
        int point;
        RGB color;
        Ball(float row_len, float col_len, int rand_seed_param); // types of ball
        void init();
        void draw(int& status, int& score, float center, float& width, float& height);
};

#endif