/**
 *  \file parallel-mergesort.cc
 *
 *  \brief Implement your parallel mergesort in this file.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <string.h>

#include "sort.hh"

void parallelMerge(int workIndex, keytype* workArray, keytype* compareArray, keytype* tempArray, int arraySize);
int BinarySearch(int workingNumber, keytype* compareArray, int arraySize);
int binSearch(keytype *B, int v);

void
parallelSort (int N, keytype* A)
{
  /* Lucky you, you get to start from scratch */
	//Sizing/Spliting issues
	int i;
	keytype *A1 = newCopy(N/2, A);
	keytype *A2 = newCopy(N/2, (A + (N/2 + 1)));
	keytype *TempArray = newCopy(N,A);
	#pragma omp parallel shared(A1,A2,TempArray) private(i)
	{
		if (N > 2){
			#pragma omp task
			parallelSort(N/2, A1);
			parallelSort(N/2, A2);
			#pragma omp taskwait 
		}
		#pragma omp for 
		for(i=0;i<N/2;i++){
			//printf("Merging now at N=%d\n",N/2);
			#pragma omp task
			parallelMerge(i,A1,A2,TempArray,N/2);
			parallelMerge(i,A2,A1,TempArray,N/2);
			#pragma omp taskwait 
		}
		memcpy(A,TempArray,N*sizeof(keytype));
	}
		
}

/*Tony's Merge*/
void parallelMerge(int workIndex, keytype* workArray, keytype* compareArray, keytype* tempArray, int arraySize){
	printf("Pmerging now for N=%d\n",arraySize);
	int finalPosition = BinarySearch(workIndex,compareArray,arraySize); //BinarySearch(*(workArray+workIndex),compareArray,arraySize); //find where the number fits in the compared Array
	finalPosition += workIndex+1;										   //find final position by adding where it fits in its array with where it fits in 
																		   //the compared Array
	*(tempArray+finalPosition) = *(workArray+workIndex);
}
/*Tony's BinarySearch*/
//https://mathbits.com/MathBits/CompSci/Arrays/Binary.htm//
int BinarySearch(int workingNumber,keytype* compareArray,int arraySize){
    int workingIndex=arraySize/2;
	int lowerBound = 0;
	int upperBound = arraySize;
	//sequentialSort(arraySize, compareArray);
	//printf("Binary Searching for N=%d Index=%d",arraySize,workingIndex);
	while((*(compareArray+workingIndex)!=workingNumber)){
		if(*(compareArray+workingIndex)>workingNumber){
			upperBound = workingIndex - 1;
		}else{
			lowerBound = workingIndex + 1;
		}
		
		if(lowerBound<=upperBound){
			workingIndex = (lowerBound+upperBound)/2;
		}else{//the number is not found in the compared Array, need to return the final position which is either +-1 of workingIndex 
			if(*(compareArray+workingIndex)>workingNumber){
				return workingIndex;
			}else{
				return workingIndex+1;
			}
		}
	}
	//if we find the exact same number in the compared array, for loop exits
	if(lowerBound<=upperBound){ 
	    return workingIndex;
	}				
	//+1 because of how the final position is calculated 
}

/*Micheal's Merge //
keytype *parallelMerge(keytype *A, keytype *B){
	keytype *E1;
	keytype *E2;
	keytype *t;
	int N1;
	int N2;
	int Na = sizeof(A)/sizeof(keytype);
	int Nb = sizeof(B)/sizeof(keytype);
	int v = A[Na/2];
	int k = binSearch(B, v);
	
	keytype *C1 = newCopy(v, A);
	keytype *C2 = newCopy(Na/2, (A + v + 1));
	keytype *D1 = newCopy(k, B);
	keytype *D2 = newCopy(Nb/2, (B + k + 1));
	
	*E1 = parallelMerge(C1, D1);
	*E2 = parllelMerge(C2, D2);
	*t = newKeys(Na + Nb);
	N1 = sizeof(E1)/sizeof(keytype);
	N2 = sizeof(E2)/sizeof(keytype);
	memcpy(t, E1, N1*sizeof(keytype));
	memcpy(t + N1, E2, N2*sizeof(keytype));
	return t; 
	 
}
*/

int binSearch(keytype *B, int v){
	//TODO Have to sort first (I think)
	int min = 0;
	int max = sizeof(B)/sizeof(keytype);
	sequentialSort(max, B);
	int guess;
	while(1){
		if(min > max){
			break;
		}
		else if(B[guess] == v){
			return guess;
		}
		else if(B[guess] > v){
			max = guess - 1;
		}
		else{
			min = guess + 1;
		}
	}
	return 0;
	
}


/* eof */
