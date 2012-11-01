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

#include <math.h>
#include <stdio.h>
#include <string.h>

#include <celmek/celmek.h>

//lambda  geo lonitude
//beta    geo latitude
//delta   geo distance

// Table 45.A
typedef struct {
  double D, M, M_prime, F;
  double zl;
  double zr;
} elp2000_long_dist_term_t;

elp2000_long_dist_term_t long_dist_terms[] = {
  {0.0,  0.0,  1.0,  0.0, 6288774.0, -20905355.0},
  {2.0,  0.0, -1.0,  0.0, 1274027.0,  -3699111.0},
  {2.0,  0.0,  0.0,  0.0,  658314.0,  -2955968.0},
  {0.0,  0.0,  2.0,  0.0,  213618.0,   -569925.0},
  {0.0,  1.0,  0.0,  0.0, -185116.0,     48888.0},
  {0.0,  0.0,  0.0,  2.0, -114332.0,     -3149.0},
  {2.0,  0.0, -2.0,  0.0,   58793.0,    246158.0},
  {2.0, -1.0, -1.0,  0.0,   57066.0,   -152138.0},
  {2.0,  0.0,  1.0,  0.0,   53322.0,   -170733.0},
  {2.0, -1.0,  0.0,  0.0,   45758.0,   -204586.0}, 
  {0.0,  1.0, -1.0,  0.0,  -40923.0,   -129620.0},
  {1.0,  0.0,  0.0,  0.0,  -34720.0,    108743.0},
  {0.0,  1.0,  1.0,  0.0,  -30383.0,    104755.0},
  {2.0,  0.0,  0.0, -2.0,   15327.0,     10321.0},
  {0.0,  0.0,  1.0,  2.0,  -12528.0,         0.0},
  {0.0,  0.0,  1.0, -2.0,   10980.0,     79661.0},
  {4.0,  0.0, -1.0,  0.0,   10675.0,    -34782.0},
  {0.0,  0.0,  3.0,  0.0,   10034.0,    -23210.0},
  {4.0,  0.0, -2.0,  0.0,    8548.0,    -21636.0},
  {2.0,  1.0, -1.0,  0.0,   -7888.0,     24208.0},
  {2.0,  1.0,  0.0,  0.0,   -6766.0,     30824.0},
  {1.0,  0.0, -1.0,  0.0,   -5163.0,     -8379.0},
  {1.0,  1.0,  0.0,  0.0,    4987.0,    -16675.0},
  {2.0, -1.0,  1.0,  0.0,    4036.0,    -12831.0},
  {2.0,  0.0,  2.0,  0.0,    3994.0,    -10445.0}, 
  {4.0,  0.0,  0.0,  0.0,    3861.0,    -11650.0},
  {2.0,  0.0, -3.0,  0.0,    3665.0,     14403.0},
  {0.0,  1.0, -2.0,  0.0,   -2689.0,     -7003.0},
  {2.0,  0.0, -1.0,  2.0,   -2602.0,         0.0},
  {2.0, -1.0, -2.0,  0.0,    2390.0,     10056.0},
  {1.0,  0.0,  1.0,  0.0,   -2348.0,      6322.0},
  {2.0, -2.0,  0.0,  0.0,    2236.0,     -9884.0},
        
  {0.0,  1.0,  2.0,  0.0,   -2120.0,      5751.0},
  {0.0,  2.0,  0.0,  0.0,   -2069.0,         0.0}, 
  {2.0, -2.0, -1.0,  0.0,    2048.0,     -4950.0},
  {2.0,  0.0,  1.0, -2.0,   -1773.0,      4130.0},
  {2.0,  0.0,  0.0,  2.0,   -1595.0,         0.0},
  {4.0, -1.0, -1.0,  0.0,    1215.0,     -3958.0},
  {0.0,  0.0,  2.0,  2.0,   -1110.0,         0.0},
  {3.0,  0.0, -1.0,  0.0,    -892.0,      3258.0},
  {2.0,  1.0,  1.0,  0.0,    -810.0,      2616.0},
  {4.0, -1.0, -2.0,  0.0,     759.0,     -1897.0},
  {0.0,  2.0, -1.0,  0.0,    -713.0,     -2117.0},
  {2.0,  2.0, -1.0,  0.0,    -700.0,      2354.0},
  {2.0,  1.0, -2.0,  0.0,     691.0,         0.0},
  {2.0, -1.0,  0.0, -2.0,     596.0,         0.0},
  {4.0,  0.0,  1.0,  0.0,     549.0,     -1423.0},
  {0.0,  0.0,  4.0,  0.0,     537.0,     -1117.0},
  {4.0, -1.0,  0.0,  0.0,     520.0,     -1571.0},
  {1.0,  0.0, -2.0,  0.0,    -487.0,     -1739.0},
  {2.0,  1.0,  0.0, -2.0,    -399.0,         0.0},
  {0.0,  0.0,  2.0, -2.0,    -381.0,     -4421.0},
  {1.0,  1.0,  1.0,  0.0,     351.0,         0.0},
  {3.0,  0.0, -2.0,  0.0,    -340.0,         0.0},
  {4.0,  0.0, -3.0,  0.0,     330.0,         0.0},
  {2.0, -1.0,  2.0,  0.0,     327.0,         0.0},
  {0.0,  2.0,  1.0,  0.0,    -323.0,      1165.0},
  {1.0,  1.0, -1.0,  0.0,     299.0,         0.0},
  {2.0,  0.0,  3.0,  0.0,     294.0,         0.0},
  {2.0,  0.0, -1.0, -2.0,       0.0,      8752.0},
};

