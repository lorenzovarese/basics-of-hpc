#include "walltime.h"
#include <iostream>
#include <cmath>
#include <cstdio>

#define NUM_ITERATIONS 100
#define N_DEF 100000
#define EPSILON 0.1

using namespace std;

int main(int argc, char *argv[])
{
  int N = N_DEF;
  double time_serial = 0.0, time_start = 0.0;
  double *a, *b;

  if (argc == 2)
    N = stoi(argv[1]);

  a = new double[N];
  b = new double[N];

  for (int i = 0; i < N; i++)
  {
    a[i] = i;
    b[i] = i / 10.0;
  }

  volatile long double alpha = 0; // volatile to prevent compiler optimizations

  time_start = wall_time();
  for (int iterations = 0; iterations < NUM_ITERATIONS; iterations++)
  {
    alpha = 0.0;
    for (int i = 0; i < N; i++)
    {
      alpha += a[i] * b[i];
    }
  }
  time_serial = wall_time() - time_start;
  printf("%f", time_serial); // %f to print double

  delete[] a;
  delete[] b;

  return 0;
}
