/*
 * Copyright (c) 2012, Mattias Holm <lorrden(at)openorbit.org>.
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

/*
  Pluto orbit, valid 1885 to 2099
*/
#include <stdio.h>
#include <math.h>
#include <celmek/celmek.h>

typedef struct {
  double j, s, p;
  double long_a, long_b;
  double lat_a, lat_b;
  double rad_a, rad_b;
} goffin_term_t;

goffin_term_t terms[] = {
  {0,  0,  1, -19798886, 19848454, -5453098, -14974876,  66867334, 68955876},
  {0,  0,  2,    897499, -4955707,  3527363,   1672673, -11826086,  -333765},
  {0,  0,  3,    610820,  1210521, -1050939,    327763,   1593657, -1439953},
  {0,  0,  4,   -341639,  -189719,   178691,   -291925,    -18948,   482443},
  {0,  0,  5,    129027,   -34863,    18763,    100448,    -66634,   -85576},
  {0,  0,  6,    -38215,    31061,   -30594,    -25838,     30841,    -5765},
  {0,  1, -1,     20349,    -9886,     4965,     11263,     -6140,    22254},
  {0,  1,  0,     -4045,    -4904,      310,      -132,      4434,     4443},
  {0,  1,  1,     -5885,    -3238,     2036,      -947,     -1518,      641},
  {0,  1,  2,     -3812,     3011,       -2,      -674,        -5,      792},
  {0,  1,  3,      -601,     3468,     -329,      -563,       518,      518},
  {0,  2, -2,      1237,      463,      -64,        39,       -13,     -221},
  {0,  2, -1,      1086,     -911,      -94,       210,       837,     -494},
  {0,  2,  0,       595,    -1229,       -8,      -160,      -281,      616},
  {1, -1,  0,      2484,     -485,     -177,       259,       260,     -395},
  {1, -1,  1,       839,    -1414,       17,       234,      -191,     -396},
  {1,  0, -3,      -964,     1059,      582,      -285,     -3218,      370},
  {1,  0, -2,     -2303,    -1038,     -298,       692,      8019,    -7869},
  {1,  0, -1,      7049,      747,      157,       201,       105,    45637},
  {1,  0,  0,      1179,     -358,      304,       825,      8623,     8444},
  {1,  0,  1,       393,      -63,     -124,       -29,      -896,     -801},
  {1,  0,  2,       111,     -268,       15,         8,       208,     -122},
  {1,  0,  3,       -52,     -154,        7,        15,      -133,       65},
  {1,  0,  4,       -78,      -30,        2,         2,       -16,        1},
  {1,  1, -3,       -34,      -26,        4,         2,       -22,        7},
  {1,  1, -2,       -43,        1,        3,         0,        -8,       16},
  {1,  1, -1,       -15,       21,        1,        -1,         2,        9},
  {1,  1,  0,        -1,       15,        0,        -2,        12,        5},
  {1,  1,  1,         4,        7,        1,         0,         1,       -3},
  {1,  1,  3,         1,        5,        1,        -1,         1,        0},
  {2,  0, -6,         8,        3,       -2,        -3,         9,        5},
  {2,  0, -5,        -3,        6,        1,         2,         2,       -1},
  {2,  0, -4,         6,      -13,       -8,         2,        14,       10},
  {2,  0, -3,        10,       22,       10,        -7,       -65,       12},
  {2,  0, -2,       -57,      -32,        0,        21,       126,     -233},
  {2,  0, -1,       157,      -46,        8,         5,       270,     1068},
  {2,  0,  0,        12,      -18,       13,        16,       254,      155},
  {2,  0,  1,        -4,        8,       -2,        -3,       -26,       -2},
  {2,  0,  2,        -5,        0,        0,         0,         7,        0},
  {2,  0,  3,         3,        4,        0,         1,       -11,        4},
  {3,  0, -2,        -1,       -1,        0,         1,         4,      -14},
  {3,  0, -1,         6,       -3,        0,         0,        18,       35},
  {3,  0,  0,        -1,       -2,        0,         1,        13,        3},
};

