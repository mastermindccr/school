#include "1622.h"
#include <iostream>
#include <vector>
#include <algorithm>

Number two_increasing_arrays(int n, int k, std::vector<Number> a, std::vector<Number> b) {
    std::vector<Number> tmp{a[0], b[0]}; // this costs 0
    
    std::sort(begin(tmp), end(tmp)); // the sort function will compare Number using operator <
    
    int a0_info = a[0].get_value(); // this function can NOT be called when you submit your solution to Formosa OJ
    
    if (k == 1) {
        if (a[0] < b[0]) { // this costs 1
            return a[0]; // this costs 0
        }
        else {
            return b[0]; // this costs 0
        }
    }
    return a[n-1]; // this costs 0
}
