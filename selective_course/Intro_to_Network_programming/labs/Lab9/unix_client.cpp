#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <iostream>

#define bufsize 2000

using namespace std;

int sudoku[9][9];

bool isPresentInCol(int col, int num){ //check whether num is present in col or not
   for (int row = 0; row < 9; row++)
      if (sudoku[row][col] == num)
         return true;
   return false;
}

bool isPresentInRow(int row, int num){ //check whether num is present in row or not
   for (int col = 0; col < 9; col++)
      if (sudoku[row][col] == num)
         return true;
   return false;
}

bool isPresentInBox(int boxStartRow, int boxStartCol, int num){
//check whether num is present in 3x3 box or not
   for (int row = 0; row < 3; row++)
      for (int col = 0; col < 3; col++)
         if (sudoku[row+boxStartRow][col+boxStartCol] == num)
            return true;
   return false;
}

void sudokusudoku(){ //print the sudoku sudoku after solve
   for (int row = 0; row < 9; row++){
      for (int col = 0; col < 9; col++){
         if(col == 3 || col == 6)
            cout << " | ";
         cout << sudoku[row][col] <<" ";
      }
      if(row == 2 || row == 5){
         cout << endl;
         for(int i = 0; i<9; i++)
            cout << "---";
      }
      cout << endl;
   }
}

bool findEmptyPlace(int &row, int &col){ //get empty location and update row and column
   for (row = 0; row < 9; row++)
      for (col = 0; col < 9; col++)
         if (sudoku[row][col] == 0) //marked with 0 is empty
            return true;
   return false;
}

bool isValidPlace(int row, int col, int num){
   //when item not found in col, row and current 3x3 box
   return !isPresentInRow(row, num) && !isPresentInCol(col, num) && !isPresentInBox(row - row%3 ,
col - col%3, num);
}

bool solveSudoku(){
   int row, col;
   if (!findEmptyPlace(row, col))
      return true; //when all places are filled
   for (int num = 1; num <= 9; num++){ //valid numbers are 1 - 9
      if (isValidPlace(row, col, num)){ //check validation, if yes, put the number in the sudoku
         sudoku[row][col] = num;
         if (solveSudoku()) //recursively go for other rooms in the sudoku
            return true;
         sudoku[row][col] = 0; //turn to unassigned space when conditions are not satisfied
      }
   }
   return false;
}

int main(int argc, char* argv[])
{
	int sock = 0;
	struct sockaddr_un remote, sockfd;
	char buf[2000];
	bzero(buf, 2000);
	
	const char* socket_path = "/sudoku.sock";

	if( (sock = socket(AF_LOCAL, SOCK_STREAM, 0)) == -1  )
	{
		printf("Client: Error on socket() call \n");
		return 1;
	}
	memset(&remote, 0, sizeof(struct sockaddr_un));
	remote.sun_family = AF_LOCAL;
	strcpy( remote.sun_path, socket_path );
	cout<<"before connection"<<'\n';
	if( connect(sock, (struct sockaddr*)&remote, sizeof(remote)) == -1 )
	{
		cout<<"Client: Error on connect call \n";
		return 1;
	}
	cout<<"connected"<<'\n';

	string tmp = "S";
	write(sock, tmp.data(), tmp.length());
	read(sock, buf, 200);
	for(int i = 4;i<85;i++){
		int row = (i-4)/9;
		int col = (i-4)%9;
		if(buf[i]!='.') sudoku[row][col] = buf[i]-48;
		else sudoku[row][col] = 0;
	}
	solveSudoku();

	for(int i = 0;i<9;i++){
		for(int j = 0;j<9;j++){
			string ret = "V "+to_string(i)+" "+to_string(j)+" "+to_string(sudoku[i][j]);
			write(sock, ret.data(), ret.length());
			usleep(10000);
			cout<<sudoku[i][j]<<' ';
		}
		cout<<'\n';
	}

	tmp = "C";
	write(sock, tmp.data(), tmp.length());
	bzero(buf, bufsize);
	read(sock, buf, bufsize);
	cout<<buf<<'\n';
	close(sock);
}