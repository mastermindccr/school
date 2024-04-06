#include "1612.h"
#include <iostream>

using namespace std;

int convex_hull(int L, int k) {
    int left = (int)-1e9, right = (int)1e9, slope;
    while(left + 1 < right) {
        slope = (left + right) / 2;
        auto [x, y] = query(slope);
        if (x == k) {
            return y;
        }
        else if (x < k) {
            right = slope;
        }
        else {
            left = slope;
        }
    }
    auto [x, y] = query(right);
    return y+(k-x)*right;
}
