/*
    Please include compiler name below
    (you may also include any other modules you would like to be loaded)

COMPILER= gcc

    Please include All compiler flags and libraries as you want them run.
    You can simply copy this over from the Makefile's first few lines

CC = cc
OPT = -O3
CFLAGS = -Wall -std=gnu99 $(OPT)
MKLROOT = /opt/intel/composer_xe_2013.1.117/mkl
LDLIBS = -lrt -Wl,--start-group $(MKLROOT)/lib/intel64/libmkl_intel_lp64.a
$(MKLROOT)/lib/intel64/libmkl_sequential.a
$(MKLROOT)/lib/intel64/libmkl_core.a -Wl,--end-group -lpthread -lm

*/

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

/*
Stride calculation:
L1 cache = 128 KB
size of 1 double = 8 bytes
M = 128 KB/(8 bytes) = 16 * 10^3
s = sqrt(M/3)
s <= 73.02
s_max = 73
*/
#define STRIDE 24
#define MIN(a, b) (((a) < (b)) ? (a) : (b))

const char *dgemm_desc = "Naive, three-loop dgemm.";

/* This routine performs a dgemm operation
 *  C := C + A * B
 * where A, B, and C are lda-by-lda matrices stored in column-major format.
 * On exit, A and B maintain their input values. */
void square_dgemm(int n, double *A, double *B, double *C)
{
  int block_size = STRIDE;
  double C_temp[block_size * block_size];

  // Outer loop on rows of C matrix
  for (int i = 0; i < n; i += block_size)
  {
    int i_next_block = MIN(i + block_size, n);

    // Outer loop on columns of C matrix
    for (int j = 0; j < n; j += block_size)
    {
      int j_next_block = MIN(j + block_size, n);

      // Clear C_temp
      for (int ci = 0; ci < block_size * block_size; ci++)
      {
        C_temp[ci] = 0.0;
      }

      // Outer loop for dot product
      for (int k = 0; k < n; k += block_size)
      {
        int k_next_block = MIN(k + block_size, n);

        // Naive matrix multiplication (using only fast memory)
        for (int i_block = i, c_row_index = 0; i_block < i_next_block; i_block++, c_row_index++)
        {
          for (int j_block = j, c_col_index = 0; j_block < j_next_block; j_block++, c_col_index++)
          {
            double temp = 0.0;

            for (int k_block = k; k_block < k_next_block; k_block++)
            {
              temp += A[k_block * n + i_block] * B[k_block + j_block * n];
            }

            C_temp[c_row_index + c_col_index * block_size] += temp;
          }
        }
      }

      // Store C_temp in C
      for (int col_index = j, block_col_index = 0; col_index < j_next_block; col_index++, block_col_index++)
      {
        // Memcpy one column in each iteration
        memcpy(C + col_index * n + i, C_temp + block_col_index * block_size, (i_next_block - i) * sizeof(double));
      }
    }
  }
}
