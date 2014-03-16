/*
 * Copyright (c) 2013, Mattias Holm <lorrden(at)openorbit.org>.
 * All rights reserved.
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301  USA
 */

#include <stdio.h>
#include <math.h>
#include <complex.h>
#include <celmek/celmek.h>

// TODO: Implement gust86 model of uranian moons
#define ALEN(a) sizeof(a)/sizeof(a[0])

// n = mean motion
// lambda = longityude
// z = eccentricity
// zeta = inclination

typedef enum {
  N1,
  N2,
  N3,
  N4,
  N5,
  E1,
  E2,
  E3,
  E4,
  E5,
  I1,
  I2,
  I3,
  I4,
  I5,
} gust86_term_id_t;

typedef struct gust86_term_t gust86_term_t;

struct gust86_term_t {
  double mult;
  gust86_term_id_t term;
};

struct gust86_sat_t {
  double n_amp_const;
  size_t n_count;
  double (*n_amp)[3];
  gust86_term_t (*n_terms)[3];

  double lon_amp_const;
  double lon_T;
  size_t lon_count;
  double (*lon_amp)[3];
  gust86_term_t (*lon_terms)[3];

  size_t z_count;
  double (*z_amp)[3];
  gust86_term_t (*z_terms)[3];

  size_t incl_count;
  double (*incl_amp)[3];
  gust86_term_t (*incl_terms)[3];
};


double table_1_e[][5] = {
  { 0.999997, 0.059274, 0.017385, 0.005428, 0.003471},
  {-0.005223, 0.980346, 0.214697, 0.057494, 0.030783},
  {-0.000160,-0.188166, 0.972995, 0.248610, 0.122884},
  {-0.000013,-0.001061,-0.080862, 0.750105, 0.620678},
  {-0.000003,-0.000286, 0.018571,-0.610082, 0.773756},
};

// amp, freq, phase
double miranda_n_amp[][3] = {
  //{4443522.67, NAN, NAN},
  {    -34.92e-6, NAN, NAN},
  {      8.47e-6, NAN, NAN},
  {      1.31e-6, NAN, NAN},
  {    -52.28e-6, NAN, NAN},
  {   -136.65e-6, NAN, NAN}
};

double miranda_lon_amp[][3] = {
  //{-238051.58, NAN, NAN},
  //{4445190.55, NAN, NAN},
  {  25472.17e-6, NAN, NAN},
  {  -3088.31e-6, NAN, NAN},
  {   -318.10e-6, NAN, NAN},
  {    -37.49e-6, NAN, NAN},
  {    -57.85e-6, NAN, NAN},
  {    -62.32e-6, NAN, NAN},
  {    -27.95e-6, NAN, NAN},
};

double miranda_z_amp[][3] = {
  {1312.38e-6, NAN, NAN},
  {  71.81e-6, NAN, NAN},
  {  69.77e-6, NAN, NAN},
  {   6.75e-6, NAN, NAN},
  {   6.27e-6, NAN, NAN},
  {-123.31e-6, NAN, NAN},
  {  39.52e-6, NAN, NAN},
  { 194.10e-6, NAN, NAN},
};

double miranda_incl_amp[][3] = {
  {37871.71e-6, NAN, NAN},
  {   27.01e-6, NAN, NAN},
  {   30.76e-6, NAN, NAN},
  {   12.18e-6, NAN, NAN},
  {    5.37e-6, NAN, NAN},
};

double ariel_n_amp[][3] = {
  //{2492542.57, NAN, NAN},
  {      2.55e-6, NAN, NAN},
  {    -42.16e-6, NAN, NAN},
  {   -102.56e-6, NAN, NAN},
};

double ariel_lon_amp[][3] = {
  //{3098046.41, NAN, NAN},
  //{2492952.52, NAN, NAN},
  {  -1860.50e-6, NAN, NAN},
  {    219.99e-6, NAN, NAN},
  {     23.10e-6, NAN, NAN},
  {      4.30e-6, NAN, NAN},
  {    -90.11e-6, NAN, NAN},
  {    -91.07e-6, NAN, NAN},
  {    -42.75e-6, NAN, NAN},
  {    -16.49e-6, NAN, NAN},
};

double ariel_z_amp[][3] = {
  {  -3.35e-6, NAN, NAN},
  {1187.63e-6, NAN, NAN},
  { 861.59e-6, NAN, NAN},
  {  71.50e-6, NAN, NAN},
  {  55.59e-6, NAN, NAN},
  { -84.60e-6, NAN, NAN},
  {  91.81e-6, NAN, NAN},
  {  20.03e-6, NAN, NAN},
  {  89.77e-6, NAN, NAN},
};

