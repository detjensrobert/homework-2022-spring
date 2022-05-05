#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <omp.h>
#include <stdlib.h>
#include <time.h>

#define DEBUG 0
#define dbg_printf(format, ...)                                                \
  if (DEBUG) {                                                                 \
    fprintf(stderr, (format), ##__VA_ARGS__);                                  \
  }

// === GLOBAL SYSTEM STATE ===

int current_year;  // 2022 - 2027
int current_month; // 0 - 11

float current_precip;  // inches of rain per month
float current_temp;    // temperature this month
float current_gheight; // grain height in inches
int current_ndeer;     // number of deer in the current population
int current_burn;    // whether all the grain is burning

// === NEEDED PARAMETERS ===

const float GRAIN_GROWS_PER_MONTH = 15.0;
const float ONE_DEER_EATS_PER_MONTH = 1.0;

const float AVG_PRECIP_PER_MONTH = 7.0; // average
const float AMP_PRECIP_PER_MONTH = 6.0; // plus or minus
const float RANDOM_PRECIP = 2.0;        // plus or minus noise

const float AVG_TEMP = 60.0;    // average
const float AMP_TEMP = 20.0;    // plus or minus
const float RANDOM_TEMP = 10.0; // plus or minus noise

const float MIDTEMP = 40.0;
const float MIDPRECIP = 10.0;

const float BURN_CHANCE = 0.1; // % chance for a wildfire to burn all the grain

unsigned int seed = time(NULL);

// helper funcs
float rand_float(unsigned int *seedp, float low, float high);
float SQR(float x);

// threads
void watcher();
void grain();
void deer();
void california_wildfires();

int main() {
  omp_set_num_threads(4); // same as # of sections

  // starting date and time:
  current_month = 0;
  current_year = 2022;

  // starting state (feel free to change this if you want):
  current_ndeer = 1;
  current_gheight = 1.;
  current_temp = 40;

// csv header
#ifdef CSV
  printf("year,month,temp,precip,deer,grain,burned\n");
#endif

#pragma omp parallel sections
  {
#pragma omp section
    { watcher(); }

#pragma omp section
    { deer(); }

#pragma omp section
    { grain(); }

#pragma omp section
    { california_wildfires(); }
  }
  // implied barrier -- all functions must return in order
  // to allow any of them to get past here
}

void watcher() {
  while (current_year < 2028) {
    // nop

    dbg_printf("  w> calc nop\n");

// Done Computing
#pragma omp barrier

    // nop

    dbg_printf("  w> assign nop\n");

// Done Assigning
#pragma omp barrier

// print results of this month
#ifndef CSV
    printf("%2i-%i:  %2.f temp, %2.f precip, %2i deer, "
           "%2.f grain, %i burned \n",
           current_year, current_month, current_temp, current_precip,
           current_ndeer, current_gheight, current_burn);
#else
    printf("%i,%i,%f,%f,%i,%f,%i\n", current_year, current_month, current_temp,
           current_precip, current_ndeer, current_gheight, current_burn);
#endif

    // increment time
    current_month = (current_month + 1) % 12;
    current_year += (current_month == 0);

    // compute new temp + rain
    float ang = (30. * (float)current_month + 15.) * (M_PI / 180.);

    float temp = AVG_TEMP - AMP_TEMP * cos(ang);
    current_temp = temp + rand_float(&seed, -RANDOM_TEMP, RANDOM_TEMP);

    float precip = AVG_PRECIP_PER_MONTH + AMP_PRECIP_PER_MONTH * sin(ang);
    current_precip = precip + rand_float(&seed, -RANDOM_PRECIP, RANDOM_PRECIP);
    if (current_precip < 0.)
      current_precip = 0.;

    // handle burned area
    if (current_burn) {
      // its all gone :(
      current_gheight = 0;
      current_ndeer = 0;
    }

// Done Printing
#pragma omp barrier
  }
}

void deer() {
  while (current_year < 2028) {
    // compute into local variables the next number of deer based on the current
    // set of global state variables

    int next_ndeer = current_ndeer;
    int carrying_capacity = (int)(current_gheight);
    if (next_ndeer < carrying_capacity)
      next_ndeer++;
    else if (next_ndeer > carrying_capacity)
      next_ndeer--;

    if (next_ndeer < 0)
      next_ndeer = 0;

    dbg_printf("  d> current deer ct: %i\n", current_ndeer);
    dbg_printf("  d> next deer count: %i\n", next_ndeer);

// Done Computing
#pragma omp barrier

    // copy into global state
    current_ndeer = next_ndeer;

    dbg_printf("  d> deer updated\n");

// Done Assigning
#pragma omp barrier

    // nop

    dbg_printf("  d> print nop\n");

// Done Printing
#pragma omp barrier
  }
}

void grain() {
  while (current_year < 2028) {
    // compute into local variables the next grain height based on the current
    // set of global state variables

    float temp_factor = exp(-SQR((current_temp - MIDTEMP) / 10.));
    float precip_factor = exp(-SQR((current_precip - MIDPRECIP) / 10.));

    float next_gheight = current_gheight;
    next_gheight += temp_factor * precip_factor * GRAIN_GROWS_PER_MONTH;
    next_gheight -= (float)current_ndeer * ONE_DEER_EATS_PER_MONTH;

    if (next_gheight < 0.) {
      next_gheight = 0.;
    }

    dbg_printf("  g> current grain hgt: %f\n", current_gheight);
    dbg_printf("  g> next grain height: %f\n", next_gheight);

// Done Computing
#pragma omp barrier

    // copy into global state
    current_gheight = next_gheight;

    dbg_printf("  g> grain updated\n");

// Done Assigning
#pragma omp barrier

    // nop
    dbg_printf("  g> print nop\n");

// Done Printing
#pragma omp barrier
  }
}

// custom agent
void california_wildfires() {
  while (current_year < 2028) {

    // its california...
    // during summer (months 5..8), there's a chance all of the grain will burn
    int next_burn = 0;
    if (current_month >= 5 && current_month <= 8) {
      float next_burn_chance = rand_float(&seed, 0., 1.);
      next_burn = next_burn_chance > 1 - BURN_CHANCE;
    }

    dbg_printf("  c> is it burning? %i\n", next_burn);

// Done Computing
#pragma omp barrier

    // nop
    current_burn = next_burn;

    dbg_printf("  c> burn status updated");

// Done Assigning
#pragma omp barrier

    // nop

    dbg_printf("  c> burn nop\n");

// Done Printing
#pragma omp barrier
  }
}

// helper functions

float rand_float(unsigned int *seedp, float low, float high) {
  float r = (float)rand_r(seedp); // 0 - RAND_MAX

  return (low + r * (high - low) / (float)RAND_MAX);
}

float SQR(float x) { return x * x; }
