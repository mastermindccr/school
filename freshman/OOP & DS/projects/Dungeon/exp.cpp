#include "exp.h"
#include<math.h>

int exp(int level){
    return (int)pow(10,(double)(10+level)/10);
}