double ariel_incl_amp[][3] = {
  {-121.75e-6, NAN, NAN},
  { 358.25e-6, NAN, NAN},
  { 290.08e-6, NAN, NAN},
  {  97.78e-6, NAN, NAN},
  {  33.97e-6, NAN, NAN},
};


double umbriel_n_amp[][3] = {
  //{1515954.90, NAN, NAN},
  {      9.74e-6, NAN, NAN},
  {   -106.00e-6, NAN, NAN},
  {     54.16e-6, NAN, NAN},
  {    -23.59e-6, NAN, NAN},
  {    -70.70e-6, NAN, NAN},
  {    -36.28e-6, NAN, NAN},
};

double umbriel_lon_amp[][3] = {
  //{2285401.69, NAN, NAN},
  //{1516148.11, NAN, NAN},
  {    660.57e-6, NAN, NAN},
  {    -76.51e-6, NAN, NAN},
  {     -8.96e-6, NAN, NAN},
  {     -2.53e-6, NAN, NAN},
  {    -52.91e-6, NAN, NAN},
  {     -7.34e-6, NAN, NAN},
  {     -1.83e-6, NAN, NAN},
  {    147.91e-6, NAN, NAN},
  {     -7.77e-6, NAN, NAN},
  {     97.76e-6, NAN, NAN},
  {     73.13e-6, NAN, NAN},
  {     34.71e-6, NAN, NAN},
  {     18.89e-6, NAN, NAN},
  {    -67.89e-6, NAN, NAN},
  {    -82.86e-6, NAN, NAN},
  {    -33.81e-6, NAN, NAN},
  {    -15.79e-6, NAN, NAN},
  {    -10.21e-6, NAN, NAN},
  {    -17.08e-6, NAN, NAN},
};

double umbriel_z_amp[][3] = {
  {  -0.21e-6, NAN, NAN},
  {-227.95e-6, NAN, NAN},
  {3904.69e-6, NAN, NAN},
  { 309.17e-6, NAN, NAN},
  { 221.92e-6, NAN, NAN},
  {  29.34e-6, NAN, NAN},
  {  26.20e-6, NAN, NAN},
  {  51.19e-6, NAN, NAN},
  {-103.86e-6, NAN, NAN},
  { -27.16e-6, NAN, NAN},
  { -16.22e-6, NAN, NAN},
  { 549.23e-6, NAN, NAN},
  {  34.70e-6, NAN, NAN},
  {  12.81e-6, NAN, NAN},
  {  21.81e-6, NAN, NAN},
  {  46.25e-6, NAN, NAN},
};


double umbriel_incl_amp[][3] = {
  { -10.86e-6, NAN, NAN},
  { -86.51e-6, NAN, NAN},
  {1113.36e-6, NAN, NAN},
  { 350.14e-6, NAN, NAN},
  { 106.50e-6, NAN, NAN},
};

double titania_n_amp[][3] = {
  //{721663.16, NAN, NAN},
  {    -2.64e-6, NAN, NAN},
  {    -2.16e-6, NAN, NAN},
  {     6.45e-6, NAN, NAN},
  {    -1.11e-6, NAN, NAN},
  {   -62.23e-6, NAN, NAN},
  {   -56.13e-6, NAN, NAN},
  {   -39.94e-6, NAN, NAN},
  {   -91.85e-6, NAN, NAN},
  {   -58.31e-6, NAN, NAN},
  {   -38.60e-6, NAN, NAN},
  {   -26.18e-6, NAN, NAN},
  {   -18.06e-6, NAN, NAN},
};

double titania_lon_amp[][3] = {
  //{856358.79, NAN, NAN},
  //{721718.51, NAN, NAN},
  {    20.61e-6, NAN, NAN},
  {    -2.07e-6, NAN, NAN},
  {    -2.88e-6, NAN, NAN},
  {   -40.79e-6, NAN, NAN},
  {     2.11e-6, NAN, NAN},
  {   -51.83e-6, NAN, NAN},
  {   159.87e-6, NAN, NAN},
  {   -35.05e-6, NAN, NAN},
  {    -1.56e-6, NAN, NAN},
  {    40.54e-6, NAN, NAN},
  {    46.17e-6, NAN, NAN},
  {  -317.76e-6, NAN, NAN},
  {  -305.59e-6, NAN, NAN},
  {  -148.36e-6, NAN, NAN},
  {   -82.92e-6, NAN, NAN},
  {   -49.98e-6, NAN, NAN},
  {   -31.56e-6, NAN, NAN},
  {   -20.56e-6, NAN, NAN},
  {   -13.69e-6, NAN, NAN},
};

