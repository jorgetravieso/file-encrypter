/*

  This is program for Encryption and Decryption
  This program uses the Simple Data Encryption Standard (SDES) Algorithm.
  This Algo takes 8-bits of plaintext at a time and produces 8-bits of ciphertext.
  It uses 10-bits of key for Encryption and Decryption.
  
  Developed by : Vivek Kumar (alldogblue@yahoo.co.in)
                 
   Created on : 31 March 2005
   Last Modified on : 10 April 2005

   Any sort of suggetions/comments are most welcome on alldogblue@yahoo.co.in

*/

#include<iostream.h>
#include<stdio.h>
#include<conio.h>
#include<string.h>
#include<stdlib.h>
#include<assert.h>

void mainmenu(int *);
void menuEn(char*,char*,char*);
void menuDe(char*,char*,char*);
int DoEnDe(char*,char*,char*,int);
class SDES
{
  private:
	char KEY[11],K1[9],K2[9],IPOutput[9],InvIPOutput[9];
	char F1Output[9],F2Output[9];

	char INPUT_BIT[9],OUTPUT_BIT[9];

  public:
   unsigned char INPUT,OUTPUT;

   SDES(char *key);
   ~SDES();
   void GenerateKeys();
   char *Left_Shift(char *,int );
   void conv_to_bits(unsigned char );
   void IP(char *);
   void InvIP(char *);
   void DES_Encryption(unsigned char );
   void DES_Decryption(unsigned char );
   void Function_F(char *,char *,int );
   char *EX_OR(char *,int );
   char *SBOX0(char *);
   char *SBOX1(char *);
   void SDES::GetChar();
};

	/*Method to perform SDES-Encryption on 8-bit 'input'*/
	void SDES::DES_Encryption(unsigned char input)
	{
	  char LIP[5],RIP[5],L1[5],R1[5];
	  int i;

	  INPUT=input;
	  conv_to_bits(INPUT);  //Converts the input to bit-string
	  IP(INPUT_BIT);        //Initial-Permutation

	  gotoxy(1,1);
	  printf("\nEncrpyting.........");

	  /*Dividing the output of IP into 2 parts*/
	  for(i=0;i<4;i++)
	  {
		LIP[i]=IPOutput[i];
		RIP[i]=IPOutput[i+4];
	  }
	  LIP[4]='\0';
	  RIP[4]='\0';

	  /*Sending the above divided parts to Function_F and  sub-key K1*/
	  Function_F(LIP,RIP,1);

	  /*Dividing the output of the Function_F into 2 parts*/
	  for(i=0;i<4;i++)
	  {
		L1[i]=F1Output[i];
		R1[i]=F1Output[4+i];
	  }
	  L1[4]='\0';
	  R1[4]='\0';

	  /*This time the string-parameters swaped and uses sub-key K2*/
	  Function_F(R1,L1,2);

	  /*Performing the Inverse IP on the output of the Funtion_F*/
	  InvIP(F1Output); //The output of the function will give us
			   //Cipher-string

	  /*Cipher string is converted back to unsigned char and stored
	    in private-variable OUTPUT of this class*/
	  GetChar();
	}


	/*Decryption is just inverse of Encryption
	  Here IP, InvIP, E/P, SBOX1 and SBOX2 are same
	  But Function_F first operats on sub-key K2 and
	  then on sub-key K1*/
	void SDES::DES_Decryption(unsigned char input)
	{
	  char LIP[5],RIP[5],L1[5],R1[5];
	  int i;

	  INPUT=input;
	  conv_to_bits(INPUT);
	  IP(INPUT_BIT);        //Initial-Permutation

	  gotoxy(1,1);
	  printf("\nDecrpyting.........");

	  for(i=0;i<4;i++)
	  {
		LIP[i]=IPOutput[i];
		RIP[i]=IPOutput[i+4];
	  }
	  LIP[4]='\0';
	  RIP[4]='\0';

	  Function_F(LIP,RIP,2);

	  for(i=0;i<4;i++)
	  {
		L1[i]=F1Output[i];
		R1[i]=F1Output[4+i];
	  }
	  L1[4]='\0';
	  R1[4]='\0';

	  Function_F(R1,L1,1);
	  InvIP(F1Output);
	  GetChar();
	}

	void SDES::Function_F(char *linput,char *rinput,int key)
	{
	 int E_P[8]={4,1,2,3,2,3,4,1}; //E/P Operation-Array
	 int P4[4]={2,4,3,1};          //P4 Operation-Array
	 int i;
	 char E_POutput[9],*EXOR_Output,*LEXOR,*REXOR;
	 char *SBOX0_Output,*SBOX1_Output;
	 char SBOX_Output[5];
	 char P4_Output[5];
	 char fk_Output[5];
	 char Main_Output[9];

	 /*E/P Operaion is performed here*/
	 for(i=0;i<8;i++)
	 {
	  E_POutput[i]=rinput[E_P[i]-1];
	 }
	 E_POutput[8]='\0';

	 /*Bitwise-EXOR is done on E/P Output and sub-key(K1/K2)*/
	 EXOR_Output=EX_OR(E_POutput,key);

	 /*Divide the output of Exor in 2 parts*/
	 LEXOR=new char[strlen(EXOR_Output)/2+1];
	 REXOR=new char[strlen(EXOR_Output)/2+1];

	 for(i=0;i<strlen(EXOR_Output)/2;i++)
	 {
	  LEXOR[i]=EXOR_Output[i];
	  REXOR[i]=EXOR_Output[i+4];
	 }
	 LEXOR[4]=REXOR[4]='\0';


	 /*Peforming SBOX0 Operation on left 4 bits*/
	 SBOX0_Output=SBOX0(LEXOR);

	 /*Peforming SBOX1 Operation on right 4 bits*/
	 SBOX1_Output=SBOX1(REXOR);

	 /*Combining the 2-bits output of both SBOXES in one string*/
	 for(i=0;i<2;i++)
	 {
	  SBOX_Output[i]=SBOX0_Output[i];
	  SBOX_Output[i+2]=SBOX1_Output[i];
	 }
	 SBOX_Output[4]='\0';

	 /*Performing the P4 operation on SBOX output*/
	 for(i=0;i<4;i++)
	 {
	  P4_Output[i]=SBOX_Output[P4[i]-1];
	 }
	 P4_Output[4]='\0';

	 /*Performing the EXOR operation on 4-bits P4-output
	   and 4-bits Leftinput of Funtion_F*/
	 for(i=0;i<4;i++)
	 {
	  if(P4_Output[i]==linput[i])
	   fk_Output[i]='0';
	  else
	   fk_Output[i]='1';
	 }
	 fk_Output[4]='\0';

	 /*Cancating the 4-bits output of above EXOR-operation
	   and 4-bits Right-input of Function_F*/
	 for(i=0;i<4;i++)
	 {
	  Main_Output[i]=fk_Output[i];
	  Main_Output[i+4]=rinput[i];
	 }
	 Main_Output[8]='\0';
	 /*Assigning this Cucaneted string to Private variable 'F1Output'*/
	 strcpy(F1Output,Main_Output);
	}

	/*This method EXORS the output ofE/P and sub-keys
	  depending on the parameter k.
	  k=1:subkey K1  k=2:subkey K2*/
	char *SDES::EX_OR(char *ep,int k)
	{
	 char *output,*key;
	 int i,klen;

	 output=new char[strlen(ep)+1];
	 key=new char[strlen(K1)+1];
	 if(k==1)
	 {
	  strcpy(key,K1);
	 }
	 else
	 {
	  if(k==2)
	  {
	   strcpy(key,K2);
	  }
	  else
	  {
	   printf("\n\nWrong Choice in the key parameter(1/2)");
	   getch();
	   exit(1);
	  }
	 }
	 klen=strlen(K1);
	 if(strlen(ep)!=klen)
	 {
	  printf("\ninput=%d is not equal to K=%d",strlen(ep),klen);
	  printf("\n\nError in the Output of E/P (Length)..Press any key");
	  getch();
	  exit(1);
	 }
	 for(i=0;i<strlen(ep);i++)
	 {
	  if(ep[i]==key[i])
	   output[i]='0';
	  else
	   output[i]='1';
	 }
	 output[strlen(ep)]='\0';
	 return(output);
	}

	/*SBOX0 Operation is defined here*/
	char *SDES::SBOX0(char *l)
	{
	 int S0[4][4]={1,0,3,2,  //S0 Matrix
		       3,2,1,0,
		       0,2,1,3,
		       3,1,3,2
		       };

	 char *bits[]={"00","01","10","11"};
	 char lrow[3],lcol[3];
	 char *SO;
	 int i,lr,lc,b;

	 SO=new char[3];

	 lrow[0]=l[0];
	 lrow[1]=l[3];
	 lcol[0]=l[1];
	 lcol[1]=l[2];

	 lrow[2]='\0';
	 lcol[2]='\0';


	 for(i=0;i<4;i++)
	 {
	  if(strcmp(lrow,bits[i])==0)
	   lr=i;
	  if(strcmp(lcol,bits[i])==0)
	   lc=i;
	 }
	 b=S0[lr][lc];
	 for(i=0;i<3;i++)
	  SO[i]=bits[b][i];
	 SO[3]='\0';
	 return(SO);
	}
	/*SBOX1 Operation is defined here*/
	char *SDES::SBOX1(char *l)
	{
	 int S0[4][4]={0,1,2,3,   //S1 Matrix
		       2,0,1,3,
		       3,0,1,0,
		       2,1,0,3
		       };

	 char *bits[]={"00","01","10","11"};
	 char lrow[3],lcol[3];
	 char *SO;
	 int i,lr,lc,b;

	 SO=new char[3];

	 lrow[0]=l[0];
	 lrow[1]=l[3];
	 lcol[0]=l[1];
	 lcol[1]=l[2];

	 lrow[2]='\0';
	 lcol[2]='\0';


	 for(i=0;i<4;i++)
	 {
	  if(strcmp(lrow,bits[i])==0)
	   lr=i;
	  if(strcmp(lcol,bits[i])==0)
	   lc=i;
	 }
	 b=S0[lr][lc];
	 for(i=0;i<3;i++)
	  SO[i]=bits[b][i];

	 SO[3]='\0';
	 return(SO);
	}

	/*Method to get back unsigned char from bit-string*/
	void SDES::GetChar()
	{
	   int i,j,in;
	   unsigned char ch=0;
	   char *bs;
	   bs=new char[9];
	   bs=InvIPOutput;
	   if(strlen(bs)>8)
	   {
	    printf("\nWRONG LENGTH STRING");
	    exit(0);
	   }
	   for(i=0;i<8;i++)
	   {
	    if(bs[i]=='1')
	    {
	     in=1;
	     for(j=1;j<8-i;j++)
	     {
	       in=in*2;
	     }
	     ch=ch+in;
	    }
	   }
	   OUTPUT=ch;
	  }

	/*Destructor*/
	SDES::~SDES()
	{
	}


