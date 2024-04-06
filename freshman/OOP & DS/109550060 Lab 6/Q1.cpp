#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <algorithm>
using namespace std;

void trim(string &str){
    //TODO 
    for(size_t i = 0;i<str.length();i++)
    {
        if(str[0] == ' ' && str[i] == ' ' && str[i+1] != ' ')
        {
            str = &str[i+1];
            break;
        }
    }
}


void reverse(string &str){
    //TODO
    string tmp;
    for(size_t i = 0;i<str.length();i++)
    {
        tmp += str[str.length()-i-1];
    }
    str = tmp;
}

int main()
{
    string input_line;
      //TODO
      char ch;
    while((ch = getchar())!=EOF)
    {
        input_line += ch;
    }
    trim(input_line);
    reverse(input_line);
    trim(input_line);
    cout << input_line;
    return 0;
}
