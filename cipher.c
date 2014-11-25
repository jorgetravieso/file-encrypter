
//Run ./cipher 1100011110 11011011 file1.txt file2.txt			encrypts file1.txt as file2.txt
//Run ./cipher -d 1100011110 11011011 file2.txt file3.txt			decrypts file2.txt as file3.txt, where file1 == file3


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "sdes.h"

#define BUFFER_SIZE  1024

void fencrypt(FILE * fpin, FILE * fpout);
void fdecrypt(FILE * fpin, FILE * fpout);
void bitstr_to_bitset(char * bitstr, int * bitset, int N);

int main (int argc, char ** argv)
{

	char * Kstr = "";
	char * IVstr = "";	
	char * in_filename = "";
	char * out_filename = "";
	FILE * fpout;
	FILE * fpin;

	if(argc == 6)								//decriptying flag
	{										
		if(strncmp(argv[1],"-d", 2) == 0){
			Kstr = argv[2];
			IVstr = argv[3];
			in_filename = argv[4];
			out_filename = argv[5];
		}
	}
	else if(argc == 5){
		Kstr = argv[1];
		IVstr = argv[2];
		in_filename = argv[3];
		out_filename = argv[4];
	}
	else{
		printf("run: %s [-d] <init_key> <init_vector> <original_file> <result_file>\n", argv[0]);
		exit(1);
	}


	int K [10];										
	int IV [8];//{};
	bitstr_to_bitset(Kstr, K, 10);
	bitstr_to_bitset(IVstr, IV, 8);
	gen_keys(K, IV);

	fpin = fopen(in_filename, "rb");
	if(fpin == NULL) raise_error("the original_file couldn't be processed. exiting...");
	fpout = fopen(out_filename, "wb");
	if(fpout == NULL) raise_error("the result_file file couldn't be processed. exiting...");

	if(strncmp(argv[1],"-d", 2) == 0){
		printf("%s\n", "decrypting ....");
		fdecrypt(fpin, fpout);
	}
	else{
		printf("%s\n", "encrypting ....");
		fencrypt(fpin, fpout);
	}	

	fflush(fpout);

	printf("%s\n", "finished ....");

	fclose(fpin);
	fclose(fpout);

}

//encrypts an input file in an output file
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



//decrypts an input file in an output file
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

//converts a char *  bitstr to an int * 
void bitstr_to_bitset(char * bitstr, int * bitset, int N)
{
	int i = 0;
	for(; i < N; i++){
		if(bitstr[i] == '1'){
			bitset[i] = 1;
		}
		else if(bitstr[i] == '0'){
			bitset[i] = 0;
		}
		else{
			raise_error("invalid K or IV");
		}
	}

}


