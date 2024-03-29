/******************************************************************************
 * FILE: omp_bug1fix.c
 * DESCRIPTION:
 *   This example attempts to show use of the parallel for construct.  However
 *   it will generate errors at compile time.  Try to determine what is causing
 *   the error.
 ******************************************************************************/
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#define N 50
#define CHUNKSIZE 5

int main(int argc, char *argv[]) {
  int i, chunk, tid;
  float a[N], b[N], c[N];

  /* Some initializations */
  for (i = 0; i < N; i++)
    a[i] = b[i] = i * 1.0;
  chunk = CHUNKSIZE;

/* NOTE
 * When you use #pragma omp parallel for in OpenMP
 * the tid = omp_get_thread_num(); line should be inside the loop body
 * This allows each thread to determine its own thread ID (tid) 
 * while executing the loop iterations in parallel
*/
#pragma omp parallel for shared(a, b, c, chunk) private(i, tid)                \
    schedule(static, chunk)
  {
    tid = omp_get_thread_num();
    for (i = 0; i < N; i++) {
      c[i] = a[i] + b[i];
      printf("tid= %d i= %d c[i]= %f\n", tid, i, c[i]);
    }
  } /* end of parallel for construct */
}