// Table 45.B
typedef struct {
  double D, M, M_prime, F;
  double zb;
} elp2000_lat_term_t;

elp2000_lat_term_t lat_terms[] = {
  {0.0,  0.0,  0.0,  1.0, 5128122.0},
  {0.0,  0.0,  1.0,  1.0,  280602.0},
  {0.0,  0.0,  1.0, -1.0,  277693.0},
  {2.0,  0.0,  0.0, -1.0,  173237.0},
  {2.0,  0.0, -1.0,  1.0,   55413.0},
  {2.0,  0.0, -1.0, -1.0,   46271.0},
  {2.0,  0.0,  0.0,  1.0,   32573.0},
  {0.0,  0.0,  2.0,  1.0,   17198.0},
  {2.0,  0.0,  1.0, -1.0,    9266.0},
  {0.0,  0.0,  2.0, -1.0,    8822.0},
  {2.0, -1.0,  0.0, -1.0,    8216.0},
  {2.0,  0.0, -2.0, -1.0,    4324.0},
  {2.0,  0.0,  1.0,  1.0,    4200.0},
  {2.0,  1.0,  0.0, -1.0,   -3359.0},
  {2.0, -1.0, -1.0,  1.0,    2463.0},
  {2.0, -1.0,  0.0,  1.0,    2211.0},
  {2.0, -1.0, -1.0, -1.0,    2065.0},
  {0.0,  1.0, -1.0, -1.0,   -1870.0},
  {4.0,  0.0, -1.0, -1.0,    1828.0},
  {0.0,  1.0,  0.0,  1.0,   -1794.0},
  {0.0,  0.0,  0.0,  3.0,   -1749.0},
  {0.0,  1.0, -1.0,  1.0,   -1565.0},
  {1.0,  0.0,  0.0,  1.0,   -1491.0},
  {0.0,  1.0,  1.0,  1.0,   -1475.0},
  {0.0,  1.0,  1.0, -1.0,   -1410.0},
  {0.0,  1.0,  0.0, -1.0,   -1344.0},
  {1.0,  0.0,  0.0, -1.0,   -1335.0},
  {0.0,  0.0,  3.0,  1.0,    1107.0},
  {4.0,  0.0,  0.0, -1.0,    1021.0},
  {4.0,  0.0, -1.0,  1.0,     833.0},

  {0.0,  0.0,  1.0, -3.0,     777.0},
  {4.0,  0.0, -2.0,  1.0,     671.0},
  {2.0,  0.0,  0.0, -3.0,     607.0},
  {2.0,  0.0,  2.0, -1.0,     596.0},
  {2.0, -1.0,  1.0, -1.0,     491.0},
  {2.0,  0.0, -2.0,  1.0,    -451.0},
  {0.0,  0.0,  3.0, -1.0,     439.0},
  {2.0,  0.0,  2.0,  1.0,     422.0},
  {2.0,  0.0, -3.0, -1.0,     421.0},
  {2.0,  1.0, -1.0,  1.0,    -366.0},
  {2.0,  1.0,  0.0,  1.0,    -351.0},
  {4.0,  0.0,  0.0,  1.0,     331.0},
  {2.0, -1.0,  1.0,  1.0,     315.0},
  {2.0, -2.0,  0.0, -1.0,     302.0},
  {0.0,  0.0,  1.0,  3.0,    -283.0},
  {2.0,  1.0,  1.0, -1.0,    -229.0},
  {1.0,  1.0,  0.0, -1.0,     223.0},
  {1.0,  1.0,  0.0,  1.0,     223.0},
  {0.0,  1.0, -2.0, -1.0,    -220.0},
  {2.0,  1.0, -1.0, -1.0,    -220.0},
  {1.0,  0.0,  1.0,  1.0,    -185.0},
  {2.0, -1.0, -2.0, -1.0,     181.0},
  {0.0,  1.0,  2.0,  1.0,    -177.0},
  {4.0,  0.0, -2.0, -1.0,     176.0},
  {4.0, -1.0, -1.0, -1.0,     166.0},
  {1.0,  0.0,  1.0, -1.0,    -164.0},
  {4.0,  0.0,  1.0, -1.0,     132.0},
  {1.0,  0.0, -1.0, -1.0,    -119.0},
  {4.0, -1.0,  0.0, -1.0,     115.0},
  {2.0, -2.0,  0.0,  1.0,     107.0},
};





