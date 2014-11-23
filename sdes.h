

#ifndef SDES_H
#define SDES_H

/*************************************************************
Utility Functions
**************************************************************/

void swap(int * array, int i , int j);
void print_array(int * array);
void left_shift_1(int * bits, int low, int high);
void left_shift_2(int * bits, int low, int high);
void int_to_bitset(int value, int * bitset, int N);
int bitset_to_int(int * bitset, int N);
unsigned char bitset_to_char(int * input, int N);
unsigned char bitset_to_char2(int * input, int N);
void XOR(int * a, int * b, int N);
void raise_error(char * msg);




/*************************************************************
S-DES Algorithm Functions
**************************************************************/
void gen_keys(int * key);
void IP_function(int * input, int * output);
void IP_INV_function(int * input, int * output);
void S_function(int * input,int low, int high, const int S[4][4]  , int * S_out );
void F_mapping(int * input, int * sk, int low, int * output);
void f_function(int * input, int * sk, int * output);		//4bit
void SW_function(int * input);
unsigned char encrypt(int c);
unsigned char decrypt(int c);

#endif