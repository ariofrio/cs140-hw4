#include "functions.h"
#include <stdio.h>
#include<stdlib.h>
#include <reducer_opadd.h>

#define COARSENESS 4

// recursively evaluate the inner product
int rec_cilkified(int* a, int* b, unsigned int n)
{
  if(n < COARSENESS) {
    int ret = 0;
    for(int i=0; i<n; i++) {
      ret += a[i] * b[i];
    }
    return ret;
  } else {
    int left, right;
    left = cilk_spawn rec_cilkified(a, b, n/2);
    right = cilk_spawn rec_cilkified(
                        a+n/2, b+n/2, n-n/2);
    return left + right;
  }
}

int loop_cilkified(int* a, int* b, unsigned int n)
{
  int iterations = (n+COARSENESS-1)/COARSENESS;
  int* rets = (int *)malloc(sizeof(int)*iterations);
  cilk_for(int i = 0; i < iterations; i++) {
    int size = n - i*COARSENESS;
    if(size > COARSENESS) size = COARSENESS;
    rets[i] = 0;
    for(int j = 0; j < size; j++) {
      int index = i*COARSENESS + j;
      rets[i] += a[index] * b[index];
    }
  }
  int ret = 0;
  for(int i = 0; i < iterations; i++) {
    ret += rets[i];
  }
  free(rets);
	return ret;
}

int hyperobject_cilkified(int *a,int *b,unsigned int n)
{
  cilk::reducer_opadd<int> ret;
  cilk_for(int i = 0; i < n; i++) {
    ret += a[i] * b[i];
  }
	return ret.get_value();
}

