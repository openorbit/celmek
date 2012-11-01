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

/*
Jupiter Moon simulation
*/
#include <assert.h>
#include <math.h>
#include <stdlib.h>

#include <celmek/celmek.h>

typedef struct {
  double p;
  double t;
} param_t;

typedef enum {
  LIESKE_IO,
  LIESKE_EUROPA,
  LIESKE_GANYMEDE,
  LIESKE_CALLISTO,
} moon_id_t;

typedef enum {
  JM_ELL1 = 0,
  JM_ELL2,
  JM_ELL3,
  JM_ELL4,
  JM_PI1,
  JM_PI2,
  JM_PI3,
  JM_PI4,
  JM_W1,
  JM_W2,
  JM_W3,
  JM_W4,
  JM_PHI,
  JM_PSI,
  JM_G,
  JM_Gp,
  
  JM_NORMAL_PARAM_COUNT,
  // Sum of longitued terms, these work diffrently, but are needed here
  // for tabular definitions 
  JM_L1 = JM_NORMAL_PARAM_COUNT,
  JM_L2,
  JM_L3,
  JM_L4,
  JM_Z1,
  JM_Z2,
  JM_Z3,
  JM_Z4,
  JM_CONST, // Should be set to 1.0
  JM_PI_C,
  JM_PARAM_COUNT
} jm_param_id_t;
  
  
static param_t param_rules[JM_NORMAL_PARAM_COUNT] = {
  // ell
  {106.07947, 203.488955432},
  {175.72938, 101.374724550},
  {120.55434,  50.317609110},
  { 84.44868,  21.571071314},
  
  // pi
  {58.3329, 0.16103936},
  {132.8959, 0.04647985},
  {187.2887, 0.00712740},
  {335.3418, 0.00183998},
  
  // w
  {331.0793, 0.13279430},
  {100.5099, 0.03263047},
  {119.1688, 0.00717704},
  {322.6729, 0.00175934},
  
  //
  // JM_GAMMA,
  // phi
  {191.8132, 0.17390023},
  // psi
  {316.5182, 0.00000208},
  // G (note, special cased in module step)
  {30.23756, 0.0830925701},
  // G'
  {31.97853, 0.0334597339}  
};
static param_t mean_long[] = {
  {106.07947, 203.488955432},
  {175.72938, 101.374724550},
  {120.55434,  50.317609110},
  { 84.44868,  21.571071314},
};

static param_t long_of_perijove[] = {
  {58.3329, 0.16103936},
  {132.8959, 0.04647985},
  {187.2887, 0.00712740},
  {335.3418, 0.00183998}
};

static param_t long_of_nodes[] = {
  {331.0793, 0.13279430},
  {100.5099, 0.03263047},
  {119.1688, 0.00717704},
  {322.6729, 0.00175934},
};


#define JUPITER_PERIHELION_LONGITUDE 13.469942

static double sat_i_long_ampl[] = {
  +0.47259,
  -0.03480,
  -0.01756,
  +0.01080,
  +0.00757,
  +0.00663,
  +0.00453,
  +0.00453,
  -0.00354,
  -0.00317,
  -0.00269,
  +0.00263,

  +0.00186,
  -0.00186,
  +0.00167,
  +0.00158,
  -0.00155,
  -0.00142,
  -0.00115,
  +0.00089,
  +0.00084,
  +0.00084,
  +0.00053
};

static double sat_ii_long_ampl[] = {
  +1.06476,
  0.04253,
  0.03579,
  0.02383,
  0.01977,
  -0.01843,
  0.01299,
  -0.01142,
  0.01078,
  -0.01058,
  0.00870,
  -0.00775,
  0.00524,
  -0.00460,
  0.00450,
  0.00327,
  -0.00296,
  -0.00151,
  0.00146,
  0.00125,
  -0.00117,
      
  -0.00095,
  0.00086,  
  -0.00086,
  -0.00078,
  -0.00064,
  -0.00063,
  0.00061,
  0.00058,
  0.00058,
  0.00056,
  0.00055,
  0.00052,
  -0.00043,
  0.00042,
  0.00041,
  0.00041,
  0.00038,
  0.00032,
  0.00032,
  0.00029
};

static double sat_iii_long_ampl[] = {
  0.16477,
  0.09062,
  -0.06907,
  0.03786,
  0.01844,
  -0.01340,
  0.00703,
  -0.00670,
  -0.00540,
  0.00481,
  -0.00409,
  0.00379,
  0.00235,
  0.00198,
  0.00180,
  0.00129,

  0.00124,
  -0.00119,
  0.00109,
  -0.00099,
  0.00091,
  0.00081,
  -0.00076,
  0.00069,
  -0.00058,
  0.00057,
  -0.00057,
  -0.00052,
  -0.00052,

  0.00048,
  -0.00045,
  -0.00041,
  -0.00038,
  -0.00033,
  -0.00032,
  0.00030,

  -0.00029,
  0.00029,
  0.00026,
  0.00024,
  0.00021,
  -0.00021,
  0.00017
};
static double sat_iv_long_ampl[] = {
  0.84109,
  0.03429,
  -0.03305,
  -0.03211,
  -0.01860,
  0.01182,
  0.00622,
  0.00385,
  -0.00284,
  -0.00233,
  -0.00223,
  -0.00208,
  0.00177,
  0.00134,
  0.00125,
  -0.00117,
  -0.00112,
  0.00106,
  0.00102,
  0.00096,
  0.00087,
  -0.00087,
  0.00085,
  -0.00081,
  0.00071,
  0.00060,
  -0.00056,
  -0.00055,

  0.00051,
  0.00042,
  0.00039,
  0.00036,
  0.00035,
  -0.00035,
  -0.00032,
  0.00030,
  0.00030,
  0.00028,
  -0.00028,
  -0.00027,
  -0.00026,
  0.00025,
  -0.00025,
  -0.00023,
  0.00021,
  -0.00021,
  0.00019,
  -0.00019,
  -0.00018,
  -0.00016
};

