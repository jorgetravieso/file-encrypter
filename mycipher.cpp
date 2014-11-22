
#include <stdio.h>


int K1[8];												//will contain the 1st key
int K2[8];												//will contain the 2nd and final key			

int P10 [] = {3, 5, 2, 7, 4, 10, 1, 9, 8, 6};
int P8 [] =  {6, 3, 7, 4, 8, 5, 10, 9};
int IP [] = {2, 6, 3, 1, 4, 8, 5, 7};



void gen_keys(int * key);
void print_array(int * array);
void left_shift_1(int * bits, int low, int high);
void left_shift_2(int * bits, int low, int high);

int main (int argc, char ** argv)
{
	char * text = "h";   	
	char ciphertext [8];								//8 chars
	int K [] = {1,0,1,0,0,0,0,0,1,0};
	gen_keys(K);	
	
	int i;
	for(i = 0; i < 8; i++){
		ciphertext[i] = text[IP[i] - 1];
	}



	

}


void gen_keys(int * K)
{
	
	int permuted[10];									//will contain the results of the permutation with P10

	int i;
	for(i = 0; i < 10; i++){							//permute according to P10
		permuted[i] = K[P10[i]-1];
	}
	left_shift_1(permuted, 0, 4);						//apply circular LS-1 to the 5 lower and upper bits 
	left_shift_1(permuted, 5, 9);							
	
	for(i = 0; i < 8; i++){								//generate the K1 from the permutation
		K1[i] = permuted[P8[i]-1];
	}
	printf("K1: ");
	print_array(K1);

	left_shift_2(permuted,0,4);							//apply circular LS-2 to the 4 lower and upper bits 
	left_shift_2(permuted,5,9);

	for(i = 0; i < 8; i++){								//generate the K2 from the K1
		K2[i] = permuted[P8[i]-1];
	}
	printf("K2: ");
	print_array(K2);



}

void left_shift_1(int * bits, int low, int high)
{
	int first = bits[low];
	int i = low + 1;
	for(; i <= high; i++){
		bits[i-1] = bits[i];
	}
	bits[--i] = first;
}

void left_shift_2(int * bits, int low, int high)
{
	left_shift_1(bits,low,high);
	left_shift_1(bits,low,high);
}

void print_array(int * array)
{
	int i;
	for(i = 0; i < 8; i++){
		printf("%d", array[i]);
	}
	printf("\n");
}

int * int_to_bitstr(int number)
{

}



