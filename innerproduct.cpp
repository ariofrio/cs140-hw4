/*
Homework 4 : Cilkified Inner Products.
Team member 1 : Jane Doe
Team member 2 : John Doe
*/

#include<cilk.h>
#include<cilkview.h>
#include<iostream>
#include<string.h>
#include<stdio.h>
#include<stdlib.h>
#include<numeric>

#include "functions.h"

// Function declarations
void fill_arrays(int *a, int *b, int n);

using namespace std;

// String to display results
char result[][15] = {"matched","did not match"};

cilk::cilkview cv;

int cilk_main(int argc, char **argv)
{
	unsigned int n;

	// Size calculation
	n = (unsigned int)atoi(argv[1]);
	
	// Array allocation	
	int *a,*b;
	a = (int *)malloc(sizeof(int)*n);
	b = (int *)malloc(sizeof(int)*n);

	int loop_result, rec_result, hyper_result, ref_result; 
	
	// Initialize arrays
	fill_arrays(a,b,n);
	
  // Compute inner product for each method
  int iterations = 100000000/n + 1;
  cv.start();
  for(int i=0; i<iterations; i++) {
    rec_result = rec_cilkified(a,b,n);
  }
  cv.stop();
  printf("recursive: %.2f us\n",
      cv.accumulated_milliseconds()*1000.0/iterations);
  cv.reset();

  cv.start();
  for(int i=0; i<iterations; i++) {
    loop_result = loop_cilkified(a,b,n);
  }
  cv.stop();
  printf("loop: %.2f us\n",
      cv.accumulated_milliseconds()*1000.0/iterations);
  cv.reset();

  cv.start();
  for(int i=0; i<iterations; i++) {
    hyper_result = hyperobject_cilkified(a,b,n);
  }
  cv.stop();
  printf("hyperobject: %.2f us\n",
      cv.accumulated_milliseconds()*1000.0/iterations);
  cv.reset();
	
	// Compute inner product using library function	
	ref_result = std::inner_product(a,a+n,b,0);

	printf("Result from standard library function : %d\n",ref_result);
	printf("Recursive result %s\n",result[rec_result != ref_result]);
	printf("Loop result %s\n",result[loop_result != ref_result]);
	printf("Hyperobject result %s\n",result[hyper_result != ref_result]);

	return 0;

}

void fill_arrays(int *a, int *b, int n)
{
	int i;
	for(i = 0 ; i < n; i++)
	{
		a[i] = i;	
		b[i] = 10*n+i;
	}
}

