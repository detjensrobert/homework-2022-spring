#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

// print debugging messages?
#ifndef DEBUG
#define DEBUG false
#endif

// default number of threads
#ifndef NUMT
#define NUMT 2
#endif

// default number of nodes
#ifndef NUMNODES
#define NUMNODES 1000
#endif

#define XMIN -1.
#define XMAX 1.
#define YMIN -1.
#define YMAX 1.

float Height(int, int); // function prototype

int main(int argc, char *argv[]) {

  // set the number of threads to use in parallelizing the for-loop:`
  omp_set_num_threads(NUMT);

  // the area of a single full-sized tile:
  // (not all tiles are full-sized, though)

  const float tile_area = ((XMAX - XMIN) / (float)(NUMNODES - 1)) *
                          ((YMAX - YMIN) / (float)(NUMNODES - 1));

  double time_start = omp_get_wtime();

  // sum up the weighted heights into the variable "volume"
  // using an OpenMP for-loop and a reduction:

  double total_volume = 0;
#pragma omp parallel for collapse(2) default(none) firstprivate(tile_area) reduction(+ : total_volume)
  // The (2) means you are collapsing 2 nested for-loops into one
  // The end effect is exactly like above, but without the mod / divide
  for (int iv = 0; iv < NUMNODES; iv++) {
    for (int iu = 0; iu < NUMNODES; iu++) {

      // double tile_volume = 2 * Height(iu, iv) * tile_area;
      double tile_volume = 2 * Height(iu, iv) * tile_area;

      // adjust volume for edges / corners where half/quarter are cut off
      if (iv == 0 || iv == NUMNODES - 1) {
        tile_volume *= 0.5;
      }
      if (iu == 0 || iu == NUMNODES - 1) {
        tile_volume *= 0.5;
      }

      total_volume += tile_volume;
    }
  }

  double time_end = omp_get_wtime();
  double meganodes_per_second =
      (double)NUMNODES / (time_end - time_start) / 1000000.;

#ifndef QUIET
  fprintf(stderr,
          "%2d threads : %8d nodes ; volume = %lf ; megatrials/sec = %6lf\n",
          NUMT, NUMNODES, total_volume, meganodes_per_second);
#endif
#ifdef CSV
  printf("%d,%d,%lf,%lf\n", NUMT, NUMNODES, total_volume, meganodes_per_second);
#endif
}

// The code to evaluate the height at a given iu and iv is:

const float N = 2.5f;
const float R = 1.2f;

float Height(int iu, int iv) { // iu,iv = 0 .. NUMNODES-1

  float x = -1. + 2. * (float)iu / (float)(NUMNODES - 1); // -1. to +1.
  float y = -1. + 2. * (float)iv / (float)(NUMNODES - 1); // -1. to +1.

  float xn = pow(fabs(x), (double)N);
  float yn = pow(fabs(y), (double)N);
  float rn = pow(fabs(R), (double)N);
  float r = rn - xn - yn;
  if (r <= 0.)
    return 0.;
  float height = pow(r, 1. / (double)N);
  return height;
}
