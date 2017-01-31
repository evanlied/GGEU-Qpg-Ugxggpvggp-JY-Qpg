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
parallelSort (int length, keytype* A)
{
    int B[length];
    printf("\nparrallel Sort begins\n");

    //split A into 2 between the halves of B
    split(0,N/2);
    split(N/2+1,N);
}

/* eof */