static double sat_i_lat_ampl[] =
// Latitude amplitudes
{
  0.0006502, // Sat1
  0.0001835,
  0.0000329,
  -0.0000311,
  0.0000093,
  0.0000075,
  0.0000046,// Sat 1
};
static double sat_ii_lat_ampl[] = {
  0.0081275, // Sat 2
  0.0004512,
  -0.0003286,
  0.0001164,
  0.0000273,
  0.0000143,
  -0.0000143,
  0.0000035,
  -0.0000028, // Sat 2
};
static double sat_iii_lat_ampl[] = {
  0.0032364, // Sat 3
  -0.0016911,
  0.0006849,
  -0.0002806,
  0.0000321,
  0.0000051,
  -0.0000045,
  -0.0000045,
  0.0000037,
  0.0000030,
  -0.0000021, // Sat 3
};
static double sat_iv_lat_ampl[] = {
  -0.0076579, // Sat 4
  0.0044148,
  -0.0005106,
  0.0000773,
  0.0000104,
  -0.0000102,
  0.0000088,
  -0.0000038 // Sat 4
};

static double sat_i_rad_ampl[] =
// Radius amplitudes
{
  -0.0041339, // Sat 1
  -0.0000395,
  -0.0000214,
  0.0000170,
  -0.0000162,
  -0.0000130,
  0.0000106,
  -0.0000063, //Sat 1
};
static double sat_ii_rad_ampl[] = {
  0.0093847, // Sat 2
  -0.0003114,
  -0.0001738,
  -0.0000941,
  0.0000553,
  0.0000523,
  -0.0000290,
  0.0000166,
  0.0000107,
  -0.0000102,
  -0.0000091, //Sat 2
};
static double sat_iii_rad_ampl[] = {
  -0.0014377, // Sat 3
  -0.0007904,
  0.0006342,
  -0.0001758,
  0.0000294,
  -0.0000156,
  0.0000155,
  -0.0000153,
  0.0000070,
  -0.0000051, //Sat 3
};
static double sat_iv_rad_ampl[] = {
  -0.0073391, // Sat 4
  0.0001620,
  0.0000974,
  -0.0000541,
  -0.0000269,
  0.0000182,
  0.0000177,
  -0.0000167,
  0.0000167,
  -0.0000155,
  0.0000142,
  0.0000104,
  
  0.0000092,
  -0.0000089,
  -0.0000062,
  0.0000048 //Sat 4
};



typedef struct {
  double mult;
  jm_param_id_t param;
} trig_param_comp_t;

typedef struct {
  double mult;
  trig_param_comp_t comps[4];
} trig_param_t;