double titania_z_amp[][3] = {
  {  -0.02e-6, NAN, NAN},
  {  -1.29e-6, NAN, NAN},
  {-324.51e-6, NAN, NAN},
  { 932.81e-6, NAN, NAN},
  {1120.89e-6, NAN, NAN},
  {  33.86e-6, NAN, NAN},
  {  17.46e-6, NAN, NAN},
  {  16.58e-6, NAN, NAN},
  {  28.89e-6, NAN, NAN},
  { -35.86e-6, NAN, NAN},
  { -17.86e-6, NAN, NAN},
  { -32.10e-6, NAN, NAN},
  {-177.83e-6, NAN, NAN},
  { 793.43e-6, NAN, NAN},
  {  99.48e-6, NAN, NAN},
  {  44.83e-6, NAN, NAN},
  {  25.13e-6, NAN, NAN},
  {  15.43e-6, NAN, NAN},
};

double titania_incl_amp[][3] = {
  {  -1.43e-6, NAN, NAN},
  {  -1.06e-6, NAN, NAN},
  {-140.13e-6, NAN, NAN},
  { 685.72e-6, NAN, NAN},
  { 378.32e-6, NAN, NAN},
};

double oberon_n_amp[][3] = {
  //{466580.54, NAN, NAN},
  {     2.08e-6, NAN, NAN},
  {    -6.22e-6, NAN, NAN},
  {     1.07e-6, NAN, NAN},
  {   -43.10e-6, NAN, NAN},
  {   -38.94e-6, NAN, NAN},
  {   -80.11e-6, NAN, NAN},
  {    59.06e-6, NAN, NAN},
  {    37.49e-6, NAN, NAN},
  {    24.82e-6, NAN, NAN},
  {    16.84e-6, NAN, NAN},
};

double oberon_lon_amp[][3] = {
  //{-915591.80, NAN, NAN},
  //{ 466692.12, NAN, NAN},
  {     -7.82e-6, NAN, NAN},
  {     51.29e-6, NAN, NAN},
  {   -158.24e-6, NAN, NAN},
  {     34.51e-6, NAN, NAN},
  {     47.51e-6, NAN, NAN},
  {     38.96e-6, NAN, NAN},
  {    359.73e-6, NAN, NAN},
  {    282.78e-6, NAN, NAN},
  {    138.60e-6, NAN, NAN},
  {     78.03e-6, NAN, NAN},
  {     47.29e-6, NAN, NAN},
  {     30.00e-6, NAN, NAN},
  {     19.62e-6, NAN, NAN},
  {     13.11e-6, NAN, NAN},
};

double oberon_z_amp[][3] = {
  {   0.00e-6, NAN, NAN},
  {  -0.35e-6, NAN, NAN},
  {  74.53e-6, NAN, NAN},
  {-758.68e-6, NAN, NAN},
  {1397.34e-6, NAN, NAN},
  {  39.00e-6, NAN, NAN},
  {  17.66e-6, NAN, NAN},
  {  32.42e-6, NAN, NAN},
  {  79.75e-6, NAN, NAN},
  {  75.66e-6, NAN, NAN},
  { 134.04e-6, NAN, NAN},
  {-987.26e-6, NAN, NAN},
  {-126.09e-6, NAN, NAN},
  {- 57.42e-6, NAN, NAN},
  {- 32.41e-6, NAN, NAN},
  {- 19.99e-6, NAN, NAN},
  {- 12.94e-6, NAN, NAN},
};

double oberon_incl_amp[][3] = {
  {  -0.44e-6, NAN, NAN},
  {  -0.31e-6, NAN, NAN},
  {  36.89e-6, NAN, NAN},
  {-596.33e-6, NAN, NAN},
  { 451.69e-6, NAN, NAN},
};


double table_2a[10][5] = {
  { 19.440, 1.033, 20.082,  1075.0e-6,  753.0e-6 }, // E1
  {  5.944, 1.046,  6.217,  -900.0e-6,  810.0e-6}, // E2
  {  2.731, 1.049,  2.865, -1913.0e-6, 3527.0e-6}, // E3
  {  1.735, 1.198,  2.078,   922.0e-6,  834.0e-6}, // E4
  {  0.383, 1.009,  0.386,  1639.0e-6,  747.0e-6}, // E5

  {-19.453, 1.044, -20.309, 31660.0e-6, -20782.0e-6}, // I1
  { -5.999, 1.048,  -6.288,   340.0e-6,    142.0e-6}, // I2
  { -2.706, 1.048,  -2.836,   964.0e-6,    645.0e-6}, // I3
  { -1.828, 1.008,  -1.843,  -170.0e-6,    964.0e-6}, // I4
  { -0.259, 1.000,  -0.259,  -290.0e-6,   -524.0e-6}, // I5
};


