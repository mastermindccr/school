#ifndef BOWL_H_INCLUDED
#define BOWL_H_INCLUDED

class Bowl{
    public:
        float x;
        float width;
        float height;
        Bowl(float x_position, float width, float height);
        void draw();
        void move(int key, int row_len);
};

#endif