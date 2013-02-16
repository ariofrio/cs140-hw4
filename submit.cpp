#include "functions.h"
#include <stdio.h>
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
    int left = cilk_spawn rec_cilkified(a, b, n/2);
    int right = cilk_spawn rec_cilkified(
                        a+n/2, b+n/2, n-n/2);
    return left + right;
  }
}

int loop_cilkified(int* a, int* b, unsigned int n)
{
  cilk::reducer_opadd<int> ret;
  cilk_for(int i = 0;
      i < (n+COARSENESS-1)/COARSENESS; i++) {
    int size = n - i*COARSENESS;
    if(size > COARSENESS) size = COARSENESS;
    for(int j = 0; j < size; j++) {
      int index = i*COARSENESS + j;
      ret += a[index] * b[index];
    }
  }
	return ret.get_value();
}

int hyperobject_cilkified(int *a,int *b,unsigned int n)
{
  cilk::reducer_opadd<int> ret;
  cilk_for(int i = 0; i < n; i++) {
    ret += a[i] * b[i];
  }
	return ret.get_value();
}