// GM, lambda, Ni
double table_2b[5][3] = {
  {  4.4,  -238051e-6, 4445190.550e-6}, // Miranda
  { 86.1,  3098046e-6, 2492952.519e-6}, // Ariel
  { 84.0,  2285402e-6, 1516148.111e-6}, // Umbriel
  {230.0,   856359e-6,  721718.509e-6}, // Titania
  {200.0,  -915592e-6,  466692.120e-6}, // Oberon
};


// GM, lambda, Ni
double table_3b[5][3] = {
  {  4.8,  -237924, 4445190.857}, // Miranda
  { 94.4,  3098578, 2492952.726}, // Ariel
  { 71.2,  2285286, 1516148.050}, // Umbriel
  {201.3,   856441,  721718.519}, // Titania
  {233.6,  -915620,  466692.130}, // Oberon
};

//  pi, phase
double table_4[10][2] = {
  { 1312.38e-6, 0.611392}, // E1
  { 1211.43e-6, 2.408074},
  { 4013.06e-6, 2.067774},
  { 1243.57e-6, 0.735131},
  { 1805.91e-6, 0.426767}, // E5
  {37871.91e-6, 5.702313}, // I1
  {  368.39e-6, 0.395757},
  { 1160.02e-6, 0.589326},
  {  978.84e-6, 1.746237},
  {  599.73e-6, 4.206896}, // I5
};

// mi/Mv, Ni, n0i, a0i
double table_5[5][4] = {
  {0.075, 4445190.550e-6, 4443522.67, 129872},
  {1.49 , 2492952.519e-6, 2492542.57, 190945},
  {1.45 , 1516148.111e-6, 1515954.90, 265998},
  {3.97 ,  721718.509e-6,  721663.16, 436298},
  {3.45 ,  466692.120e-6,  466580.54, 583519},
};

gust86_term_t miranda_n_terms[][3] = {
  // Miranda n
  {{1, N1}, {-3, N2}, {2, N3}},
  {{2, N1}, {-6, N2}, {4, N3}},
  {{3, N1}, {-9, N2}, {6, N3}},
  {{1, N1}, {-1, N2}, {0, 0}},
  {{2, N1}, {-2, N2}, {0, 0}},
};
  // Miranda long
gust86_term_t miranda_lon_terms[][3] = {
  //{{1, T}},
  {{1, N1}, {- 3, N2}, {2, N3}},
  {{2, N1}, {- 6, N2}, {4, N3}},
  {{3, N1}, {- 9, N2}, {6, N3}},
  {{4, N1}, {-12, N2}, {8, N3}},
  {{1, N1}, {- 1, N2}},
  {{2, N1}, {- 2, N2}},
  {{3, N1}, {- 3, N2}},
};

gust86_term_t miranda_z_terms[][3] = {
  {{1, E1}},
  {{1, E2}},
  {{1, E3}},
  {{1, E4}},
  {{1, E5}},
  {{-1, N1}, {2, N2}},
  {{-2, N1}, {3, N2}},
  {{1, N1}},
};

gust86_term_t miranda_incl_terms[][3] = {
  {{1, I1}},
  {{1, I2}},
  {{1, I3}},
  {{1, I4}},
  {{1, I5}},
};

gust86_term_t ariel_n_terms[][3] = {
  // Ariel n
  {{1, N1}, {-3, N2}, {2, N3}},
  {{1, N2}, {-1, N3}},
  {{2, N2}, {-2, N3}},
};

gust86_term_t ariel_lon_terms[][3] = {

  // Ariel long
  //{{1, T}},
  {{1, N1}, {- 3, N2}, {2, N3}},
  {{2, N1}, {- 6, N2}, {4, N3}},
  {{3, N1}, {- 9, N2}, {6, N3}},
  {{4, N1}, {-12, N2}, {8, N3}},
  {{1, N2}, {- 1, N3}},
  {{2, N2}, {- 2, N3}},
  {{3, N2}, {- 3, N3}},
  {{2, N2}, {- 2, N4}},
};

gust86_term_t ariel_z_terms[][3] = {
  {{1, E1}},
  {{1, E2}},
  {{1, E3}},
  {{1, E4}},
  {{1, E5}},
  {{-1, N2}, {2, N3}},
  {{-2, N2}, {3, N3}},
  {{-1, N2}, {2, N4}},
  {{1, N2}},
};

