/*
 * Copyright (c) 2012, Mattias Holm. All rights reserved.
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

#include <celmek/celmek.h>

typedef struct {
  double L[4];
  double a[4];
  double e[4];
  double i[4];
  double omega[4];
  double pi[4];
} cm_planet_t;

// TODO: Tables need to be verified
// Mean equinox of the date
static cm_planet_t mercury = {
  .L     = {252.250906,    149474.0722491,    0.00030397,    0.000000018},
  .a     = {  0.387098310},
  .e     = {  0.20563175,       0.000020406, -0.0000000284, -0.00000000017},
  .i     = {  7.004986,         0.0018215,   -0.00001809,    0.000000053},
  .omega = { 48.330893,         1.1861890,    0.00017587,    0.000000211},
  .pi    = { 77.456119,         1.5564775,    0.00029589,    0.000000056},
};

static cm_planet_t venus = {
  .L     = {181.979801,    58519.2130302,   0.00031060,   0.000000015},
  .a     = {  0.723329820},
  .e     = {  0.00677188,     -0.000047766, 0.0000000975, 0.00000000044},
  .i     = {  3.394662,        0.0010037,  -0.00000088,  -0.000000007},
  .omega = { 76.679920,        0.9011190,   0.00040665,  -0.000000080},
  .pi    = {131.563707,        1.4022188,  -0.00107337,  -0.000005315},
};

static cm_planet_t earth = {
  .L     = {100.466449,    36000.7698231,    0.00030368,   0.000000021},
  .a     = {  1.000001018},
  .e     = {  0.01670862,     -0.000042073, -0.0000001236, 0.00000000004},
  .i     = {  0},
  .omega = {   },
  .pi    = {102.937348,        1.7195269,    0.00045962,   0.000000499},
};

static cm_planet_t mars = {
  .L     = {355.433275,    19141.6964746,    0.00031097,    0.000000015},
  .a     = {  1.523679342},
  .e     = {  0.09340062,      0.000090483, -0.0000000806, -0.00000000035},
  .i     = {  1.849726,       -0.0006010,    0.00001276,   -0.000000006},
  .omega = { 49.558093,        0.7720923,    0.00001605,    0.000002325},
  .pi    = {336.060234,        1.8410331,    0.00013515,    0.000000318},
};

static cm_planet_t jupiter = {
  .L     = { 34.351484,   3036.3027889,     0.00022374,    0.000000025},
  .a     = {  5.202603191,   0.0000001913},
  .e     = {  0.04849485,    0.000163244,  -0.0000004719, -0.00000000197},
  .i     = {  1.303270,     -0.0054966,     0.00000465,   -0.000000004},
  .omega = {100.464441,      1.0209550,     0.00040117,    0.000000569},
  .pi    = { 14.331309,      1.6126668,     0.00103127,   -0.000004569},
};

static cm_planet_t saturn = {
  .L     = { 50.077471,   1223.5110141,     0.00051952,  -0.000000003},
  .a     = {  9.554909596,  -0.0000021389},
  .e     = {  0.05550862,   -0.000346818,  -0.0000006456, 0.00000000338},
  .i     = {  2.488878,     -0.0037363,    -0.00001516,   0.000000089},
  .omega = {113.665524,      0.8770979,    -0.00012067,  -0.000002380},
  .pi    = { 93.056787,      1.9637694,     0.00083757,   0.000004899},
};

static cm_planet_t uranus = {
  .L     = {314.055005,   429.8640561,    0.00030434,     0.000000026},
  .a     = { 19.218446062, -0.0000000372, 0.00000000098},
  .e     = {  0.04629590,  -0.000027337,  0.0000000790,   0.00000000025},
  .i     = {  0.773196,     0.0007744,    0.00003749,    -0.000000092},
  .omega = { 74.005947,     0.5211258,    0.00133982,     0.000018516},
  .pi    = {173.005159,     1.4863784,    0.00021450,     0.000000433},
};

static cm_planet_t neptune = {
  .L     = {304.348665,   219.8833092,    0.00030926,     0.000000018},
  .a     = { 20.110386869, -0.0000001663, 0.00000000069},
  .e     = {  0.00898809,   0.000006408, -0.0000000008,  -0.00000000005},
  .i     = {  1.769952,    -0.0093082,   -0.00000708,     0.000000028},
  .omega = {131.784057,     1.1022057,    0.00026006,    -0.000000636},
  .pi    = { 48.123691,     1.4262677,    0.00037918,    -0.000000003},
};




// Standard equinox J2000.0
static cm_planet_t mercury_j2000 = {
  .L     = {252.250906, 149427.6746358, -0.00000535, 0.000000002},
  .a     = {0.387098310},
  .e     = {0.20563175, 0.000020406, -0.0000000284, -0.00000000017},
  .i     = {7.004986, -0.0059516, 0.00000081, 0.000000041},
  .omega = {48.330893, -0.1254229, -0.00008833, -0.000000196},
  .pi    = {77.456119},
};

static cm_planet_t venus_j2000 = {
  .L     = {181.979801,    58517.8156760, 0.00000165, -0.000000002},    
  .a     = {  0.723329820},
  .e     = {  0.00677188,  -0.000047766, 0.0000000975, 0.00000000044},
  .i     = {  3.394662,    -0.0008568, -0.00003244, 0.000000010},
  .omega = { 76.679920,    -0.2780080, -0.00014256, -0.000000198},
  .pi    = {131.563707,    0.0048646, -0.00138432, -0.000005332},
};

static cm_planet_t earth_j2000 = {
  .L     = {100.466449,    35999.3728519, -0.00000568, 0.000000000},
  .a     = {  1.000001018},
  .e     = {  0.01670862,  -0.000042073, -0.0000001236, 0.00000000004},
  .i     = {  0,               0.0130546, -0.00000931, -0.000000034},
  .omega = {174.873174, -0.2410908, 0.00004067, -0.000001327},
  .pi    = {102.937348,  0.3225557, 0.00015026, 0.000000478},
};

static cm_planet_t mars_j2000 = {
  .L     = {355.433275,    19140.2993313, 0.00000261, -0.000000003},
  .a     = {  1.523679342},
  .e     = {  0.09340062,  0.000090483, -0.0000000806, -0.00000000035},
  .i     = {  1.849726,    -0.0081479, -0.00002255, -0.000000027},
  .omega = { 49.558093,    -0.2949846, -0.00063993, -0.000002143},
  .pi    = {336.060234,    0.4438898, -0.00017321, 0.000000300},
};

static cm_planet_t jupiter_j2000 = {
  .L     = { 34.351484,   3034.9056746, -0.00008501, 0.000000004},
  .a     = {  5.202603191, 0.0000001913},
  .e     = {  0.04849485, 0.000163244, -0.0000004719, -0.00000000197},
  .i     = {  1.303270,   -0.0019872, 0.00003318, 0.000000092},
  .omega = {100.464441,   0.1766828, 0.00090387, -0.000007032},
  .pi    = { 14.331309,   0.2155525, 0.00072252, -0.000004590},
};

static cm_planet_t saturn_j2000 = {
  .L     = { 50.077471,   122.1137943, 0.00021004, -0.000000019},
  .a     = {  9.554909596, -0.0000021389},
  .e     = {  0.05550862, -0.000346818, -0.0000006456, 0.00000000338},
  .i     = {  2.488878,   0.0025515, -0.00004903, 0.000000018},
  .omega = {113.665524,   -0.2566649, -0.00018345, 0.000000357},
  .pi    = { 93.056787,   0.5665496, 0.00052809, 0.000004882},
};

static cm_planet_t uranus_j2000 = {
  .L     = {314.055005,   428.4669983, -0.00000486, 0.000000006},
  .a     = { 19.218446062, -0.0000000372, 0.00000000098},
  .e     = {  0.04629590, -0.000027337, 0.0000000790, 0.00000000025},
  .i     = {  0.773196,   -0.0016869, 0.00000349, 0.000000016},
  .omega = { 74.005947,   0.0741461, 0.00040540, 0.000000104},
  .pi    = {173.005159,   0.0893206, -0.00009470, 0.000000413},
};

static cm_planet_t neptune_j2000 = {
  .L     = {304.348665,   218.4862002, 0.00000059, -0.000000002},
  .a     = { 20.110386869, -0.0000001663, 0.00000000069},
  .e     = {  0.00898809, 0.000006408, -0.0000000008, -0.00000000005},
  .i     = {  1.769952,   0.0002257, 0.00000023, -0.000000000},
  .omega = {131.784057,   -0.0061651, -0.00000219, -0.000000078},
  .pi    = { 48.123691,   0.0291587, 0.00007051, -0.000000023},
};



static cm_planet_t *epoch[] = {
  &mercury, &venus, &earth, &mars, &jupiter, &saturn, &uranus, &neptune
};

static cm_planet_t *j2000[] = {
  &mercury_j2000, &venus_j2000, &earth_j2000, &mars_j2000,
  &jupiter_j2000, &saturn_j2000, &uranus_j2000, &neptune_j2000
};

// TODO: Vectorise
void
cm_compute_mean_orbital_elements_at_date(cm_orbital_elements_t *elems,
                                         cm_body_id_t planet, double T)
{
  elems->L = epoch[planet]->L[0]
           + epoch[planet]->L[1] * T
           + epoch[planet]->L[2] * T*T
           + epoch[planet]->L[3] * T*T*T;
  elems->a = epoch[planet]->a[0]
           + epoch[planet]->a[1] * T
           + epoch[planet]->a[2] * T*T
           + epoch[planet]->a[3] * T*T*T;
  elems->e = epoch[planet]->e[0]
           + epoch[planet]->e[1] * T
           + epoch[planet]->e[2] * T*T
           + epoch[planet]->e[3] * T*T*T;
  elems->i = epoch[planet]->i[0]
           + epoch[planet]->i[1] * T
           + epoch[planet]->i[2] * T*T
           + epoch[planet]->i[3] * T*T*T;
  elems->omega = epoch[planet]->omega[0]
               + epoch[planet]->omega[1] * T
               + epoch[planet]->omega[2] * T*T
               + epoch[planet]->omega[3] * T*T*T;
  elems->pi = epoch[planet]->pi[0]
            + epoch[planet]->pi[1] * T
            + epoch[planet]->pi[2] * T*T
            + epoch[planet]->pi[3] * T*T*T;
}

double
cm_mean_long_at_date(cm_body_id_t planet, double T)
{
  return epoch[planet]->L[0]
       + epoch[planet]->L[1] * T
       + epoch[planet]->L[2] * T*T
       + epoch[planet]->L[3] * T*T*T;
}
double
cm_semimajor_at_date(cm_body_id_t planet, double T)
{
  return epoch[planet]->a[0]
       + epoch[planet]->a[1] * T
       + epoch[planet]->a[2] * T*T
       + epoch[planet]->a[3] * T*T*T;
}
double
cm_eccentricity_at_date(cm_body_id_t planet, double T)
{
  return epoch[planet]->e[0]
       + epoch[planet]->e[1] * T
       + epoch[planet]->e[2] * T*T
       + epoch[planet]->e[3] * T*T*T;
}
double
cm_incl_to_ecl_at_date(cm_body_id_t planet, double T)
{
  return epoch[planet]->i[0]
       + epoch[planet]->i[1] * T
       + epoch[planet]->i[2] * T*T
       + epoch[planet]->i[3] * T*T*T;
}
double
cm_long_of_asc_node_at_date(cm_body_id_t planet, double T)
{
  return epoch[planet]->omega[0]
       + epoch[planet]->omega[1] * T
       + epoch[planet]->omega[2] * T*T
       + epoch[planet]->omega[3] * T*T*T;
}

double
cm_long_of_peri_at_date(cm_body_id_t planet, double T)
{
  return epoch[planet]->pi[0]
       + epoch[planet]->pi[1] * T
       + epoch[planet]->pi[2] * T*T
       + epoch[planet]->pi[3] * T*T*T;
}

void
cm_compute_mean_orbital_elements_j2000(cm_orbital_elements_t *elems,
                                       cm_body_id_t planet, double T)
{
  elems->L = j2000[planet]->L[0]
           + j2000[planet]->L[1] * T
           + j2000[planet]->L[2] * T*T
           + j2000[planet]->L[3] * T*T*T;
  elems->a = j2000[planet]->a[0]
           + j2000[planet]->a[1] * T
           + j2000[planet]->a[2] * T*T
           + j2000[planet]->a[3] * T*T*T;
  elems->e = j2000[planet]->e[0]
           + j2000[planet]->e[1] * T
           + j2000[planet]->e[2] * T*T
           + j2000[planet]->e[3] * T*T*T;
  elems->i = j2000[planet]->i[0]
           + j2000[planet]->i[1] * T
           + j2000[planet]->i[2] * T*T
           + j2000[planet]->i[3] * T*T*T;
  elems->omega = j2000[planet]->omega[0]
               + j2000[planet]->omega[1] * T
               + j2000[planet]->omega[2] * T*T
               + j2000[planet]->omega[3] * T*T*T;
  elems->pi = j2000[planet]->pi[0]
            + j2000[planet]->pi[1] * T
            + j2000[planet]->pi[2] * T*T
            + j2000[planet]->pi[3] * T*T*T;
}

double
cm_mean_long_j2000(cm_body_id_t planet, double T)
{
  return j2000[planet]->L[0]
       + j2000[planet]->L[1] * T
       + j2000[planet]->L[2] * T*T
       + j2000[planet]->L[3] * T*T*T;
}
double
cm_semimajor_j2000(cm_body_id_t planet, double T)
{
  return j2000[planet]->a[0]
       + j2000[planet]->a[1] * T
       + j2000[planet]->a[2] * T*T
       + j2000[planet]->a[3] * T*T*T;
}
double
cm_eccentricity_j2000(cm_body_id_t planet, double T)
{
  return j2000[planet]->e[0]
       + j2000[planet]->e[1] * T
       + j2000[planet]->e[2] * T*T
       + j2000[planet]->e[3] * T*T*T;
}
double
cm_incl_to_ecl_j2000(cm_body_id_t planet, double T)
{
  return j2000[planet]->i[0]
       + j2000[planet]->i[1] * T
       + j2000[planet]->i[2] * T*T
       + j2000[planet]->i[3] * T*T*T;
}
double
cm_long_of_asc_node_j2000(cm_body_id_t planet, double T)
{
  return j2000[planet]->omega[0]
       + j2000[planet]->omega[1] * T
       + j2000[planet]->omega[2] * T*T
       + j2000[planet]->omega[3] * T*T*T;
}

double
cm_long_of_peri_j2000(cm_body_id_t planet, double T)
{
  return j2000[planet]->pi[0]
       + j2000[planet]->pi[1] * T
       + j2000[planet]->pi[2] * T*T
       + j2000[planet]->pi[3] * T*T*T;
}
