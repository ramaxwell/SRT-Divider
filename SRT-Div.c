/*
 Robert A. Maxwell
 Computer Arithmetic - Project #3
 Dr. Wei-ming Lin
 University of Texas at San Antonio

 N-bit SRT Divider with shift-over 0's and 1's
	for random bits starting at n = 16, 18, 20,...32

https://github.com/ramaxwell/SRT-Divider.git

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
//#include "CCAdder.h"

#define MAX_BITS 64					//starts at 16 and ends at 32
#define MAX_RUNS 10000
#define ARRAY_SIZE MAX_BITS
#define NUMER_ARRAY_SIZE ARRAY_SIZE*2 +1
#define NUM_SETS 29					//number of bins for average

//******************************
//Print Binary Array
//******************************

void printOperand( int *oper, int max_bits ){

	int c;

	for ( c = 0; c < max_bits; c++ ){

		printf("%u", oper[c]);

	}
	printf("\n");
}
//******************************
//Generate random binary numbers
// for denominator
//******************************

void generateDenom(unsigned int *d, int num_bits){

	int pos;
	unsigned int val;

	d[0] = 1;

	for(pos = 1; pos < num_bits; pos++){
		val = rand() %2;
	
		if(val){
			d[pos] = val;
		}
	}

}
//******************************
//Generate random binary numbers
// for denominator
//******************************

void generateNumer( unsigned int *op , int num_bits ){

	int pos;
	unsigned int val;

	for(pos = 0; pos < num_bits; pos++){
		val = rand() % 2;
		
		if(val){
			op[pos] = val;			
		}
	}
}
//******************************
// Load Registers with input vectors
//******************************
void loadRegister( unsigned int *reg, unsigned int *in, int num_bits){

	int pos;

	reg[0] = 0;

	for(pos = 1; pos < num_bits; pos++){

		reg[pos] = in[pos-1];

	}

}
//*********************************************
//Insert product into upper half of numerator array
//*********************************************
void insertUpperNumer(int *n_reg, int num_bits, int * op /*, int c*/){

	int m = 0;

	for(m = 0; m < num_bits; m++){

		n_reg[m] = op[m];

	}
}

//************************************************************
//retrieve upper portion of numerator for input into adder
//************************************************************
void getUpperNumer(int *n_reg, int *NumerReg, int num_bits){

	int m;

	for(m = 0; m < num_bits; m++){

		NumerReg[m] = n_reg[m];

	}
}
//******************************
//for copying array1 into array2
//******************************
void arrayCopy( int * arr1, int * arr2, int size){
	
	int m;

	for(m = 0; m < size; m++){

		arr2[m] = arr1[m];

	}
}
//******************************
//Register shift Left
//******************************
void shiftLeft( int *reg, int in_bit, int arraySize){

	int m;

	for(m = 0; m < arraySize-1; m++){

		reg[m] = reg[m+1];

	} 

	reg[arraySize-1] = in_bit;
}

//******************************
//compliment all bits and add 1 at the end
//******************************
void comp2(int * val, int num_bits){

	int m;

	for(m = 0; m < num_bits; m++){			//loop for complimenting all bits

		if(val[m])
			val[m] = 0;
		else val[m] = 1;

	}

	for(m = num_bits-1; m >= 0; m--){
		
		if(val[m]){
			val[m] = 0;
		}else{
			val[m] = 1;
			break;
		}	
	}
}
//******************************
// Convert Binary to Decimal
//for checking product of multiplication
//******************************
double bin2dec(int *bin, int num_bits){

	int m;
	int cnt = 0;
	double dec = 0;	

	for(m = num_bits-1; m >=0; m--){
		
		if(bin[m] == 1)
			dec += pow(2,cnt);

		cnt++;	
	}

return dec;
}

//******************************
// Ripple Carry Adder Process
//******************************
unsigned int evalRCA(int *in1, int *in2, int *out, int numBits){

	int m;
	int cin = 0;
	unsigned int cout = 0;

	for(m = numBits-1; m >= 0; m--){

		out[m] = in1[m] ^ in2[m] ^ cin;		
		cout = (in1[m] & in2[m]) | (in1[m] & cin) | (in2[m] & cin);
		cin = cout;

	}

return cout;
}