// Note JDE not same as jd
// TODO: Optimize by replacing all degree terms with radians
static inline double
clamp_degs(double a)
{
  return fmod(fmod(a, 2.0*M_PI) + 2.0*M_PI, 2.0*M_PI);
}

// Broken out of main routine for testability purposes
static inline double
cm_elp2000_82b_T(double jde)
{
  return (jde - 2451545.0) / 36525.0;
}


#define ELP_218_3164591_RAD         deg2rad(218.3164591)
#define ELP_481267_88134236_RAD     deg2rad(481267.88134236)
#define ELP_0_0013268_RAD           deg2rad(0.0013268)
#define ELP_RCP_538841_0_RAD        deg2rad(1.0/538841.0)
#define ELP_RCP_65194000_0_RAD      deg2rad(1.0/65194000.0)

static inline double
cm_elp2000_82b_Lprime(double T)
{
  double Lp = ELP_218_3164591_RAD + ELP_481267_88134236_RAD * T
  - ELP_0_0013268_RAD * T * T
  + T * T * T * ELP_RCP_538841_0_RAD
  - T * T * T * T * ELP_RCP_65194000_0_RAD;
  return clamp_degs(Lp);
}

#define ELP_297_8502042_RAD     deg2rad(297.8502042)
#define ELP_445267_1115168_RAD  deg2rad(445267.1115168)
#define ELP_0_0016300_RAD       deg2rad(0.0016300)
#define ELP_RCP_545868_0_RAD    deg2rad(1.0/545868.0)
#define ELP_RCP_113065000_0_RAD deg2rad(1.0/113065000.0)


static inline double
cm_elp2000_82b_D(double T)
{
  double D = ELP_297_8502042_RAD + ELP_445267_1115168_RAD * T
  - ELP_0_0016300_RAD * T * T
  + T * T * T * ELP_RCP_545868_0_RAD
  - T * T * T * T * ELP_RCP_113065000_0_RAD;
  return clamp_degs(D);
}

#define ELP_357_5291092_RAD     deg2rad(357.5291092)
#define ELP_35999_0502909_RAD   deg2rad(35999.0502909)
#define ELP_0_0001536_RAD       deg2rad(0.0001536)
#define ELP_RCP_24490000_0_RAD  deg2rad(1.0/24490000.0)

static inline double
cm_elp2000_82b_M(double T)
{
  double M = ELP_357_5291092_RAD + ELP_35999_0502909_RAD * T
  - ELP_0_0001536_RAD * T * T
  + T * T * T * ELP_RCP_24490000_0_RAD;
  return clamp_degs(M);

}

#define ELP_134_9634114_RAD     deg2rad(134.9634114)
#define ELP_477198_8676313_RAD  deg2rad(477198.8676313)
#define ELP_0_0089970_RAD       deg2rad(0.0089970)
#define ELP_RCP_69699_0_RAD     deg2rad(1.0/69699.0)
#define ELP_RCP_14712000_0_RAD  deg2rad(1.0/14712000.0)

static inline double
cm_elp2000_82b_Mprime(double T)
{
  double Mp = ELP_134_9634114_RAD + ELP_477198_8676313_RAD * T
  + ELP_0_0089970_RAD * T * T
  + T * T * T * ELP_RCP_69699_0_RAD
  - T * T * T * T * ELP_RCP_14712000_0_RAD;
  return clamp_degs(Mp);

}