void main(void)
{
 clrscr();
 unsigned char ch,ch1;
 int i,n=10,choice;
 char *key;//="1010000010";
 char *sfname,*tfname;
 while(1)
 {
  key = new char[11];
  sfname = new char[20];
  tfname = new char[20];
  mainmenu(&choice);
  fflush(stdin);
  switch(choice)
  {
   case 1:
	 menuEn(tfname,sfname,key);
	 DoEnDe(tfname,sfname,key,choice);
	 break;
   case 2:
	 menuDe(tfname,sfname,key);
	 DoEnDe(tfname,sfname,key,choice);
	 break;
   case 3:
	 exit(0);
   default:
	 printf("\nWrong Choice Enter again\nPress any key to return to Main Menu..");
	 getch();
	 break;
  }
 }
}

void mainmenu(int *c)
{
  clrscr();
  printf("\nWhat do you want to do..");
  printf("\n1. Encryption");
  printf("\n2. Decryption");
  printf("\n3. Exit");
  printf("\n\nEnter the choice? ");
  scanf("%d",c);
}
void menuEn(char *t,char *s,char *k)
{
 clrscr();
 printf("\nEncryption Menu\n\n");
 printf("\nEnter the filename to be Encrypted: ");
 gets(s);
 printf("\nEnter the Target file name: ");
 gets(t);
 printf("\nEnter the 10-bits KEY: ");
 gets(k);
 printf("\n\nNotedown this key, as same key is used for Decryption");
 //getch();
}
void menuDe(char *t,char *s,char *k)
{
 clrscr();
 printf("\nDecryption Menu\n\n");
 printf("\nEnter the filename to be Decrypted: ");
 gets(s);
 printf("\nEnter the Target file name: ");
 gets(t);
 printf("\nEnter the 10-bits KEY: ");
 gets(k);
}

int DoEnDe(char *t,char *s,char *k,int c)
{

 printf("\n\nTarget = %s Source = %s key = %s choice = %d",t,s,k,c);

 SDES S(k);
 int i,n;
 n=10; //Number of Rounds
 unsigned char ch;
 FILE *fp,*ft;
 fp=fopen(t,"wb");
 ft=fopen(s,"rb");
 if(fp==NULL||ft==NULL)
 {
  printf("\nFile not opened SORRY");
  getch();
  fclose(ft);
  fclose(fp);
  return(0);
 }
 while(fread(&ch,1,1,ft)==1)
 {
  S.OUTPUT=ch;

  for(i=0;i<n;i++)
  {
   if(c==1)
    S.DES_Encryption(S.OUTPUT);
   if(c==2)
    S.DES_Decryption(S.OUTPUT);
  }
  fwrite(&S.OUTPUT,1,1,fp);
 }
 printf("\nCompleted!!!!!");
 getch();
 fclose(fp);
 fclose(ft);
 return(1);
}