static const trig_param_t sat_i_long_params[] = {
  // Sat 1
  {2, {{1, JM_ELL1},  {-1, JM_ELL2}}},
  {1, {{1, JM_PI1},   {-1, JM_PI4}}},
  {1, {{1, JM_PI1},   { 1, JM_PI3},       {-2, JM_PI_C},    {-2, JM_G}}},
  {1, {{1, JM_ELL2},  {-2, JM_ELL3},        {+1, JM_PI3}}},
  {1, {{1, JM_PHI}}},
  {1, {{1, JM_ELL2},  {-2, JM_ELL3},        {+1, JM_PI4}}},
  {1, {{1, JM_ELL1},  {-1, JM_PI3}}},
  {1, {{1, JM_ELL2},  {-2, JM_ELL3},        {+1, JM_PI2}}},
  {1, {{1, JM_ELL1},  {-1, JM_ELL2}}},
  {1, {{2, JM_PSI},   {-2, JM_PI_C}}},
  {1, {{1, JM_ELL2},  {-2, JM_ELL3},        {+1, JM_PI1}}},
  {1, {{1, JM_ELL1},  {-1, JM_PI4}}},
  
  {1, {{1, JM_ELL1},  {-1, JM_PI1}}},
  {1, {{1, JM_G}}},
  {1, {{1, JM_PI2},   {-1, JM_PI3}}},
  {4, {{1, JM_ELL1},  {-1, JM_ELL2}}},
  {1, {{1, JM_ELL1},  {-1, JM_ELL3}}},
  {1, {{1, JM_PSI},   {+1, JM_W3},        {-2, JM_PI_C},    {-2, JM_G}}},
  {2, {{1, JM_ELL1},  {-2, JM_ELL2},        {+1, JM_W2}}},
  {1, {{1, JM_PI2},   {-1, JM_PI4}}},
  {1, {{1, JM_W2},    {-1, JM_W3}}},
  {1, {{1, JM_ELL1},  {+1, JM_PI3},       {-2, JM_PI_C},    {-2, JM_G}}},
  {1, {{1, JM_PSI},   {-1, JM_W2}}},
};
static const trig_param_t sat_ii_long_params[] = {
  // Sat II
  {2, {{1, JM_ELL2},    {-1, JM_ELL3}}},
  {1, {{1, JM_ELL1},    {-2, JM_ELL2},      {+1, JM_PI3}}},
  {1, {{1, JM_ELL2},    {-1, JM_PI3}}},
  {1, {{1, JM_ELL1},    {-2, JM_ELL2},      {+1, JM_PI4}}},
  {1, {{1, JM_ELL2},    {-1, JM_PI4}}},
  {1, {{1, JM_PHI}}},
  {1, {{1, JM_PI3},     {-1, JM_PI4}}},
  {1, {{1, JM_ELL2},    {-1, JM_ELL3}}},
  {1, {{1, JM_ELL2},    {-1, JM_PI2}}},
  {1, {{1, JM_G}}},
  {1, {{1, JM_ELL2},    {-2, JM_ELL3},      {+1, JM_PI2}}},
  {2, {{1, JM_PSI},     {-1, JM_PI_C}}},
  {2, {{1, JM_ELL1},    {-1, JM_ELL2}}},
  {1, {{1, JM_ELL1},    {-1, JM_ELL3}}},
  {1, {{1, JM_ELL2},    {-2, JM_ELL3},      {+1, JM_PI1}}},
  {1, {{1, JM_PSI},     {-2, JM_G},       {+1, JM_W3},      {- 2, JM_PI_C}}},
  {1, {{1, JM_PI1},     {+1, JM_PI3},     {-2, JM_PI_C},    {- 2, JM_G}}},
  {1, {{2, JM_G}}},
  {1, {{1, JM_PSI},     {-1, JM_W3}}},
  {1, {{1, JM_PSI},     {-1, JM_W4}}},
  {1, {{1, JM_ELL1},    {-2, JM_ELL3},      {+1, JM_PI3}}},
  {2, {{1, JM_ELL2},    {-1, JM_W2}}}, //
  {2, {{1, JM_ELL1},    {-2, JM_ELL2},      {+1, JM_W2}}},
  {1, {{5, JM_Gp},      {-2, JM_G},       {+ 52.225}}},
  {1, {{1, JM_ELL2},    {-1, JM_ELL4}}},
  {1, {{1, JM_ELL1},    {-2, JM_ELL3},      {+1, JM_PI4}}},
  {1, {{3, JM_ELL3},    {-7, JM_ELL4},      {+4, JM_PI4}}},
  {1, {{1, JM_PI1},     {-1, JM_PI4}}},
  {2, {{1, JM_PSI},     {-1, JM_PI_C},    {-1, JM_G}}},
  {1, {{1, JM_W3},      {-1, JM_W4}}},
  {2, {{1, JM_ELL2},    {-1, JM_ELL4}}},
  {2, {{1, JM_ELL1},    {-1, JM_ELL3}}},
  {1, {{3, JM_ELL3},    {-7, JM_ELL4},      {+1, JM_PI3},     {+ 3, JM_PI4}}},
  {1, {{1, JM_ELL1},    {-1, JM_PI3}}},
  {1, {{1, JM_PI3},     {-1, JM_PI2}}},
  {5, {{1, JM_ELL2},    {-1, JM_ELL3}}},
  {1, {{1, JM_PI4},     {-1, JM_PI_C}}},
  {1, {{1, JM_ELL2},    {-1, JM_PI1}}},
  {1, {{1, JM_W2},      {-1, JM_W3}}},
  {2, {{1, JM_ELL3},    {-1, JM_G},       {-1, JM_PI_C}}},
  {1, {{1, JM_PI1},     {-1, JM_PI3}}},
};
static const trig_param_t sat_iii_long_params[] = {
  // Sat III
  {1, {{1, JM_ELL3},    {-1, JM_PI3}}},
  {1, {{1, JM_ELL3},    {-1, JM_PI4}}},
  {1, {{1, JM_ELL2},    {-1, JM_ELL3}}},
  {1, {{1, JM_PI3},     {-1, JM_PI4}}},
  {2, {{1, JM_ELL3},    {-1, JM_ELL4}}},
  {1, {{1, JM_G}}},
  {1, {{1, JM_ELL2},    {-2, JM_ELL3},      {+1, JM_PI3}}},
  {2, {{1, JM_PSI},     {-1, JM_PI_C}}},
  {1, {{1, JM_ELL3},    {-1, JM_ELL4}}},
  {1, {{1, JM_PI1},     {+1, JM_PI2},     {-2, JM_PI_C},    {-2, JM_G}}},
  {1, {{1, JM_ELL2},    {-2, JM_ELL3},      {+1, JM_PI2}}},
  {1, {{1, JM_ELL2},    {-2, JM_ELL3},      {+1, JM_PI4}}},
  {1, {{1, JM_PSI},     {-1, JM_W3}}},
  {1, {{1, JM_PSI},     {-1, JM_W4}}},
  {1, {{1, JM_PHI}}},
  {3, {{1, JM_ELL3},    {-1, JM_ELL4}}},

  {1, {{1, JM_ELL1},    {-1, JM_ELL3}}},
  {1, {{5, JM_Gp},      {-2, JM_G},       {+52.225, JM_CONST}}},
  {1, {{1, JM_ELL1},    {-1, JM_ELL2}}},
  {1, {{3, JM_ELL3},    {-7, JM_ELL4},      {+4, JM_PI4}}},
  {1, {{1, JM_W3},      {-1, JM_W4}}},
  {1, {{3, JM_ELL3},    {-7, JM_ELL4},      {+1, JM_PI3},     {+3, JM_PI4}}},
  {1, {{2, JM_ELL2},    {-3, JM_ELL3},      {+1, JM_PI3}}},
  {1, {{1, JM_PI4},     {-1, JM_PI_C}}},
  {1, {{2, JM_ELL3},    {-3, JM_ELL4},      {+1, JM_PI4}}},
  {1, {{1, JM_ELL3},    {+1, JM_PI3},     {-2, JM_PI_C},    {-2, JM_G}}},
  {1, {{1, JM_ELL3},    {-2, JM_ELL4},      {+1, JM_PI4}}},
  {1, {{1, JM_PI2},     {-1, JM_PI3}}},
  {1, {{1, JM_ELL2},    {-2, JM_ELL3},      {+1, JM_PI1}}},

  {1, {{1, JM_ELL3},    {-2, JM_ELL4},      {+1, JM_PI3}}},
  {1, {{2, JM_ELL2},    {-3, JM_ELL3},      {+1, JM_PI4}}},
  {1, {{1, JM_PI2},     {-1, JM_PI4}}},
  {1, {{2, JM_G}}},
  {1, {{1, JM_PI3},     {-1, JM_PI4},     {+1, JM_W3},      {-1, JM_W4}}},
  {1, {{3, JM_ELL3},    {-7, JM_ELL4},      {+2, JM_PI3},     {+2, JM_PI4}}},
  {4, {{1, JM_ELL3},    {-1, JM_ELL4}}},

  {1, {{1, JM_W3},      {+1, JM_PSI},     {-2, JM_PI_C},    {-2, JM_G}}},
  {1, {{1, JM_ELL3},    {+1, JM_PI4},     {-2, JM_PI_C},    {-2, JM_G}}},
  {1, {{1, JM_ELL3},    {-1, JM_PI_C},    {-2, JM_G}}},
  {1, {{1, JM_ELL2},    {-3, JM_ELL3},      {+2, JM_L4}}},
  {2, {{1, JM_ELL3},    {-1, JM_PI_C},    {-1, JM_G}}},
  {1, {{1, JM_ELL3},    {-1, JM_PI2}}},
  {2, {{1, JM_ELL3},    {-1, JM_PI3}}},
};
static const trig_param_t sat_iv_long_params[] = {
  // Sat IV
  {1, {{1, JM_ELL4},    {-1, JM_PI4}}},
  {1, {{1, JM_PI4},     {-1, JM_PI3}}},
  {2, {{1, JM_PSI},     {-1, JM_PI_C}}},
  {1, {{1, JM_G}}},
  {1, {{1, JM_ELL4},    {-1, JM_PI3}}},
  {1, {{1, JM_PSI},     {-1, JM_W4}}},
  {1, {{1, JM_ELL4},    {+1, JM_PI4},     {-3, JM_G},       {-2, JM_PI_C}}},
  {2, {{1, JM_ELL4},    {-1, JM_PI4}}},
  {1, {{5, JM_Gp},      {-2, JM_G},       {+52.225, JM_CONST}}},
  {2, {{1, JM_PSI},     {-1, JM_PI4}}},
  {1, {{1, JM_ELL3},    {-1, JM_ELL4}}},
  {1, {{1, JM_ELL4},    {-1, JM_PI_C}}},
  {1, {{1, JM_PSI},     {+1, JM_W4},      {-2, JM_PI4}}},
  {1, {{1, JM_PI4},     {-1, JM_PI_C}}},
  {2, {{1, JM_ELL4},    {-1, JM_G},       {-1, JM_PI_C}}},
  {1, {{2, JM_G}}},
  {2, {{1, JM_ELL3},    {-1, JM_ELL4}}},
  {1, {{3, JM_ELL3},    {-7, JM_ELL4},      {+4, JM_PI4}}},
  {1, {{1, JM_ELL4},    {-1, JM_G},       {-1, JM_PI_C}}},
  {1, {{2, JM_ELL4},    {-1, JM_PSI},     {-1, JM_W4}}},
  {2, {{1, JM_PSI},     {-1, JM_W4}}},
  {1, {{3, JM_ELL3},    {-7, JM_ELL4},      {+1, JM_PI3},     {+3, JM_PI4}}},
  {1, {{1, JM_ELL3},    {-2, JM_ELL4},      {+1, JM_PI4}}},
  {2, {{1, JM_ELL4},    {-1, JM_PSI}}},
  {1, {{1, JM_ELL4},    {+1, JM_PI4},     {-2, JM_PI_C},      {-3, JM_G}}},
  {1, {{1, JM_ELL1},    {-1, JM_ELL4}}},
  {1, {{1, JM_PSI},     {-1, JM_W3}}},
  {1, {{1, JM_ELL3},    {-2, JM_ELL4},      {+1, JM_PI3}}},

  {1, {{1, JM_ELL2},    {-1, JM_ELL4}}},
  {2, {{1, JM_PSI},     {-1, JM_G},       {-1, JM_PI_C}}},
  {2, {{1, JM_PI4},     {-1, JM_W4}}},
  {1, {{1, JM_PSI},     {+1, JM_PI_C},    {-1, JM_PI4},     {-1, JM_W4}}},
  {1, {{2, JM_Gp},      {-1, JM_G},       {+188.37, JM_CONST}}},
  {1, {{1, JM_ELL4},    {-1, JM_PI4},     {+2, JM_PI_C},      {-2, JM_PSI}}},
  {1, {{1, JM_ELL4},    {+1, JM_PI4},     {-2, JM_PI_C},      {-1, JM_G}}},
  {1, {{3, JM_ELL3},    {-7, JM_ELL4},      { 2, JM_PI3},     {+2, JM_PI4}}},
  {1, {{2, JM_Gp},      {-2, JM_G},       {+149.15, JM_CONST}}},
  {1, {{1, JM_ELL4},    {-1, JM_PI4},     {+2, JM_PSI},     {-2, JM_PI_C}}},
  {2, {{1, JM_ELL4},    {-1, JM_W4}}},
  {1, {{1, JM_PI3},     {-1, JM_PI4},     {+1, JM_W3},      {-1, JM_W4}}},
  {1, {{5, JM_Gp},      {-3, JM_G},       {+188.37, JM_CONST}}},
  {1, {{1, JM_W4},      {-1, JM_W3}}},
  {1, {{1, JM_ELL2},    {-3, JM_ELL3},      {+2, JM_L4}}},
  {3, {{1, JM_ELL3},    {-1, JM_ELL4}}},
  {1, {{2, JM_ELL4},    {-2, JM_PI_C},      {-3, JM_G}}},
  {1, {{2, JM_ELL3},    {-3, JM_ELL4},      {+1, JM_PI4}}},
  {1, {{1, JM_ELL4},    {-1, JM_PI4},     {-1, JM_G}}},
  {1, {{2, JM_ELL4},    {-1, JM_PI3},     {-1, JM_PI4}}},
  {1, {{1, JM_ELL4},    {-1, JM_PI4},     {+1, JM_G}}},
  {1, {{1, JM_ELL4},    {+1, JM_PI3},     {-2, JM_PI_C},      {-2, JM_G}}},
};