#define ELP_93_2720993_RAD      deg2rad(93.2720993)
#define ELP_483202_0175273_RAD  deg2rad(483202.0175273)
#define ELP_0_0034029_RAD       deg2rad(0.0034029)
#define ELP_RCP_3526000_0_RAD   deg2rad(1.0/3526000.0)
#define ELP_RCP_863310000_0_RAD deg2rad(1.0/863310000.0)

static inline double
cm_elp2000_82b_F(double T)
{
  double F = ELP_93_2720993_RAD + ELP_483202_0175273_RAD * T
  - ELP_0_0034029_RAD * T * T
  - T * T * T * ELP_RCP_3526000_0_RAD
  + T * T * T * T * ELP_RCP_863310000_0_RAD;
  return clamp_degs(F);

}

#define ELP_119_75_RAD  deg2rad(119.75)
#define ELP_131_849_RAD deg2rad(131.849)

static inline double
cm_elp2000_82b_A1(double T) {
  double A1 = ELP_119_75_RAD + ELP_131_849_RAD * T;
  return clamp_degs(A1);
}

#define ELP_53_09_RAD       deg2rad(53.09)
#define ELP_479264_290_RAD  deg2rad(479264.290)

static inline double
cm_elp2000_82b_A2(double T) {
  double A2 = ELP_53_09_RAD + ELP_479264_290_RAD * T;
  return clamp_degs(A2);
}

#define ELP_313_45_RAD      deg2rad(313.45)
#define ELP_481266_484_RAD  deg2rad(481266.484)

static inline double
cm_elp2000_82b_A3(double T) {
  double A3 = ELP_313_45_RAD + ELP_481266_484_RAD * T;
  return clamp_degs(A3);
}

static inline double
cm_elp2000_82b_E(double T) {
  return 1.0 - 0.002516 * T - 0.0000074 * T * T;
}



#define ELP_3958_0_RAD deg2rad(3958.0*1.e-6)
#define ELP_1962_0_RAD deg2rad(1962.0*1.e-6)
#define ELP_318_0_RAD  deg2rad( 318.0*1.e-6)
#define ELP_2235_0_RAD deg2rad(2235.0*1.e-6)
#define ELP_382_0_RAD  deg2rad( 382.0*1.e-6)
#define ELP_175_0_RAD  deg2rad( 175.0*1.e-6)
#define ELP_127_0_RAD  deg2rad( 127.0*1.e-6)
#define ELP_115_0_RAD  deg2rad( 115.0*1.e-6)

// Note JDE not same as jd
// TODO: Optimize by replacing all degree terms with radians

