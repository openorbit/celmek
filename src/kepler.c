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

#include <sys/time.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

#include <celmek/celmek.h>


/*
 earth:
 semiMajor: 1.00000261 au
 149598261 km
 period: 365.256363004 days
 mean anomaly: 357.51716
 inclination: 7.155 sun eq
 1.57869 invariable plane
 0 ecliptic
 long asc: 348.73936
 arg peri: 114.20783
 */
/*
 jupiter:
 semiMajor: 5.204267 au
 778547200 km
 ecc: 0.048775
 period: 4332.59 days
 mean anomaly: 18.818
 inclination: 6.09 sun eq
 0.32 invariable plane
 1.305 ecliptic
 long asc: 100.492
 arg peri: 275.066
 */


typedef enum {
  KEPLER_PERIOD,
  KEPLER_SEMI_MAJOR,
  KEPLER_ECCENTRICITY,
  KEPLER_INCLINATION,
  KEPLER_LONGITUDE_OF_ASCENDING_NODE,
  KEPLER_ARGUMENT_OF_PERIAPSIS,
  KEPLER_MEAN_ANOMALY,

  KEPLER_BASE_PLANE_RA,
  KEPLER_BASE_PLANE_DEC,
} kepler_param_t;


typedef struct {
  double period;
  // Orbital elements
  double eccentricity;
  double semi_major;
  double semi_minor;
  double inclination;
  double longitude_of_ascending_node;
  double argument_of_periapsis;
  double mean_anomaly;
  double mean_anomaly_at_epoch;
  double time_of_periapsis;
  double semi_latus_rectum;

  double3x3 R; // Rotation matrix
} cm_elliptical_orbit_t;


double
cm_orbit_mean_anomaly(const cm_elliptical_orbit_t *orbit, double t)
{
  return 2.0 * M_PI * t / orbit->period;
}

double cutoff = 0.5;

// TODO: Fix iteration counts
double
cm_orbit_eccentric_anomaly(const cm_elliptical_orbit_t *orbit, double M)
{  
  // M = E - ecc sin E
  double e = orbit->eccentricity;
  double res = 0.0;
  double accuracy = 0.000000001;
  
  // TODO: explore the most efficient value of e for method selection
  if (e < cutoff) {
    double E0, E1;
    E0 = E1 = M;
   int i = 0;
    do {
      E0 = E1;
      E1 = M + e * sin(E0);
      i++;
    } while(fabs(E1 - E0) > accuracy/* && i < 100*/);
    //printf("fast model accuracy: %f, iters = %d\n", fabs(E1 - E0), i);
    //printf("accuracy: %f\n", fabs(E1 - E0));
    res = E1;
  } else {
    int i = 0;
    double E0, E1;
    E0 = E1 = M;
    do {
      E0 = E1;
      E1 = E0 + (M + e * sin(E0) - E0) / (1.0 - e * cos(E0));
      i++;
    } while(fabs(E1 - E0) > accuracy /*&& i < 100*/);
    //printf("slow model accuracy: %f, iters = %d\n", fabs(E1 - E0), i);
    res = E1;
  }
  
  return res;
}

double
cm_orbit_true_anomaly(const cm_elliptical_orbit_t *orbit, double E)
{
  double e = orbit->eccentricity;
  double a = sqrt((1.0 + e)/(1.0 - e)) * tan(E/2.0);
  return atan(a) * 2.0;
}

// Time in unit for orbital parameter
double3
cm_orbit_positon_at_offset(const cm_elliptical_orbit_t *orbit, double t)
{
  double e = orbit->eccentricity;
  double M = cm_orbit_mean_anomaly(orbit, t);
  double E = cm_orbit_eccentric_anomaly(orbit, M);
  double fi = cm_orbit_true_anomaly(orbit, E);

  double r = orbit->semi_major * (1.0-e*e)/(1.0+e*cos(fi));

  // We now have the angle fi and r, so we can assign a position to the orbit
  // in the elipse plane. This position is not yet rotated by decl, incl and
  // ascending node
  double3 p;
  p.x = -r * sin(fi); // Viewed from above, right handed rotation
  p.y =  r * cos(fi); // Y positive towards periapsis
  p.z =  0.0;

  return p;
}

cm_elliptical_orbit_t*
cm_new_elliptical_orbit(double T, double semi_major, double ecc,
                        double incl, double lon_asc,
                        double arg_peri, double mean_anomaly)
{
  cm_elliptical_orbit_t *orbit = malloc(sizeof(cm_elliptical_orbit_t));

  // Period is sqrt((4 * M_PI^2 / G(M+m)) * a^3), we do not have access to
  // Masses here (we shouldn't, so we need the T as extra param, but the
  // caller must create this)
	orbit->period = T;

  // Orbitl elements
  orbit->eccentricity = ecc;
  orbit->semi_major = semi_major;
  orbit->semi_minor = semi_major * sqrt(1.0 - ecc * ecc);
  orbit->inclination = incl;
  orbit->longitude_of_ascending_node = lon_asc;
  orbit->argument_of_periapsis = arg_peri;
  orbit->mean_anomaly = mean_anomaly;

  cm_zxz_rotmatrix(orbit->R, lon_asc, incl, arg_peri);

  return orbit;
}

static void
kepler_object_init(cm_orbit_t *orbit)
{
  orbit->omod_data
    = cm_new_elliptical_orbit(orbit->oparams[KEPLER_PERIOD],
                              orbit->oparams[KEPLER_SEMI_MAJOR],
                              orbit->oparams[KEPLER_ECCENTRICITY],
                              orbit->oparams[KEPLER_INCLINATION],
                              orbit->oparams[KEPLER_LONGITUDE_OF_ASCENDING_NODE],
                              orbit->oparams[KEPLER_ARGUMENT_OF_PERIAPSIS],
                              orbit->oparams[KEPLER_MEAN_ANOMALY]);

  double3x3 lrot;
  cm_laplace_frame_rotate(lrot,
                          orbit->oparams[KEPLER_BASE_PLANE_RA],
                          orbit->oparams[KEPLER_BASE_PLANE_DEC]);

  cm_elliptical_orbit_t *eo = orbit->omod_data;
  m3m_mul(eo->R, eo->R, lrot);
}

static void
kepler_model_step(cm_orbital_model_t *model, cm_world_t *state)
{
  
}

static void
kepler_object_step(cm_orbit_t *orbit, cm_world_t *state)
{
  cm_elliptical_orbit_t *ellip = orbit->omod_data;
  double3 p
    = cm_orbit_positon_at_offset(ellip,
                                 (state->jde - CM_J2000_0)/ellip->period);
  
  orbit->p = m3vd_mul(ellip->R, p);
  orbit->p = p;
}

static void
kepler_transform(cm_orbit_t *orbit, cm_world_t *state)
{
  
}

static cm_orbital_model_t kepler_model = {
  .name = "kepler",
  .init_object = kepler_object_init,
  .step_model = kepler_model_step,
  .step_object = kepler_object_step,
  .transform = kepler_transform,
};

void
kepler_init(void)
{
  cm_register_orbital_model(&kepler_model);
}
//CM_INIT {
//  cm_register_orbital_model(&kepler_model);
//}
