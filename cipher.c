
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sdes.h"
//TODO change int [] to 

#define BUFFER_SIZE  1024

void fencrypt(FILE * fpin, FILE * fpout);
void fdecrypt(FILE * fpin, FILE * fpout);

int main (int argc, char ** argv)
{

	int K [] = {1,1,0,0,0,1,1,1,1,0};
	int IV [] = {1,1,0,1,1,0,1,1};

	gen_keys(K, IV);

	char * in_filename = "";
	char * out_filename = "";
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
	if(fpin == NULL) raise_error("the original_file couldn't be processed. exiting...");
	fpout = fopen(out_filename, "w");
	if(fpout == NULL) raise_error("the result_file file couldn't be processed. exiting...");

	if(strncmp(argv[1],"-d", 2) == 0)
		fdecrypt(fpin, fpout);
	else
		fencrypt(fpin, fpout);

	fflush(fpout);
	fclose(fpin);
	fclose(fpout);

}


void fencrypt(FILE * fpin, FILE * fpout)
{
	char buffer[BUFFER_SIZE];
	int i, n = 0;
	while (1){
		n = fread(buffer, 1 , BUFFER_SIZE, fpin);
        for(i = 0; i < n; i++){
			//if(buffer[i] == 0) break;
			unsigned char in = buffer[i];
			unsigned char out =  encrypt(in);
			fwrite(&out,1,1,fpout);
		}
		if(n != BUFFER_SIZE){
			break;
		}		
	}
}

void fdecrypt(FILE * fpin, FILE * fpout)
{
	char buffer[BUFFER_SIZE];
	int i, n = 0;
	while (1){
		n = fread(buffer, 1 , BUFFER_SIZE, fpin);
        for(i = 0; i < n; i++){
			//if(buffer[i] == 0) break;
			unsigned char in = buffer[i];
			unsigned char out =  decrypt(in);
			fwrite(&out,1,1,fpout);
		}
		if(n != BUFFER_SIZE){
			break;
		}	
	}
}



