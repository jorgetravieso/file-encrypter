
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sdes.h"
//TODO change int [] to 


void fencrypt(FILE * in, FILE * out);
void fdecrypt(FILE * fpin, FILE * fpout);
 
int main (int argc, char ** argv)
{

	int K [] = {1,1,0,0,0,1,1,1,1,0};
	gen_keys(K);

	char * in_filename = "";// = "file1.txt";
	char * out_filename = "";// = "file2.txt";	
	FILE * fpout;
	FILE * fpin;

	if(argc == 6){										//decriptying
		if(strncmp(argv[1],"-d", 2) == 0){
			in_filename = argv[4];
			out_filename = argv[5];
		}
	}
	else if(argc == 5){
		in_filename = argv[3];
		out_filename = argv[4];
	}
	else{
		printf("run: %s [-d] <init_key> <init_vector> <original_file> <result_file>\n", argv[0]);
		exit(1);
	}

	fpin = fopen(in_filename, "r");
	if(fpin == NULL) raise_error("the fpin file couldn't be processed. exiting...");
	fpout = fopen(out_filename, "w");
	if(fpout == NULL) raise_error("the fpout file couldn't be processed. exiting...");

	if(strncmp(argv[1],"-d", 2) == 0)
		fdecrypt(fpin, fpout);
	else
		fencrypt(fpin, fpout);

	fflush(fpout);
	fclose(fpin);
	fclose(fpout);



	//char c = '(';   	
	//int K [] = {1,0,1,0,0,0,0,0,1,0};


	//printf("char = %c\n", c);
	//int cipher = encrypt(c);
	//printf("encrypt() -> %d\n", cipher);
	//printf("decrypt() -> %d\n", decrypt(cipher));


//	IP_INV_function(IP_Output);
	}

	void fencrypt(FILE * fpin, FILE * fpout){
		char buffer[1024];
		int i, n = 0;
		while(!feof( fpin )){
			memset(buffer, 0, 1024);   
			n = fread( buffer, 1024,1, fpin);
			if( ferror( fpin ) ) raise_error("can't read from file");
			printf("n:%d\n", n);
			for(i = 0; i < 1024; i++){
				if(buffer[i] == 0) break;
				unsigned char in = buffer[i];
				unsigned char out =  encrypt(in);
				fwrite(&out,1,1,fpout);
			}	
		}
	}

	void fdecrypt(FILE * fpin, FILE * fpout){
		char buffer[1024];
		int i, n = 0;
		while(!feof( fpin )){
			memset(buffer, 0, 1024);   
			n = fread( buffer, 1024,1, fpin);
			if( ferror( fpin ) ) raise_error("can't read from file");
			printf("n:%d\n", n);
			for(i = 0; i < 1024; i++){
				if(buffer[i] == 0) break;
				unsigned char in = buffer[i];
				unsigned char out =  decrypt(in);
				fwrite(&out,1,1,fpout);
			}	
		}
	}



