
#include <math.h>
#include <mpi.h>
#include <stdio.h>

// which node is in charge?
#define BOSS 0

// files to read and write:
#define BIGSIGNALFILEBIN (char *)"bigsignal.bin"
#define BIGSIGNALFILEASCII (char *)"bigsignal.txt"
#define CSVPLOTFILE (char *)"plot.csv"

// tag to "scatter":
#define TAG_SCATTER 'S'

// tag to "gather":
#define TAG_GATHER 'G'

// how many elements are in the big signal:
#define NUMELEMENTS (8 * 1024 * 1024)

// only do this many shifts, not all NUMELEMENTS of them (this is enough to
// uncover the secret sine wave):
#define MAXSHIFTS 1024

// how many autocorrelation sums to plot:
#define MAXPLOT 256 // an excel limit

// pick which file type to read, BINARY or ASCII (BINARY is much faster to
// read): (pick one, not both)
// #define BINARY
#define ASCII
// nb. this code only supports ascii for simplicity

// print debugging messages?
#define DEBUG 0
#define dbg_printf(format, ...)                                                \
  if (DEBUG)                                                                   \
  fprintf(stderr, (format), ##__VA_ARGS__)

// globals:
float *all_sums;     // the overall MAXSHIFTS autocorrelation array
float *all_signals;  // the overall NUMELEMENTS-big signal data
int num_cpus;        // total # of cpus involved
float *indiv_sums;   // per-processor autocorrelation sums
float *indiv_signal; // per-processor local array to hold the sub-signal
int indiv_size;      // per-processor local array size

// function prototype:
void do_local_autocorrelation(int);

int main(int argc, char *argv[]) {
  MPI_Status status;

  MPI_Init(&argc, &argv);

  MPI_Comm_size(MPI_COMM_WORLD, &num_cpus);
  int me; // which one am I?
  MPI_Comm_rank(MPI_COMM_WORLD, &me);

  // decide how much data to send to each processor:
  indiv_size = NUMELEMENTS / num_cpus; // assuming it comes out evenly

  // local arrays:
  indiv_signal =
      new float[indiv_size + MAXSHIFTS]; // per-processor local signal
  indiv_sums = new float[MAXSHIFTS]; // per-processor local sums of the products

  // read the all_signals array:
  // if we are the the big-data-owner
  if (me == BOSS) {
    all_signals = new float[NUMELEMENTS + MAXSHIFTS];
    // + MAXSHIFTS is so we can duplicate part of the array

    FILE *fp = fopen(BIGSIGNALFILEASCII, "r");
    if (fp == NULL) {
      fprintf(stderr, "Cannot open data file '%s'\n", BIGSIGNALFILEASCII);
      return -1;
    }

    for (int i = 0; i < NUMELEMENTS; i++) {
      float f;
      fscanf(fp, "%f", &f);
      all_signals[i] = f;
    }

    // duplicate part of the array:

    for (int i = 0; i < MAXSHIFTS; i++) {
      all_signals[NUMELEMENTS + i] = all_signals[i];
    }
  }

  // create the array to hold all the sums:

  if (me == BOSS) {
    all_sums = new float[MAXSHIFTS]; // where all the sums will go
  }

  // start the timer:

  double time0 = MPI_Wtime();

  // have the BOSS send to itself (really not a "send", just a copy):

  if (me == BOSS) {
    for (int i = 0; i < indiv_size + MAXSHIFTS; i++) {
      indiv_signal[i] = all_signals[BOSS * indiv_size + i];
    }
  }

  // have the BOSS send to everyone else ('s'catter):

  if (me == BOSS) {
    for (int dst = 0; dst < num_cpus; dst++) {
      if (dst == BOSS)
        continue;

      dbg_printf("boss scattering to rank %i\n", dst);
      MPI_Send(&all_signals[dst * indiv_size], indiv_size + MAXSHIFTS,
               MPI_FLOAT, dst, 's', MPI_COMM_WORLD);
    }
  } else {
    dbg_printf("rank %i receiving scatter\n", me);
    MPI_Recv(indiv_signal, indiv_size + MAXSHIFTS, MPI_FLOAT, BOSS, 's',
             MPI_COMM_WORLD, &status);
  }

  // each processor does its own autocorrelation:

  do_local_autocorrelation(me);

  // each processor sends its sums back to the BOSS:

  if (me == BOSS) {
    for (int s = 0; s < MAXSHIFTS; s++) {
      all_sums[s] =
          indiv_sums[s]; // start the overall sums with the BOSS's sums
    }
  } else {
    dbg_printf("rank %i sending gather\n", me);
    MPI_Send(indiv_sums, MAXSHIFTS, MPI_FLOAT, BOSS, 'g', MPI_COMM_WORLD);
  }

  // receive the sums and add them into the overall sums:

  if (me == BOSS) {
    float tmpSums[MAXSHIFTS];
    for (int src = 0; src < num_cpus; src++) {
      if (src == BOSS)
        continue;

      dbg_printf("boss gathering from rank %i\n", src);
      MPI_Recv(tmpSums, MAXSHIFTS, MPI_FLOAT, src, 'g', MPI_COMM_WORLD,
               &status);
      for (int s = 0; s < MAXSHIFTS; s++)
        all_sums[s] += tmpSums[s];
    }
  }

  // stop the timer:

  double time1 = MPI_Wtime();

  // print the performance:

  if (me == BOSS) {
    double seconds = time1 - time0;
    double performance = (double)MAXSHIFTS * (double)NUMELEMENTS / seconds /
                         1000000.; // mega-elements computed per second
#ifndef CSV
    fprintf(stderr,
            "%3d processors, %10d elements, %9.2lf mega-autocorrelations "
            "computed per second\n",
            num_cpus, NUMELEMENTS, performance);
#else
    printf("%d,%d,%lf\n", num_cpus, NUMELEMENTS, performance);
#endif
  }

  // write the file to be plotted to look for the secret sine wave:

  if (me == BOSS) {
    FILE *fp = fopen(CSVPLOTFILE, "w");
    if (fp == NULL) {
      fprintf(stderr, "Cannot write to plot file '%s'\n", CSVPLOTFILE);
    } else {
      fprintf(fp, "sumindex,value\n");
      for (int s = 1; s < MAXPLOT; s++) { // BigSums[0] is huge -- don't use it
        fprintf(fp, "%6d,%10.2f\n", s, all_sums[s]);
      }
      fclose(fp);
    }
  }

  // all done:

  MPI_Finalize();
  return 0;
}

// read from the per-processor signal array, write into the local sums array:

void do_local_autocorrelation(int me) {
  MPI_Status status;

  dbg_printf("Node %3d entered local_autocorrelation()\n", me);

  for (int s = 0; s < MAXSHIFTS; s++) {
    float sum = 0.;
    for (int i = 0; i < indiv_size; i++) {
      sum += indiv_signal[i] * indiv_signal[i + s];
    }
    indiv_sums[s] = sum;
  }
}
