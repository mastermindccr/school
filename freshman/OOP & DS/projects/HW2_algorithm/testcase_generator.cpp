#include<ctime>
#include<cstdlib>
#include<cmath>
#include<iostream>
#include<fstream>

using namespace std;

int main(){
	ofstream file;
	file.open("testcase.txt",ios::trunc);
	srand(time(NULL));
	for(int j = 0;j<5;j++){
		file<<9000<<endl;
		for(int i = 0;i<15;i++){
			for(int k = 0;k<600;k++)
				file<<rand()%19999-9999<<" ";
			file<<endl;
		}
	}
	file<<0<<endl;
}
