

#ifndef SDES_H
#define SDES_H

/*************************************************************
Utility Functions
**************************************************************/

//swaps 2 references in an array
void swap(int * array, int i , int j);

//prints and array to stdout
void print_array(int * array);

//LSC-1 circular left shif on an array
void left_shift_1(int * bits, int low, int high);

//LSC-2 circular left shif on an array
void left_shift_2(int * bits, int low, int high);

//converts an int to bitset
void int_to_bitset(int value, int * bitset, int N);

//converts a bitset to int
int bitset_to_int(int * bitset, int N);

//converts a bitset to its representing char
unsigned char bitset_to_char(int * input, int N);

//bit by bit XOR on 2 arrays
void XOR(int * a, int * b, int N);

//error to stderr
void raise_error(char * msg);



/*************************************************************
S-DES Algorithm Functions
**************************************************************/
//passes the initialization keys from the main function to the sdes.c file
void gen_keys(int * key, int * IV);

//IP() as described in the S-DES specification
void IP_function(int * input, int * output);

//IP()-1 as described in the S-DES specification
void IP_INV_function(int * input, int * output);

//S() function as described in the S-DES specification
void S_function(int * input,int low, int high, const int S[4][4]  , int * S_out );

//F() mapping as described in the S-DES specification
void F_mapping(int * input, int * sk, int low, int * output);

//f() function as described in the S-DES specification
void f_function(int * input, int * sk, int * output);		//4bit

//SW() function as described in the S-DES specification
void SW_function(int * input);

//encrypts a char to ciphertext given that the keys where initiliazed correctly in the genkeys()
unsigned char encrypt(int c);

//decrypts a char to text given that the keys where initiliazed correctly in the genkeys()
unsigned char decrypt(int c);

#endif