#define PLUTO_34_35_RAD     deg2rad(  34.35)
#define PLUTO_3034_9057_RAD deg2rad(3034.9057)
#define PLUTO_50_08_RAD     deg2rad(  50.08)
#define PLUTO_1222_1138_RAD deg2rad(1222.1138)
#define PLUTO_238_96_RAD    deg2rad( 238.96)
#define PLUTO_144_9600_RAD  deg2rad( 144.9600)

#define PLUTO_238_956785_RAD deg2rad(238.956785)
#define PLUTO_144_96_RAD     deg2rad(144.96)
#define PLUTO_3_908202_RAD   deg2rad(  3.908202)


double3
cm_goffin2000(double jde)
{
  double T = (jde - 2451545.0) / 36525.0;
  double J =  PLUTO_34_35_RAD + PLUTO_3034_9057_RAD * T;
  double S =  PLUTO_50_08_RAD + PLUTO_1222_1138_RAD * T;
  double P =  PLUTO_238_96_RAD +  PLUTO_144_9600_RAD * T;

  //printf("T = %f\n", T);
  //printf("J = %f\n", J);
  //printf("S = %f\n", S);
  //printf("P = %f\n", P);

  double longitude = 0.0, latitude = 0.0, rad = 0.0;

  for (int i = 0 ; i < sizeof(terms)/sizeof(goffin_term_t) ; i ++) {
    double arg = terms[i].j * J + terms[i].s * S + terms[i].p * P;

    longitude += terms[i].long_a * sin(arg) + terms[i].long_b * cos(arg);
    latitude += terms[i].lat_a * sin(arg) + terms[i].lat_b * cos(arg);
    rad += terms[i].rad_a * sin(arg) + terms[i].rad_b * cos(arg);
  }
  //printf("sums: %f %f %f\n", longitude, latitude, rad);

  longitude += PLUTO_238_956785_RAD + PLUTO_144_96_RAD * T;
  latitude -= PLUTO_3_908202_RAD;
  rad += 40.7247248;

  // We now have heliocentric equatorial coordinates for J2000
  // We need to convert these to rectangular coordinates. One problem is that
  // This method results in heliocentric coordinates, these need to be
  // translated by the sun

  // TODO: To barycentric
  double3 p = {longitude, latitude, rad};
  return p;
}

void
pluto_model_step(cm_orbital_model_t *model, cm_world_t *state)
{
}

void
pluto_object_step(cm_orbit_t *obj, cm_world_t *state)
{
  double3 sp = cm_goffin2000(state->jde);
  double2 eq = cm_ecl_to_equ(sp.x, sp.y, CM_J2000_OBL_DEG*VMATH_RAD_PER_DEG);

  double3 rp = cm_spherical_to_rect(eq.x, eq.y, sp.z);

  rp.x *= CM_AU_IN_M;
  rp.y *= CM_AU_IN_M;
  rp.z *= CM_AU_IN_M;

  obj->p = rp;
}

void
pluto_transform(cm_orbit_t *obj, cm_world_t *state)
{
  // At this point, all bodies have had their positions calculated, so we
  // convert the heliocentric position as given to solar system barycentric
  // This is trivial, we simply add the sol position to the position of pluto
  obj->p = vd3_add(obj->p, obj->parent->p);
}

void
pluto_object_init(cm_orbit_t *obj)
{

}

static cm_orbital_model_t pluto_model = {
  .name = "pluto",
  .init_object = pluto_object_init,
  .step_model = pluto_model_step,
  .step_object = pluto_object_step,
  .transform = pluto_transform,
};

void
goffin_j2000_init(void)
{
  // Adjust table scales and convert degrees to radians.
  for (int i = 0 ; i < sizeof(terms)/sizeof(goffin_term_t) ; i ++) {
    terms[i].long_a *= 1.e-6;
    terms[i].long_b *= 1.e-6;
    terms[i].long_a = deg2rad(terms[i].long_a);
    terms[i].long_b = deg2rad(terms[i].long_b);
    terms[i].lat_a *= 1.e-6;
    terms[i].lat_b *= 1.e-6;
    terms[i].lat_a = deg2rad(terms[i].lat_a);
    terms[i].lat_b = deg2rad(terms[i].lat_b);
    terms[i].rad_a *= 1.e-7;
    terms[i].rad_b *= 1.e-7;
  }

  cm_register_orbital_model(&pluto_model);
}