//******************************
// Main Function
//******************************
int main(void){

	int i, j;
	int delay;
	int m = 0;

	double effAvg[NUM_SETS];
	double effSum = 0.0;
						
	int A[ARRAY_SIZE+1];			//input to the adder	
	int B[ARRAY_SIZE+1];			//		
	int S[ARRAY_SIZE+1];			//

	int D_in[ARRAY_SIZE];
	int Numer_in[NUMER_ARRAY_SIZE-1];

	int D[ARRAY_SIZE+1];
	int Numer[NUMER_ARRAY_SIZE];

	for(i = 0; i < NUM_SETS; i++){
		effAvg[i] = 0;
	}

	srand(time(NULL));			//seed the random number generator

	for ( i = 8; i <= MAX_BITS; i+=2 ) {		//For each set of bits

		printf("Number of Bits: %i\n", i);
		effSum = 0;

		for(j = 0; j < MAX_RUNS; j++){		//For each number of runs

			int k;
			int l;
			int cycles = 0;
			int addCnt = 0;
			int bitCnt = 0;
			int q_bit = 0;

			int quotient[ARRAY_SIZE+1];
			int remainder[ARRAY_SIZE];
		

			for(l = 0; l < ARRAY_SIZE; l++){			//Initialize storage for 
				D_in[l] = 0;							//input vectors
			}											//
			for(l = 0; l < NUMER_ARRAY_SIZE-1; l++){	//	
				Numer_in[l] = 0;						//
			}											//

			for(k = 0; k < ARRAY_SIZE+1; k++){			//initialize storage arrays
				A[k] = 0;								//						
				B[k] = 0;								//
				S[k] = 0;								//
														//
				D[k] = 0;								//
			}											//
			for(k = 0; k < NUMER_ARRAY_SIZE; k++){		//
				Numer[k] = 0;							//
			}											//


			generateDenom(D_in, i);					//generate operands
			generateNumer(Numer_in, i*2);			// Numer and Denom

			loadRegister( Numer, Numer_in, i*2+1);
			loadRegister( D, D_in, i+1);

			for(l = 0; l <= i; l++){

				cycles++;

				if(Numer[0] != Numer[1]){				//if first two bits aren't the same
														//Do Addition stuff	

					addCnt++;							//increment # additions

					getUpperNumer(Numer, A, i+1);		//Save into A for adding
					arrayCopy(D, B, i+1);				//Save into B for adding		
		
					if(!Numer[0]){			//if MSB == 0, ADD -D to n+1 bits of numerator
						comp2(B, i+1);

					}

					evalRCA(A, B, S, i+1);

					insertUpperNumer(Numer, i+1, S);

					if(Numer[0]){			//if MSB==1 quotient gets a 0
						q_bit = 0;
					}else{					//else MSB==0, quotient gets a 1
						q_bit = 1;
					}
					
				}else{										//else numer[0]==numer[1]
															//can shift now
					if(Numer[0]){
						q_bit = 1;
					}else{
						q_bit = 0;
					}

				}

				shiftLeft(Numer, q_bit, i*2+1);				//always shift
				//delay += 2
				bitCnt++;									//increment # bits processed

			}//END "For" number of bits	

		
		if(Numer[0]){						//extra clock cycle(s) if remainder is neg
			getUpperNumer(Numer, A, i);		
			evalRCA(A, &D[1], S, i);
		}

		for(l = 0; l < i; l++){				//Save remainder from numerator
			remainder[l] = Numer[l];		// register to an explicit array
		}

		k = 0;
		for(l = i; l < i*2 +1; l++){		//Save quotient from numerator
			quotient[k] = Numer[l];			// register to an explicit array
			k++;
		}

		if(!addCnt)							//handle "divide by zero" exception
			effSum += (float) bitCnt;		//for when addCnt = 0
		else								//
			effSum += (float) bitCnt/addCnt;//

		}//END "FOR" LOOP (Each sim run)

		effAvg[m] = effSum/MAX_RUNS;

		printf("Efficiency Average (bits/AddSub): %.4f\n", effAvg[m]);
		m++;

	}//END "FOR" LOOP (Each Set of number of bits)


	FILE *fp;									//save processing efficiency avg to file 
	fp=fopen("run.txt", "w");				//for input into spreadsheet for graphing
	fprintf(fp, "#-bits\tAvg Efficiency\n");

	for(i = 0; i < NUM_SETS; i++){
		fprintf(fp, "%i\t\t%.4lf\n", (2*i+8), effAvg[i]);
	}

	fclose(fp);

return 0;
}
