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

int partition(keytype A[],int p,int r)
{
    int x = A[r];
    int i = p-1;
    for(int j = p;j < r;j++)
    {
        if(A[j] <= x) 
        {
            i++;
            int temp = A[i];
            A[i] = A[j];
            A[j] = temp;
        }
    }
    int temp = A[i+1];
    A[i+1] = A[r];
    A[r] = temp;
    return i+1;
}

void seqSort(keytype A[],int p,int r)
{
    if(p >= r)
        return;
    
    int q = partition(A,p,r);
    seqSort(A,p,q-1);
    seqSort(A,q+1,r);
}

void merge(keytype *T,int p1,int r1,int p2,int r2,keytype A[],int q)
{
    int a = p1;
    int b = p2;
    int c = q;
    while(a <= r1 && b <= r2) {
        if(T[a] < T[b])
            A[c++] = T[a++];
        else
            A[c++] = T[b++];
    }
    while(a <= r1) {
        A[c++] = T[a++];
    }
    while(b <= r2) {
        A[c++] = T[b++];
    }
}

//Binary Search T to find value x 
//Will return index where x=T[index] or 
int binary_search(keytype *T,int p,int r,int x)
{
    int a = p;
    int b = r;
    int c = (a+b)/2;
    while(a < b) {
        if(T[c] == x)
            return c;
        if(T[c] < x)
            a = c + 1;
        else
            b = c;
        c = (a+b)/2;
    }
    if(T[c] > x)
        return c;
    else
        return c+1;
}

void parallel_merge(keytype *A,int p,int q,int r){
	int n = r - p + 1;
    keytype* T = newKeys(n);
    int i;
#pragma omp parallel for shared(A,p,n,T) private(i)
    for(i = 0;i < n;i++)
        *(T + i) = *(A+i+p);
    
    int p1 = 0;
    int r1 = q-p;
    int q1 = (p1+r1)/2;
    int p2 = q-p+1;
    int r2 = r-p;
    int q2 = binary_search(T,p2,r2,*(T+q1));
    int p3 = p;
    int q3 = p3+(q1-p1)+(q2-p2);
    *(A+q3) = *(T+q1);
	#pragma omp parallel sections shared(A,p1,r1,q1,p2,r2,q2,p3,q3)
    	{
	#pragma omp section
    	merge(T,p1,q1-1,p2,q2-1,A,p3);
	#pragma omp section
    	merge(T,q1+1,r1,q2,r2,A,q3+1);
    	}	
    
//    free(T);
}

//p is lowerbound
//r is upperbound
//q is the halfway mark
//level is level of recursion 
void parallel_merge_sort(keytype *A,int p,int r,int level)
{
	//limit the parallelism to only 4 levels of recursion 
	//so there is limited threads 
	if(level>4){
		//seqSort(A,p,r);
		sequentialSort(r-p+1,&A[p]);
	}
    else{
        int q = (p+r)/2;
	#pragma omp parallel sections shared(A,p,r,q)
        {
	#pragma omp section
        parallel_merge_sort(A, p, q,level+1);
	#pragma omp section
        parallel_merge_sort(A, q+1, r,level+1);
        }
        parallel_merge(A,p,q,r);
    }
}


void parallelSort (int length, keytype* A)
{
    parallel_merge_sort(A,0,length-1,0); //use parallelSort() as a wrapper so I can have my own arguements 
}

/* eof */

