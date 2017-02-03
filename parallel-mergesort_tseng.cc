/**
 *  \file parallel-mergesort.cc
 *
 *  \brief Implement your parallel mergesort in this file.
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

#include "sort.hh"

void
parallelSort (int N, keytype* A)
{
  /* Lucky you, you get to start from scratch */
	//Sizing/Spliting issues 
	#pragma omp parallel{
		keytype *A1 = newCopy(N/2, A);
		keytype *A2 = newCopy(N/2, (A + (N/2 + 1)));
		keytype *TempArray = newCopy(N,A);
		if (N > 2){
			#pragma omp task
			parallelSort(N/2, A1);
			parallelSort(N/2, A2);
			#pragma omp taskwait 
		}
		#pragma omp for private(i)
		for(i=0;i<N/2;i++){
			#pragma omp task
			parallelMerge(i,A1,A2,TempArray);
			parallelMerge(i,A2,A1,TempArray);
			#pragma omp taskwait 
		}
		memcpy(A,TempArray,N*sizeof(keytype));
	}
		
}

keytype *parallelMerge(keytype *A, keytype *B){
	#pragma omp parallel
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
	
	#pragma omp task
	*E1 = parallelMerge(C1, D1);
	*E2 = parllelMerge(C2, D2);
	#pragma omp taskwait
	*t = newKeys(Na + Nb);
	N1 = sizeof(E1)/sizeof(keytype);
	N2 = sizeof(E2)/sizeof(keytype);
	memcpy(t, E1, N1*sizeof(keytype));
	memcpy(t + N1, E2, N2*sizeof(keytype));
	return t; 
	 
}

int binSearch(keytype *B, int v){
	//TODO Have to sort first (I think)
	int min = 0;
	int max = sizeof(B)/sizeof(keytype);
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