// Latitude args
static const trig_param_t sat_i_lat_params[] = {
  {1, {{1, JM_L1},  {-1, JM_W1}}},
  {1, {{1, JM_L1},  {-1, JM_W2}}},
  {1, {{1, JM_L1},  {-1, JM_W3}}},
  {1, {{1, JM_L1},  {-1, JM_PSI}}},
  {1, {{1, JM_L1},  {-1, JM_W4}}},
  {1, {{3, JM_L1},  {-4, JM_ELL2},  {-1.9927, JM_Z1},   {+1, JM_W2}}},
  {1, {{1, JM_L1},  {+1, JM_PSI}, {-2, JM_PI_C},  {-2, JM_G}}},
};
static const trig_param_t sat_ii_lat_params[] = {
  {1, {{1, JM_L1},  {-1, JM_W2}}},
  {1, {{1, JM_L1},  {-1, JM_W3}}},
  {1, {{1, JM_L1},  {-1, JM_PSI}}},
  {1, {{1, JM_L1},  {-1, JM_W4}}},
  {1, {{1, JM_ELL1},  {-2, JM_ELL3},  {+1.0146, JM_Z2},   {+1, JM_W2}}},
  {1, {{1, JM_L2},  {+1, JM_PSI}, {-2, JM_PI_C},  {-2, JM_G}}},
  {1, {{1, JM_L1},  {-1, JM_W1}}},
  {1, {{1, JM_L2},  {-1, JM_PSI}, {+1, JM_G}}},
  {1, {{1, JM_ELL1},  {-2, JM_ELL3},  {+1.0146, JM_Z2},   {+1, JM_W3}}},
};
static const trig_param_t sat_iii_lat_params[] = {
  {1, {{1, JM_L3},  {-1, JM_W3}}},
  {1, {{1, JM_L3},  {-1, JM_PSI}}},
  {1, {{1, JM_L3},  {-1, JM_W4}}},
  {1, {{1, JM_L3},  {-1, JM_W2}}},
  {1, {{1, JM_L3},  {+1, JM_PSI}, {-2, JM_PI_C},  {-2, JM_G}}},
  {1, {{1, JM_L3},  {-1, JM_PSI}, {+1, JM_G}}},
  {1, {{1, JM_L3},  {-1, JM_PSI}, {-1, JM_G}}},
  {1, {{1, JM_L3},  {+1, JM_PSI}, {-2, JM_PI_C}}},
  {1, {{1, JM_L3},  {+1, JM_PSI}, {-2, JM_PI_C},  {-3, JM_G}}},
  {1, {{2, JM_ELL2},  {-3, JM_L3},  {+4.03, JM_Z3},     {+1, JM_W2}}},
  {1, {{2, JM_ELL2},  {-3, JM_L3},  {+4.03, JM_Z3},     {+1, JM_W3}}},
};
static const trig_param_t sat_iv_lat_params[] = {
  {1, {{1, JM_L4},  {-1, JM_PSI}}},
  {1, {{1, JM_L4},  {-1, JM_W4}}},
  {1, {{1, JM_L4},  {-1, JM_W3}}},
  {1, {{1, JM_L4},  {+1, JM_PSI}, {-2, JM_PI_C},  {-2, JM_G}}},
  {1, {{1, JM_L4},  {-1, JM_PSI}, {+1, JM_G}}},
  {1, {{1, JM_L4},  {-1, JM_PSI}, {-1, JM_G}}},
  {1, {{1, JM_L4},  {+1, JM_PSI}, {-2, JM_PI_C},  {-3, JM_G}}},
  {1, {{1, JM_L4},  {+1, JM_PSI}, {-2, JM_PI_C},  {-1, JM_G}}},
};