double3
cm_elp2000_82b(double jde)
{
  double T = cm_elp2000_82b_T(jde);
  
  // Compute parameters in degrees.
  // Mean longitude . mean equinox of the date, incl light delay
  double Lprime = cm_elp2000_82b_Lprime(T);
  // Mean elongation
  double D = cm_elp2000_82b_D(T);
  // Sun's mean anomaly
  double M = cm_elp2000_82b_M(T);

  // Moon's mean anomaly
  double Mprime = cm_elp2000_82b_Mprime(T);
  // Moon's argument of latitude
  double F = cm_elp2000_82b_F(T);
    
  double A1 = cm_elp2000_82b_A1(T);
  double A2 = cm_elp2000_82b_A2(T);
  double A3 = cm_elp2000_82b_A3(T);

  // Decreasing eccentricity of earth orbit around sun
  double E = cm_elp2000_82b_E(T);
  
  double zl = 0.0;
  double zb = 0.0;
  double zr = 0.0;
  
  for (int i = 0 ; i < sizeof(long_dist_terms)/sizeof(elp2000_long_dist_term_t) ; i ++) {
    double arg = long_dist_terms[i].D * D;
    arg += long_dist_terms[i].M * M;
    arg += long_dist_terms[i].M_prime * Mprime;
    arg += long_dist_terms[i].F * F;
    arg = fmod(fmod(arg, 2.0*M_PI) + 2.0*M_PI, 2.0*M_PI);

    double Eprime = 1.0;
    if (fabs(long_dist_terms[i].M) == 2.0) Eprime = E * E;
    else if (fabs(long_dist_terms[i].M) == 1.0) Eprime = E;
    
    zl += long_dist_terms[i].zl * sin(arg) * Eprime;
    zr += long_dist_terms[i].zr * cos(arg) * Eprime;
  }

  for (int i = 0 ; i < sizeof(lat_terms)/sizeof(elp2000_lat_term_t) ; i++) {
    double arg = lat_terms[i].D * D;
    arg += lat_terms[i].M * M;
    arg += lat_terms[i].M_prime * Mprime;
    arg += lat_terms[i].F * F;

    double Eprime = 1.0;
    if (fabs(lat_terms[i].M) == 2.0) Eprime = E * E;
    else if (fabs(lat_terms[i].M) == 1.0) Eprime = E;
    arg = fmod(fmod(arg, 2.0*M_PI) + 2.0*M_PI, 2.0*M_PI);
    zb += lat_terms[i].zb * sin(arg) * Eprime;
  }

  zl += ELP_3958_0_RAD * sin(A1);
  zl += ELP_1962_0_RAD * sin(Lprime - F);    // Flattening
  zl += ELP_318_0_RAD * sin(A2);             // Jupiter

  zb -= ELP_2235_0_RAD * sin(Lprime);        // Flattening
  zb += ELP_382_0_RAD * sin(A3);
  zb += ELP_175_0_RAD * sin(A1-F);           // Venus
  zb += ELP_175_0_RAD * sin(A1+F);           // Venus
  zb += ELP_127_0_RAD * sin(Lprime-Mprime);  // Flattening
  zb -= ELP_115_0_RAD * sin(Lprime+Mprime);  // Flattening
  
  // To avoid divisions, the table cooefs need to be updated
  double lambda = Lprime + zl;// / 1000000.0;  // Degrees
  double beta = zb;// / 1000000.0;             // Degrees
  double delta = 385000.56e3 + zr;///1000.0;//  / 1000.0;   // km

//  printf("Zl = %f should be -1127527 +/- epsilon\n", zl); // -1127527
//  printf("Zb = %f should be -3229127 +/- epsilon\n", zb); // -3229127
//  printf("Zr = %f should be -16590875 +/- epsilon\n", zr); // -16590875

  //printf("%f %f %f\n", lambda, beta, delta);
  double3 res = {fmod(lambda, 2.0*M_PI), fmod(beta, M_PI), delta};
  
  
  return res;
}

void
elp2000_model_step(cm_orbital_model_t *model, cm_world_t *state)
{
  
}

void
elp2000_object_step(cm_orbit_t *obj, cm_world_t *state)
{
  double3 res = cm_elp2000_82b(state->jde);
  // Elp returns the results in ecliptic, we convert it to equ here
  // this is roughly what the ICRF is.
  double2 res2 = cm_ecl_to_equ(res.x, res.y, CM_J2000_OBL);
  
  // We should express everything in ICRF this is roughly that
  obj->p = cm_spherical_to_rect(res2.x, res2.y, res.z);
}

void
elp2000_transform(cm_orbit_t *obj, cm_world_t *state)
{
  // ELP computes the position in geo-centric coordinates
  obj->p = v3d_add(obj->p, obj->parent->p);
}

void
elp2000_object_init(cm_orbit_t *obj)
{
  if (!strcmp(obj->name, "moon")) {
    // Do we need to do anything?
  } else {
    // TODO: ERROR
  }
}

static cm_orbital_model_t elp2000_model = {
  .name = "elp2000-82b",
  .init_object = elp2000_object_init,
  .step_model = elp2000_model_step,
  .step_object = elp2000_object_step,
  .transform = elp2000_transform,
};

void
elp2000_82b_init(void)
{
  // Tables are identical to MEEUS, we need to update them to use proper units
  for (int i = 0 ; i < sizeof(long_dist_terms)/sizeof(elp2000_long_dist_term_t) ; i ++) {
    long_dist_terms[i].zl *= 1e-6; // To degrees
    long_dist_terms[i].zl = deg2rad(long_dist_terms[i].zl); // To rads
  }
  for (int i = 0 ; i < sizeof(lat_terms)/sizeof(elp2000_lat_term_t) ; i++) {
    lat_terms[i].zb *= 1e-6; // To degrees
    lat_terms[i].zb = deg2rad(lat_terms[i].zb); // And to rads
  }

  cm_register_orbital_model(&elp2000_model);
}

//CM_INIT {
//  cm_register_orbital_model(&elp2000_model);
//}