gust86_term_t ariel_incl_terms[][3] = {
  {{1, I1}},
  {{1, I2}},
  {{1, I3}},
  {{1, I4}},
  {{1, I5}},
};


gust86_term_t umbriel_n_terms[][3] = {

  // Umbiel n
  {{1, N3}, {- 2, N4}, {1, E3}},
  {{1, N2}, {- 1, N3}},
  {{2, N2}, {- 2, N3}},
  {{1, N3}, {- 1, N4}},
  {{2, N3}, {- 2, N4}},
  {{3, N3}, {- 3, N4}},
};

  // Umbriel long
gust86_term_t umbriel_lon_terms[][3] = {

  //{{1, T}},
  {{1, N1}, { - 3, N2}, {2, N3}},
  {{2, N1}, { - 6, N2}, {4, N3}},
  {{3, N1}, { - 9, N2}, {6, N3}},
  {{4, N1}, { -12, N2}, {8, N3}},
  {{1, N3}, { - 4, N4}, {3, N5}},
  {{1, N3}, { - 2, N4}, {1, E5}},
  {{1, N3}, { - 2, N4}, {1, E4}},
  {{1, N3}, { - 2, N4}, {1, E3}},
  {{1, N3}, { - 2, N4}, {1, E2}},
  {{1, N2}, { - 1, N3}},
  {{2, N2}, { - 2, N3}},
  {{3, N2}, { - 3, N3}},
  {{4, N2}, { - 4, N3}},
  {{1, N3}, { - 1, N4}},
  {{2, N3}, { - 2, N4}},
  {{3, N3}, { - 3, N4}},
  {{4, N3}, { - 4, N4}},
  {{1, N3}, { - 1, N5}},
  {{2, N3}, { - 2, N5}},
};

gust86_term_t umbriel_z_terms[][3] = {
  {{1, E1}},
  {{1, E2}},
  {{1, E3}},
  {{1, E4}},
  {{1, E5}},
  {{1, N2}},
  {{1, N3}},
  {{-1, N2}, {2, N3}},
  {{-2, N2}, {3, N3}},
  {{-3, N2}, {4, N3}},
  {{1, N4}},
  {{-1, N3}, {2, N4}},
  {{-2, N3}, {3, N4}},
  {{-3, N3}, {4, N4}},
  {{-1, N3}, {2, N5}},
  {{1, N3}},
};

gust86_term_t umbriel_incl_terms[][3] = {
  {{1, I1}},
  {{1, I2}},
  {{1, I3}},
  {{1, I4}},
  {{1, I5}},
};


gust86_term_t titania_n_terms[][3] = {

  // Titania n
  {{1, N3}, {- 2, N4}, {1, E3}},
  {{2, N4}, {- 3, N5}, {1, E5}},
  {{2, N4}, {- 3, N5}, {1, E4}},
  {{2, N4}, {- 3, N5}, {1, E3}},
  {{1, N2}, {- 1, N4}},
  {{1, N3}, {- 1, N4}},
  {{1, N4}, {- 1, N5}},
  {{2, N4}, {- 2, N5}},
  {{3, N4}, {- 3, N5}},
  {{4, N4}, {- 4, N5}},
  {{5, N4}, {- 5, N5}},
  {{6, N4}, {- 6, N5}},
};

  // Titania long
gust86_term_t titania_lon_terms[][3] = {
  //{{1, T}},
  {{1, N3}, {- 4, N4}, {3, N5}},
  {{1, N3}, {- 2, N4}, {1, E5}},
  {{1, N3}, {- 2, N4}, {1, E4}},
  {{1, N3}, {- 2, N4}, {1, E3}},
  {{1, N3}, {- 2, N4}, {1, E2}},
  {{2, N4}, {- 3, N5}, {1, E5}},
  {{2, N4}, {- 3, N5}, {1, E4}},
  {{2, N4}, {- 3, N5}, {1, E3}},
  {{3, N4}, {- 4, N5}, {1, E5}},
  {{1, N2}, {- 1, N4}},
  {{1, N3}, {- 1, N4}},
  {{1, N4}, {- 1, N5}},
  {{2, N4}, {- 2, N5}},
  {{3, N4}, {- 3, N5}},
  {{4, N4}, {- 4, N5}},
  {{5, N4}, {- 5, N5}},
  {{6, N4}, {- 6, N5}},
  {{7, N4}, {- 7, N5}},
  {{8, N4}, {- 8, N5}},
};

