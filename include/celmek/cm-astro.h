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


#ifndef celmek_cm_astro_h
#define celmek_cm_astro_h

#include <stdbool.h>

// Useful physical constants

#define CM_AU_IN_M 149597870700.0
#define CM_SEC_PER_DAY 86400.0

#define CM_MU 6.67384e-11

// Planets
#define CM_GM_SUN_KM 132712440018.0
#define CM_GM_SUN    1.32712440018e20
#define CM_GM_MERCURY_KM 22032.0
#define CM_GM_VENUS_KM 324859.0
#define CM_GM_EARTH_KM 398600.4418// km^3 / s^-2
#define CM_GM_MARS_KM 42828.0
#define CM_GM_JUPITER_KM 126686534.0
#define CM_GM_SATURN_KM 37931187.0
#define CM_GM_URANUS_KM 5793939.0
#define CM_GM_NEPTUNE_KM 6836529.0

// Moons
#define CM_GM_MOON_KM 4902.7779

// Dwarf planets
#define CM_GM_CERES_KM 63.1
#define CM_GM_PLUTO_KM 871.0
#define CM_GM_ERIS_KM 1108.0

// Stable body IDs used by the different methods. This is in no way an
// exhaustive list.

typedef enum {
  CM_SUN,
  CM_MERCURY,
  CM_VENUS,
  CM_EARTH,
  CM_MARS,
  CM_JUPITER,
  CM_SATURN,
  CM_URANUS,
  CM_NEPTUNE,

  CM_CERES,
  CM_PLUTO,
  CM_CHARON,
  CM_ERIS,

  CM_MOON,

  CM_PHOBOS,
  CM_DEIMOS,

  CM_IO,
  CM_EUROPA,
  CM_GANYMEDE,
  CM_CALLISTO,

  CM_TITAN,
  CM_TETHYS,
  CM_DIONE,
  CM_RHEA,
  CM_IAPETUS,
  CM_ENCELADUS,
  CM_HYPERION,
  CM_PHOEBE,
  CM_JANUS,

  CM_TITANIA,
  CM_OBERON,
  CM_ARIEL,
  CM_UMBRIEL,
  CM_MIRANDA,

  CM_TRITON,
  CM_NEREID,
} cm_body_id_t;


void cm_orbital_elements_init(cm_orbital_elements_t *oe);
double cm_semiminor_axis(double semimaj, double ecc);
double cm_period(double semimaj, double GM1, double GM2);
double cm_mean_anomaly(double period, double t_peri, double epoch);
double cm_time_since_periapsis(double mean_anomaly, double mean_motion);
double cm_mean_motion(double period);
double cm_long_periapsis(double arg_peri, double long_asc);
double cm_arg_periapsis(double long_peri, double long_asc);
bool cm_orbital_elements_check(cm_orbital_elements_t *oe);


void cm_mean_orbital_elements_at_date(cm_orbital_elements_t *elems,
                                      cm_body_id_t planet, double T);

double cm_mean_long_at_date(cm_body_id_t planet, double T);

double cm_semimajor_at_date(cm_body_id_t planet, double T);

double cm_eccentricity_at_date(cm_body_id_t planet, double T);

double cm_incl_to_ecl_at_date(cm_body_id_t planet, double T);

double cm_long_of_asc_node_at_date(cm_body_id_t planet, double T);

double cm_long_of_peri_at_date(cm_body_id_t planet, double T);

void cm_mean_orbital_elements_j2000(cm_orbital_elements_t *elems,
                                    cm_body_id_t planet, double T);

double cm_mean_long_j2000(cm_body_id_t planet, double T);

double cm_semimajor_j2000(cm_body_id_t planet, double T);

double cm_eccentricity_j2000(cm_body_id_t planet, double T);

double cm_incl_to_ecl_j2000(cm_body_id_t planet, double T);

double cm_long_of_asc_node_j2000(cm_body_id_t planet, double T);

double cm_long_of_peri_j2000(cm_body_id_t planet, double T);


void cm_state_vector_to_orbital_elements(cm_orbital_elements_t *oe,
                                         const cm_state_vectors_t *sv);


#endif