// Radius args
static const trig_param_t sat_i_rad_params[] = {
  {2, {{1, JM_ELL1},  {-1, JM_ELL2}}},
  {1, {{1, JM_ELL1},  {-1, JM_PI3}}},
  {1, {{1, JM_ELL1},  {-1, JM_PI4}}},
  {1, {{1, JM_ELL1},  {-1, JM_ELL2}}},
  {1, {{1, JM_ELL1},  {-1, JM_PI1}}},
  {4, {{1, JM_ELL1},  {-1, JM_ELL2}}},
  {1, {{1, JM_ELL1},  {-1, JM_ELL3}}},
  {1, {{1, JM_ELL1},  {+1, JM_PI3},   {-2, JM_PI_C},  {-2, JM_G}}},
};
static const trig_param_t sat_ii_rad_params[] = {
  {1, {{1, JM_ELL1},  {-1, JM_ELL2}}},
  {1, {{1, JM_ELL2},  {-1, JM_PI3}}},
  {1, {{1, JM_ELL2},  {-1, JM_PI4}}},
  {1, {{1, JM_ELL2},  {-1, JM_PI2}}},
  {1, {{1, JM_ELL2},  {-1, JM_ELL3}}},
  {1, {{1, JM_ELL1},  {-1, JM_ELL3}}},
  {2, {{1, JM_ELL1},  {-1, JM_ELL2}}},
  {2, {{1, JM_ELL2},  {-1, JM_W2}}},
  {1, {{1, JM_ELL1},  {-2, JM_ELL3},    {+1, JM_PI3}}},
  {1, {{1, JM_ELL2},  {-1, JM_PI1}}},
  {2, {{1, JM_ELL1},  {-1, JM_ELL3}}},
};
static const trig_param_t sat_iii_rad_params[] = {
  {1, {{1, JM_ELL3},  {-1, JM_PI3}}},
  {1, {{1, JM_ELL3},  {-1, JM_PI4}}},
  {1, {{1, JM_ELL2},  {-1, JM_ELL3}}},
  {2, {{1, JM_ELL3},  {-1, JM_ELL4}}},
  {1, {{1, JM_ELL3},  {-1, JM_ELL4}}},
  {3, {{1, JM_ELL3},  {-1, JM_ELL4}}},
  {1, {{1, JM_ELL1},  {-1, JM_ELL3}}},
  {1, {{1, JM_ELL1},  {-1, JM_ELL2}}},
  {1, {{2, JM_ELL2},  {-3, JM_ELL3},    {+1, JM_PI3}}},
  {1, {{1, JM_ELL3},  {+1, JM_PI3},   {-2, JM_PI_C},  {-2, JM_G}}},
};
static const trig_param_t sat_iv_rad_params[] = {
  {1, {{1, JM_ELL4},  {-1, JM_PI4}}},
  {1, {{1, JM_ELL4},  {-1, JM_PI3}}},
  {1, {{1, JM_ELL3},  {-1, JM_ELL4}}},
  {1, {{1, JM_ELL4},  {+1, JM_PI4},   {-2, JM_PI_C},  {-2, JM_G}}},
  {2, {{1, JM_ELL4},  {-1, JM_PI4}}},
  {1, {{1, JM_ELL4},  {-1, JM_PI_C}}},
  {2, {{1, JM_ELL3},  {-1, JM_ELL4}}},
  {1, {{2, JM_ELL4},  {-1, JM_PSI},   {-1, JM_W4}}},
  {1, {{1, JM_PSI}, {-1, JM_W4}}},
  {2, {{1, JM_ELL4},  {-1, JM_PI_C},  {-1, JM_G}}},
  {2, {{1, JM_ELL4},  {-1, JM_PSI}}},
  {1, {{1, JM_ELL1},  {-1, JM_ELL4}}},
  {1, {{1, JM_ELL2},  {-1, JM_ELL4}}},
  {1, {{1, JM_ELL4},  {-1, JM_PI_C},  {-1, JM_G}}},
  {1, {{1, JM_ELL4},  {+1, JM_PI4},   {-2, JM_PI_C},  {-3, JM_G}}},
  {2, {{1, JM_ELL4},  {-1, JM_W4}}},
};