gust86_term_t titania_z_terms[][3] = {
  {{1, E1}},
  {{1, E2}},
  {{1, E3}},
  {{1, E4}},
  {{1, E5}},
  {{1, N2}},
  {{1, N4}},
  {{-1, N2}, {2, N4}},
  {{1, N3}},
  {{-1, N3}, {2, N4}},
  {{1, N4}},
  {{1, N5}},
  {{-1, N4}, {2, N5}},
  {{-2, N4}, {3, N5}},
  {{-3, N4}, {4, N5}},
  {{-4, N4}, {5, N5}},
  {{-5, N4}, {6, N5}},
  {{-6, N4}, {7, N5}},
};

gust86_term_t titania_incl_terms[][3] = {
  {{1, I1}},
  {{1, I2}},
  {{1, I3}},
  {{1, I4}},
  {{1, I5}},
};


gust86_term_t oberon_n_terms[][3] = {

  // Oberon n
  {{2, N4}, {- 3, N5}, {1, E5}},
  {{2, N4}, {- 3, N5}, {1, E4}},
  {{2, N4}, {- 3, N5}, {1, E3}},
  {{1, N2}, {- 1, N5}},
  {{1, N3}, {- 1, N5}},
  {{1, N4}, {- 1, N5}},
  {{2, N4}, {- 2, N5}},
  {{3, N4}, {- 3, N5}},
  {{4, N4}, {- 4, N5}},
  {{5, N4}, {- 5, N5}},
};

gust86_term_t oberon_lon_terms[][3] = {

  // Oberon long
  //{{1, T},  {0, 0},   {0, 0}},
  {{1, N3}, {-4, N4}, {3, N5}},
  {{2, N4}, {-3, N5}, {1, E5}},
  {{2, N4}, {-3, N5}, {1, E4}},
  {{2, N4}, {-3, N5}, {1, E3}},
  {{1, N2}, {-1, N5}},
  {{1, N3}, {-1, N5}},
  {{1, N4}, {-1, N5}},
  {{2, N4}, {-2, N5}},
  {{3, N4}, {-3, N5}},
  {{4, N4}, {-4, N5}},
  {{5, N4}, {-5, N5}},
  {{6, N4}, {-6, N5}},
  {{7, N4}, {-7, N5}},
  {{8, N4}, {-8, N5}},
};

gust86_term_t oberon_z_terms[][3] = {
  {{1, E1}},
  {{1, E2}},
  {{1, E3}},
  {{1, E4}},
  {{1, E5}},
  {{1, N2}},
  {{-1, N2}, {2, N5}},
  {{1, N3}},
  {{1, N4}},
  {{1, N5}},
  {{-1, N4}, {2, N5}},
  {{-2, N4}, {3, N5}},
  {{-3, N4}, {4, N5}},
  {{-4, N4}, {5, N5}},
  {{-5, N4}, {6, N5}},
  {{-6, N4}, {7, N5}},
  {{-7, N4}, {8, N5}},
};

gust86_term_t oberon_incl_terms[][3] = {
  {{1, I1}},
  {{1, I2}},
  {{1, I3}},
  {{1, I4}},
  {{1, I5}},
};


double
get_freq(gust86_term_t terms[][3], int n, int i, int j)
{
  if (N1 <= terms[i][j].term && terms[i][j].term <= N5) {
    return table_2b[terms[i][j].term - N1][2];
  } else if (E1 <= terms[i][j].term && terms[i][j].term <= I5) {
    return (table_2a[terms[i][j].term-E1][2] * (M_PI/180.0)) / 365.25;

    //return table_1_e[n-1][terms[i][j].term - E1] * table_4[terms[i][j].term - E1][0];//table_1_e[n-1][terms[i][j].term - E1];
  } else {
    return 0.0;
  }
}

double
get_phase(gust86_term_t terms[][3], int n, int i, int j)
{
  if (N1 <= terms[i][j].term && terms[i][j].term <= N5) {
    return table_2b[terms[i][j].term - N1][1];
  } else if (E1 <= terms[i][j].term && terms[i][j].term <= I5) {
    return table_4[terms[i][j].term - E1][1];
  } else {
    return 0.0;
  }
}

#include <math.h>
void
gust87_step(void)
{
  //double A, w, t, phi;
  // Mean motion
  //A * cos(w*t + phi);

  // Mean longitude
  //A * sin(w*t + phi);

}

