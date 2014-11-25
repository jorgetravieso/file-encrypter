

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "sdes.h"


int IV[8];

int K1[8];												//will contain the 1st key
int K2[8];												//will contain the 2nd and final key			

const int P10 [] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};
const int P8 [] =  {6, 3, 7, 4, 8, 5, 10, 9};
const int IP [] = {2, 6, 3, 1, 4, 8, 5, 7};
const int IP_INV[] = {4, 1, 3, 5, 7, 2, 8, 6};
const int EP[] = {4, 1, 2, 3, 2, 3, 4, 1 };

const int S0[][4]  = {{1, 0, 3, 2},
{3, 2, 1, 0},
{0, 2, 1, 3},
{3, 1, 3, 2}};

const int S1[][4]  = {{0, 1, 2, 3},
{2, 0, 1, 3},
{3, 0, 1, 0},
{2, 1, 0, 3}};



int IP_Output[8];
int IP_Inv_Output[8];
int F_Mapping_Output[4];
int f1_Function_Output[8];
int f2_Function_Output[8];
int S0_out[2];
int S1_out[2];




//GOOD
void gen_keys(int * K , int aIV[])
{


	memcpy(IV,aIV, sizeof(int) * 8);						//copy the initial vector
	memcpy(IP_Inv_Output,aIV, sizeof(int) * 8);				//in case of encryption the IP_Inv_Output will contain the results of every encryption
															
	
	int permuted[10];										//will contain the results of the permutation with P10

	int i;
	for(i = 0; i < 10; i++){								//P10() permute
		permuted[i] = K[P10[i]-1];
	}
	left_shift_1(permuted, 0, 4);							//apply circular LS-1 to the 5 lower and upper bits 
	left_shift_1(permuted, 5, 9);							
	
	for(i = 0; i < 8; i++){									//P8() permute
		K1[i] = permuted[P8[i]-1];
	}
	left_shift_2(permuted,0,4);								//apply circular LS-2 to the 4 lower and upper bits 
	left_shift_2(permuted,5,9);

	for(i = 0; i < 8; i++){									//generate the K2 from the K1 using P8()
		K2[i] = permuted[P8[i]-1];
	}

}

//GOOD
void IP_function(int * input, int * output)
{
	int i;
	for(i = 0; i < 8; i++){									//IP permute
		output[i] = input[ IP[i] - 1];
	}
}
//GOOD
void IP_INV_function(int * input, int * output)
{
	int i;
	for(i = 0; i < 8; i++){									//IP permute
		output[i] = input[ IP_INV[i] - 1];
	}
}





unsigned char encrypt(int c)								//pass output as arg to the functions;
{								
	
	printf("%s\n", "\nencrypting ....");
	int plaintext[8];										//bitset of plaintext
	int_to_bitset(c,plaintext,8);							//will convert the char to a bitset
	XOR(plaintext,IP_Inv_Output,8);			 				//CBC the output of the previous call is used to XOR the plaintext
	IP_function(plaintext, IP_Output);						//IP() initial permutation	
	f_function(IP_Output, K1, f1_Function_Output);			//f1() function
	SW_function(f1_Function_Output);						//SW() function (switch)	
	f_function(f1_Function_Output, K2, f2_Function_Output);	//f2() function
	IP_INV_function(f2_Function_Output,IP_Inv_Output);		//IP-1() function
	return bitset_to_char(IP_Inv_Output,8);					//convert to char the bitset

}
unsigned char decrypt(int c){
	
	printf("%s\n", "\ndecrypting ....");
	int ciphertext[8];
	int_to_bitset(c,ciphertext,8);							//will convert the int value to a bitset
	IP_function(ciphertext,IP_Output);						//
	f_function(IP_Output, K2, f2_Function_Output);
	SW_function(f2_Function_Output);
	f_function(f2_Function_Output, K1, f1_Function_Output);
	IP_INV_function(f1_Function_Output,IP_Inv_Output);
	XOR(IP_Inv_Output,IV, 8);
	memcpy(IV, ciphertext, sizeof(int) * 8);
	return bitset_to_char(IP_Inv_Output,8);
}


void SW_function(int * input){						//TO-CHECK
	int i= 0;
	for(; i < 4; i++){
		swap(input, i,i+4);
	}
}


void f_function(int * input, int * sk, int * output)
{	

	//fK(L, R) = (L XOR F(R, SK), R)
	//sk some key
													//let L and R be the left and right 4 bits of input
	F_mapping(input, sk, 4, F_Mapping_Output);	//F(R, SK)
	XOR(F_Mapping_Output, input, 4);				//L XOR F(R, SK) 		
	int i;
	for(i = 0; i < 4; i++)
		output[i] = F_Mapping_Output[i];
	for(i = 4; i < 8; i++)
		output[i] = input[i];

}

void F_mapping(int * input, int * sk, int low, int * output){
	

	int EP_Ouput[8];
	int i;
	for(i = 0; i < 8; i++){							//EP() expand and permute and XOR() to LEFT
		EP_Ouput[i] = input[low + EP[i]-1] ^ sk[i];
	}
													//S0() and S1() boxes
	S_function(EP_Ouput,0,3,S0,S0_out);
	S_function(EP_Ouput,4,7,S1,S1_out);				//F() ---> SO() Union S1()  

													//P4 permutation
	output[0] =  S0_out[1];
	output[1] =  S1_out[1];
	output[2] =  S1_out[0];
	output[3] =  S0_out[0];

}

void S_function(int * input,int low, int high, const int S [4][4]  , int * S_out ){
	int i_arr[] = {input[low], input[high]};
	int j_arr[] = {input[low+1], input[high-1]};
	int i = bitset_to_int(i_arr,2);
	int j = bitset_to_int(j_arr,2);
	int val = S[i][j];
	//printf("val: %d\n", val);
	int_to_bitset(val, S_out, 2);

}


/*************************************************************
Utility Functions
**************************************************************/



//GOOD
void int_to_bitset(int value, int * bitset, int N)
{
	if(value < 0){
	//	raise_error("invalid number range. exiting...");
	}	
	memset(bitset,0,N);
	int i;
	for(i = N-1; i >= 0; i--){
		bitset[i] = value % 2;
		value /= 2;
	}
}

int bitset_to_int(int * bitset, int N)
{
	int i, value  =0;
	int p = 0;
	for(i = N - 1; i >= 0; i--){
		if (bitset[i] == 1){
			value += pow(2,p);
		}
		p++;
	}

	return value;
}

unsigned char bitset_to_char(int * input, int N)
{
	if(N < 0 || N > 8){
		raise_error("\nWRONG LENGTH STRING");
	}
	return bitset_to_int(input,8);
}


//GOOD
void print_array(int * array)
{
	int i;
	for(i = 0; i < 8; i++){
		printf("%d", array[i]);
	}
	printf("\n");
}
//GOOD
void swap(int * array, int i , int j)
{
	int temp = array[i];
	array[i] = array[j];
	array[j] = temp;
}	


//GOOD
void left_shift_1(int * bits, int low, int high)
{
	int first = bits[low];
	int i = low + 1;
	for(; i <= high; i++){
		bits[i-1] = bits[i];
	}
	bits[--i] = first;
}
//GOOD
void left_shift_2(int * bits, int low, int high)
{
	left_shift_1(bits,low,high);
	left_shift_1(bits,low,high);
}

void XOR(int * a, int * b, int N)
{
	for(int i = 0; i < N; i++)
		a[i] = a[i] ^ b[i];
}

void raise_error(char * msg) { perror(msg); exit(-1); } 