typedef struct {
  double *longitude;
  double *latitude;
  double *radius;
} sat_amps_t;

static const sat_amps_t sat_i_amps = {
  .longitude = sat_i_long_ampl,
  .latitude = sat_i_lat_ampl,
  .radius = sat_i_rad_ampl
};

static const sat_amps_t sat_ii_amps = {
  .longitude = sat_ii_long_ampl,
  .latitude = sat_ii_lat_ampl,
  .radius = sat_ii_rad_ampl
};

static const sat_amps_t sat_iii_amps = {
  .longitude = sat_iii_long_ampl,
  .latitude = sat_iii_lat_ampl,
  .radius = sat_iii_rad_ampl
};

static const sat_amps_t sat_iv_amps = {
  .longitude = sat_iv_long_ampl,
  .latitude = sat_iv_lat_ampl,
  .radius = sat_iv_rad_ampl
};

typedef struct {
  const trig_param_t *longitude;
  const trig_param_t *latitude;
  const trig_param_t *radius;
} sat_args_t;

static const sat_args_t sat_i_params = {
  .longitude = sat_i_long_params,
  .latitude = sat_i_lat_params,
  .radius = sat_i_rad_params
};

static const sat_args_t sat_ii_params = {
  .longitude = sat_ii_long_params,
  .latitude = sat_ii_lat_params,
  .radius = sat_ii_rad_params
};