#define INIT_LOOP(sat, n)                                                      \
  do {                                                                         \
    for (int i = 0 ; i < ALEN(sat##_n_amp) ; i ++) {                           \
      double freq = 0.0;                                                       \
      double phase = 0.0;                                                      \
      for (int j = 0 ; j < 3 ; j++) {                                          \
        freq += sat##_n_terms[i][j].mult * get_freq(sat##_n_terms, n, i, j);   \
        phase += sat##_n_terms[i][j].mult * get_phase(sat##_n_terms, n, i, j); \
        phase = fmod(phase, 2.0*M_PI);                                         \
        if (phase < 0.0) phase = 2.0*M_PI + phase;                             \
      }                                                                        \
      sat##_n_amp[i][1] = freq;                                                \
      sat##_n_amp[i][2] = phase;                                               \
                                                                               \
    printf(#sat " n %d %f\t%f\t%f\n", i,                                       \
           sat##_n_amp[i][0], (2.0*M_PI)/sat##_n_amp[i][1], sat##_n_amp[i][2]);\
    }                                                                          \
                                                                               \
    for (int i = 0 ; i < ALEN(sat##_lon_amp) ; i ++) {                         \
      double freq = 0.0;                                                       \
      double phase = 0.0;                                                      \
      for (int j = 0 ; j < 3 ; j++) {                                          \
        freq += sat##_lon_terms[i][j].mult * get_freq(sat##_lon_terms, n, i, j); \
        phase += sat##_lon_terms[i][j].mult * get_phase(sat##_lon_terms, n, i, j); \
        phase = fmod(phase, 2.0*M_PI);                                         \
        if (phase < 0.0) phase = 2.0*M_PI + phase;                             \
      }                                                                        \
      sat##_lon_amp[i][1] = freq;                                              \
      sat##_lon_amp[i][2] = phase;                                             \
                                                                               \
    printf(#sat " lon %d %f\t%f\t%f\n", i,                                     \
           sat##_lon_amp[i][0], (2.0*M_PI)/sat##_lon_amp[i][1],                \
           sat##_lon_amp[i][2]);                                               \
    }                                                                          \
    for (int i = 0 ; i < ALEN(sat##_z_amp) ; i ++) {                           \
      double freq = 0.0;                                                       \
      double phase = 0.0;                                                      \
      for (int j = 0 ; j < 3 ; j++) {                                          \
        freq += sat##_z_terms[i][j].mult * get_freq(sat##_z_terms, n, i, j);   \
        phase += sat##_z_terms[i][j].mult * get_phase(sat##_z_terms, n, i, j); \
        phase = fmod(phase, 2.0*M_PI);                                         \
        if (phase < 0.0) phase = 2.0*M_PI + phase;                             \
      }                                                                        \
      sat##_z_amp[i][1] = freq;                                                \
      sat##_z_amp[i][2] = phase;                                               \
                                                                               \
    printf(#sat " z %d %f\t%f\t%f\n", i,                                       \
           sat##_z_amp[i][0], (2.0*M_PI)/sat##_z_amp[i][1], sat##_z_amp[i][2]);\
    }                                                                          \
                                                                               \
    for (int i = 0 ; i < ALEN(sat##_incl_amp) ; i ++) {                        \
      double freq = 0.0;                                                       \
      double phase = 0.0;                                                      \
      for (int j = 0 ; j < 3 ; j++) {                                          \
        freq += sat##_incl_terms[i][j].mult * get_freq(sat##_incl_terms, n, i, j);   \
        phase += sat##_incl_terms[i][j].mult * get_phase(sat##_incl_terms, n, i, j); \
        phase = fmod(phase, 2.0*M_PI);                                         \
        if (phase < 0.0) phase = 2.0*M_PI + phase;                             \
      }                                                                        \
      sat##_incl_amp[i][1] = freq;                                             \
      sat##_incl_amp[i][2] = phase;                                            \
                                                                               \
      printf(#sat " incl %d %f\t%f\t%f\n", i,                                  \
            sat##_incl_amp[i][0], (2.0*M_PI)/sat##_incl_amp[i][1],             \
            sat##_incl_amp[i][2]);                                             \
        }                                                                      \
                                                                               \
  } while (0)


#define MAKE_SAT(sat, kn, klon, T)                                           \
  gust86_sat_t gust86_##sat = {                                              \
               kn, ALEN(sat##_n_amp), sat##_n_amp, sat##_n_terms,            \
               klon, T, ALEN(sat##_lon_amp), sat##_lon_amp, sat##_lon_terms, \
               ALEN(sat##_z_amp), sat##_z_amp, sat##_z_terms,                \
               ALEN(sat##_incl_amp), sat##_incl_amp, sat##_incl_terms}

MAKE_SAT(miranda, 4443522.67e-6, -238051.58e-6, 4445190.55e-6);
MAKE_SAT(ariel,   2492542.57e-6, 3098046.41e-6, 2492952.52e-6);
MAKE_SAT(umbriel, 1515954.90e-6, 2285401.69e-6, 1516148.11e-6);
MAKE_SAT(titania,  721663.16e-6,  856358.79e-6,  721718.51e-6);
MAKE_SAT(oberon,   466580.54e-6, -915591.80e-6,  466692.12e-6);

void
cm_gust86(gust86_res_t *res, const gust86_sat_t *sat, double t)
{
  res->n = sat->n_amp_const;
  for (int i = 0 ; i < sat->n_count ; i ++) {
    double A = sat->n_amp[i][0];
    double w = sat->n_amp[i][1];
    double phi = sat->n_amp[i][2];
    res->n += A * cos(w * t + phi);
  }

  double T = t;
  res->lon = sat->lon_amp_const;
  res->lon += T * sat->lon_T;
  for (int i = 0 ; i < sat->lon_count ; i ++) {
    double A = sat->lon_amp[i][0];
    double w = sat->lon_amp[i][1];
    double phi = sat->lon_amp[i][2];
    res->lon += A * sin(w * t + phi);
  }

  res->z = 0.0 + 0.0 * I;
  for (int i = 0 ; i < sat->z_count ; i ++) {
    double A = sat->z_amp[i][0];
    double w = sat->z_amp[i][1];
    double phi = sat->incl_amp[i][2];
    res->z += A * cexp(I * (w*t + phi));
  }
  // ζ
  res->incl = 0.0 + 0.0 * I;
  for (int i = 0 ; i < sat->incl_count ; i ++) {
    double A = sat->incl_amp[i][0];
    double w = sat->incl_amp[i][1];
    double phi = sat->incl_amp[i][2];
    res->incl += A * cexp(I * (w*t + phi));
  }
}

void
cm_gust86_to_elems(cm_orbital_elements_t *elems, const gust86_res_t *gust, double epoch)
{
  // gust->lon mean lonitude
  // gust->n mean motion
  //elems->a = ;
  elems->M = fmod(epoch * gust->n, 2*M_PI);
  elems->i = 2.0 * asin(cabs(gust->incl));
  elems->e = cabs(gust->z);
  elems->Omega = carg(gust->incl);
  elems->w = cm_arg_periapsis(carg(gust->z), elems->Omega);

}

#define URANUS_ALPHA_1950 76.6067
#define URANUS_DELTA_1950 15.0322
static double3x3 R;

void
gust86_eme50()
{

}

void
gust86_init(void)
{
  R[0][0] = sin(deg2rad(URANUS_ALPHA_1950));
  R[0][1] = cos(deg2rad(URANUS_ALPHA_1950)) * sin(deg2rad(URANUS_DELTA_1950));
  R[0][2] = cos(deg2rad(URANUS_ALPHA_1950)) * cos(deg2rad(URANUS_DELTA_1950));

  R[1][0] = -cos(deg2rad(URANUS_ALPHA_1950));
  R[1][1] = sin(deg2rad(URANUS_ALPHA_1950)) * sin(deg2rad(URANUS_DELTA_1950));
  R[1][2] = sin(deg2rad(URANUS_ALPHA_1950)) * cos(deg2rad(URANUS_DELTA_1950));

  R[2][0] = 0.0;
  R[2][1] = -cos(deg2rad(URANUS_DELTA_1950));
  R[2][2] = sin(deg2rad(URANUS_DELTA_1950));

  for (int i = 0 ; i < 10 ; i ++) {
    table_2a[i][2] = table_2a[i][0] * table_2a[i][1];
  }

  // Initialise the Uranian sat tables
  INIT_LOOP(miranda, 1); printf("\n");
  INIT_LOOP(ariel, 2); printf("\n");
  INIT_LOOP(umbriel, 3); printf("\n");
  INIT_LOOP(titania, 4); printf("\n");
  INIT_LOOP(oberon, 5);
}


  // Period is derived: use argument directly, use table 5
  //printf("umbriel period %f\n", (2.0*M_PI)/(table_5[1][1] - table_5[2][1]));

  // Phase is derived: replace Ni with lambda0i
  // replace Ei or Ii by phase i in table 4
  //printf("umbriel phase %f\n", (table_2b[1][1] - table_2b[2][1]));

  // mean motion (n): A cos(wt + phi)
  // mean longitude:  A sin(wt + phi)
  // And the coordinates, both complex numbers:
  // z and incl:      A exp sqrt(-1) (wt + phi)

