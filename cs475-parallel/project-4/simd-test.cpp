#include <ctime>
#include <math.h>
#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/resource.h>
#include <sys/time.h>

// SSE stands for Streaming SIMD Extensions

#define SSE_WIDTH 4
#define ALIGNED __attribute__((aligned(16)))

#define NUMTRIES 100

#ifndef ARRAYSIZE
#define ARRAYSIZE 1024 * 1024
#endif

ALIGNED float A[ARRAYSIZE];
ALIGNED float B[ARRAYSIZE];
ALIGNED float C[ARRAYSIZE];

void simd_mult(float *, float *, float *, int);
void nosimd_mult(float *, float *, float *, int);
float simd_mulsum(float *, float *, int);
float nosimd_mulsum(float *, float *, int);

#ifdef CSV
void logprint(const char *l, int v) { printf("%d,", v); }
void logprint(const char *l, double v) { printf("%.2lf,", v); }
#else
void logprint(const char *l, int v) { printf("%s: %d\t", l, v); }
void logprint(const char *l, double v) { printf("%s: %.2lf\t", l, v); }
#endif

int main(int argc, char *argv[]) {
  for (int i = 0; i < ARRAYSIZE; i++) {
    A[i] = sqrtf((float)(i + 1));
    B[i] = sqrtf((float)(i + 1));
  }

  // fprintf(stderr, "%12d\t", ARRAYSIZE);
  logprint("arraysize", ARRAYSIZE);

  double maxPerformance = 0.;
  for (int t = 0; t < NUMTRIES; t++) {
    double time0 = omp_get_wtime();
    nosimd_mult(A, B, C, ARRAYSIZE);
    double time1 = omp_get_wtime();
    double perf = (double)ARRAYSIZE / (time1 - time0);
    if (perf > maxPerformance)
      maxPerformance = perf;
  }
  double megaMults = maxPerformance / 1000000.;
  // fprintf(stderr, "N %10.2lf\t", megaMults);
  logprint("mul nosimd", megaMults);
  double mmn = megaMults;

  maxPerformance = 0.;
  for (int t = 0; t < NUMTRIES; t++) {
    double time0 = omp_get_wtime();
    simd_mult(A, B, C, ARRAYSIZE);
    double time1 = omp_get_wtime();
    double perf = (double)ARRAYSIZE / (time1 - time0);
    if (perf > maxPerformance)
      maxPerformance = perf;
  }
  megaMults = maxPerformance / 1000000.;
  // fprintf(stderr, "S %10.2lf\t", megaMults);
  logprint("with simd", megaMults);
  double mms = megaMults;
  double speedup = mms / mmn;
  // fprintf(stderr, "(%6.2lf)\t", speedup);
  logprint("speedup", speedup);

  maxPerformance = 0.;
  float sumn, sums;
  for (int t = 0; t < NUMTRIES; t++) {
    double time0 = omp_get_wtime();
    sumn = nosimd_mulsum(A, B, ARRAYSIZE);
    double time1 = omp_get_wtime();
    double perf = (double)ARRAYSIZE / (time1 - time0);
    if (perf > maxPerformance)
      maxPerformance = perf;
  }
  double megaMultAdds = maxPerformance / 1000000.;
  // fprintf(stderr, "N %10.2lf\t", megaMultAdds);
  logprint("muladd nos", megaMultAdds);
  mmn = megaMultAdds;

  maxPerformance = 0.;
  for (int t = 0; t < NUMTRIES; t++) {
    double time0 = omp_get_wtime();
    sums = simd_mulsum(A, B, ARRAYSIZE);
    double time1 = omp_get_wtime();
    double perf = (double)ARRAYSIZE / (time1 - time0);
    if (perf > maxPerformance)
      maxPerformance = perf;
  }
  megaMultAdds = maxPerformance / 1000000.;
  // fprintf(stderr, "S %10.2lf\t", megaMultAdds);
  logprint("with simd", megaMultAdds);
  mms = megaMultAdds;
  speedup = mms / mmn;
  // fprintf(stderr, "(%6.2lf)\n", speedup);
  logprint("speedup", speedup);
  // fprintf( stderr, "[ %8.1f , %8.1f , %8.1f ]\n", C[ARRAYSIZE-1], sumn, sums
  // );

  printf("\n");

  return 0;
}

void nosimd_mult(float *a, float *b, float *c, int len) {
  for (int i = 0; i < len; i++) {
    c[i] = a[i] * b[i];
  }
}

float nosimd_mulsum(float *a, float *b, int len) {
  float sum = 0;

  for (int i = 0; i < len; i++) {
    sum += a[i] * b[i];
  }

  return sum;
}

void simd_mult(float *a, float *b, float *c, int len) {
  int limit = (len / SSE_WIDTH) * SSE_WIDTH;
  asm(".att_syntax\n\t"
      "movq    -24(%rbp), %r8\n\t " // a
      "movq    -32(%rbp), %rcx\n\t" // b
      "movq    -40(%rbp), %rdx\n\t" // c
  );

  for (int i = 0; i < limit; i += SSE_WIDTH) {
    asm(".att_syntax\n\t"
        "movups	(%r8), %xmm0\n\t"  // load the first sse register
        "movups	(%rcx), %xmm1\n\t" // load the second sse register
        "mulps	%xmm1, %xmm0\n\t"  // do the multiply
        "movups	%xmm0, (%rdx)\n\t" // store the result
        "addq $16, %r8\n\t"
        "addq $16, %rcx\n\t"
        "addq $16, %rdx\n\t");
  }

  for (int i = limit; i < len; i++) {
    c[i] = a[i] * b[i];
  }
}

float simd_mulsum(float *a, float *b, int len) {
  float sum[4] = {0., 0., 0., 0.};
  int limit = (len / SSE_WIDTH) * SSE_WIDTH;

  asm(".att_syntax\n\t"
      "movq    -40(%rbp), %r8\n\t"  // a
      "movq    -48(%rbp), %rcx\n\t" // b
      "leaq    -32(%rbp), %rdx\n\t" // &sum[0]
      "movups	 (%rdx), %xmm2\n\t" // 4 copies of 0. in xmm2
  );

  for (int i = 0; i < limit; i += SSE_WIDTH) {
    asm(".att_syntax\n\t"
        "movups	(%r8), %xmm0\n\t"  // load the first sse register
        "movups	(%rcx), %xmm1\n\t" // load the second sse register
        "mulps	%xmm1, %xmm0\n\t"  // do the multiply
        "addps	%xmm0, %xmm2\n\t"  // do the add
        "addq $16, %r8\n\t"
        "addq $16, %rcx\n\t");
  }

  asm(".att_syntax\n\t"
      "movups	 %xmm2, (%rdx)\n\t" // copy the sums back to sum[ ]
  );

  for (int i = limit; i < len; i++) {
    sum[0] += a[i] * b[i];
  }

  return sum[0] + sum[1] + sum[2] + sum[3];
}