static const sat_args_t sat_iii_params = {
  .longitude = sat_iii_long_params,
  .latitude = sat_iii_lat_params,
  .radius = sat_iii_rad_params
};

static const sat_args_t sat_iv_params = {
  .longitude = sat_iv_long_params,
  .latitude = sat_iv_lat_params,
  .radius = sat_iv_rad_params
};

#define ALEN(a) sizeof(a)/sizeof(typeof(a[0]))

typedef struct {
  int n_long_terms;
  int n_lat_terms;
  int n_rad_terms;
  const sat_amps_t *amps;
  const sat_args_t *args;
  double longitude;
  double mean_dist;
} sat_t;

sat_t sats[] = {
  {.n_long_terms = ALEN(sat_i_long_ampl), .n_lat_terms = ALEN(sat_i_lat_ampl),  
   .n_rad_terms = ALEN(sat_i_rad_ampl), 
   .amps = &sat_i_amps, .args = &sat_i_params,
   .mean_dist = 5.90730},

  {.n_long_terms = ALEN(sat_ii_long_ampl), .n_lat_terms = ALEN(sat_ii_lat_ampl),  
   .n_rad_terms = ALEN(sat_ii_rad_ampl), 
   .amps = &sat_ii_amps, .args = &sat_ii_params,
   .mean_dist = 9.39912},

  {.n_long_terms = ALEN(sat_iii_long_ampl), .n_lat_terms = ALEN(sat_iii_lat_ampl),  
   .n_rad_terms = ALEN(sat_iii_rad_ampl), 
   .amps = &sat_iii_amps, .args = &sat_iii_params,
   .mean_dist = 14.99240},

  {.n_long_terms = ALEN(sat_iv_long_ampl), .n_lat_terms = ALEN(sat_iv_lat_ampl),  
   .n_rad_terms = ALEN(sat_iv_rad_ampl), 
   .amps = &sat_iv_amps, .args = &sat_iv_params,
   .mean_dist = 26.36990},   
};


double
principal_inequality(double t)
{
  return 0.33033 * sin(163.679 + 0.0010512 * t)
       + 0.03439 * sin(34.486 - 0.0161731 * t);
}


double gParams[JM_PARAM_COUNT];
void
lieske_init_module(void)
{
  gParams[JM_PI_C] = JUPITER_PERIHELION_LONGITUDE;
  gParams[JM_CONST] = 1.0;
}

double
comp_arg(const trig_param_t *param)
{
  double res = 0.0;
  // Max 4 components in the parameter
  for (int i = 0 ; i < 4 ; i ++) {
    res += param->comps[i].mult * gParams[param->comps[i].param];
  }
  res *= param->mult;
  return res;
}

void
lieske_model_step(cm_orbital_model_t *model, cm_world_t *state)
{
  // We ignore light time here
  double t = state->jde - 2443000.5;

  for (int i = 0 ; i < JM_NORMAL_PARAM_COUNT ; i ++) {
    gParams[i] = param_rules[i].p + param_rules[i].t *t; 
  }

  gParams[JM_G] += principal_inequality(t);
  // For all the sattelites, we must now compute the longitudes, these are used
  // For lat computations
  
  for (int i = 1 ; i < 4 ; i ++) {
    double zk = 0.0;
    for (int j = 0 ; j < sats[i].n_long_terms; j++) {
      double arg = comp_arg(&sats[i].args->longitude[j]);
      zk += sats[i].amps->longitude[j] * sin(arg);
    }
    gParams[JM_Z1+i] = zk; // Needed for latitude comps
    gParams[JM_L1+i] = gParams[JM_ELL1+i] + zk; // Needed for latitude comps
    sats[i].longitude = gParams[JM_ELL1+i] + zk;
  }
}

double3
lieske_e2x3_for_sat(sat_t *sat, double jde)
{
  double longitude = sat->longitude;
  // Latitude
  double B = 0.0;
  for (int i = 0 ; i < sat->n_lat_terms; i++) {
    double arg = comp_arg(&sat->args->latitude[i]);
    B += sat->amps->latitude[i] * sin(arg);
  }

  double R = 0.0;
  for (int i = 0 ; i < sat->n_rad_terms; i++) {
    double arg = comp_arg(&sat->args->radius[i]);
    R += sat->amps->radius[i] * cos(arg);
  }

  R = sat->mean_dist + R;

  double T0 = (jde - CM_B1950_0) / 36525.0;

  // Precession in longitude
  double P = 1.3966626 * T0 + 0.0003088 * T0 * T0;
  longitude += P;
  double psi = gParams[JM_PSI] + P;

  double x = R * cos(longitude - psi) * cos(B);
  double y = R * sin(longitude - psi) * cos(B);
  double z = R * sin(B);

  double T = (jde - CM_B1900_0) / 36525.0;

  // Inclination of jupiters rot axis
  double I = 3.120262 + 0.0006 * T;

  /// We have: tropical longitude, B = eq lat and R = radius (eq jup radius)

  // FIXME
  double omega = cm_long_of_asc_node_at_date(CM_JUPITER, T);
  double phi = psi - omega;

  // Rotate towards orbital plane
  double x1 = x;
  double y1 = y * cos(I) - z * sin(I);
  double z1 = y * sin(I) + z * cos(I);

  // Rotate towards ascending node
  double x2 = x1 * cos(phi) - y1 * sin(phi);
  double y2 = x1 * sin(phi) + y1 * cos(phi);
  double z2 = z1;

  double i = cm_incl_to_ecl_at_date(CM_JUPITER, T);
  // Rotate towards the ecliptic
  double x3 = x2;
  double y3 = y2 * cos(i) - z2 * sin(i);
  double z3 = y2 * sin(i) + z2 * cos(i);

  // Toward vernal equinox
  double x4 = x3 * cos(omega) - y3 * sin(omega);
  double y4 = x3 * sin(omega) + y3 * cos(omega);
  double z4 = z3;

  double3 p4 = {x4, y4, z4};
  return p4;
}

// Stand alone routine, note that it will rerun the models step function every
// time. It is therefore not optimal for repeated computations for all the sat
// positions.
double3
cm_lieske_e2x3(int sat, double jde)
{
  cm_world_t state;
  state.jde = jde;
  lieske_model_step(NULL, &state);

  double3 res = lieske_e2x3_for_sat(&sats[sat], jde);
  return res;
}


void
lieske_object_step(cm_orbit_t *obj, cm_world_t *state)
{
  sat_t *sat = obj->omod_data;


  // We need to do the following transformation but using rectangular
  // coordinates
  //double2 coord = cm_equ_epoch_conv(double ra0, double decl0, T0, CM_J2000_0);

  obj->p = lieske_e2x3_for_sat(sat, state->jde);
}

void
lieske_transform(cm_orbit_t *obj, cm_world_t *state)
{
  
}

void
lieske_object_init(cm_orbit_t *obj)
{
  
}

static cm_orbital_model_t lieske_model = {
  .name = "lieske-e2x3",
  .init_object = lieske_object_init,
  .step_model = lieske_model_step,
  .step_object = lieske_object_step,
  .transform = lieske_transform,
};

void
lieske_e2x3_init(void)
{
  /* code */
  assert(ALEN(sat_i_long_ampl) == ALEN(sat_i_long_params));
  assert(ALEN(sat_ii_long_ampl) == ALEN(sat_ii_long_params));
  assert(ALEN(sat_iii_long_ampl) == ALEN(sat_iii_long_params));
  assert(ALEN(sat_iv_long_ampl) == ALEN(sat_iv_long_params));

  assert(ALEN(sat_i_lat_ampl) == ALEN(sat_i_lat_params));
  assert(ALEN(sat_ii_lat_ampl) == ALEN(sat_ii_lat_params));
  assert(ALEN(sat_iii_lat_ampl) == ALEN(sat_iii_lat_params));
  assert(ALEN(sat_iv_lat_ampl) == ALEN(sat_iv_lat_params));

  assert(ALEN(sat_i_rad_ampl) == ALEN(sat_i_rad_params));
  assert(ALEN(sat_ii_rad_ampl) == ALEN(sat_ii_rad_params));
  assert(ALEN(sat_iii_rad_ampl) == ALEN(sat_iii_rad_params));
  assert(ALEN(sat_iv_rad_ampl) == ALEN(sat_iv_rad_params));

  cm_register_orbital_model(&lieske_model);
}

//CM_INIT {
//  cm_register_orbital_model(&lieske_model);
//}
