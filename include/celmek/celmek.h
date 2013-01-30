/*
 * Copyright (c) 2012,2013, Mattias Holm <lorrden(at)openorbit.org>.
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

#ifndef celmek_celmek_h
#define celmek_celmek_h


#include <celmek/cm-math.h>

#define CM_TT_TAI_OFFSET         32.184

// See IAU2009 at http://maia.usno.navy.mil/NSFA/IAU2009_consts.html
#define CM_C              299792458.0
#define CM_LG                     6.969290134e-10
#define CM_LB                     1.550519768e-8
#define CM_LC                     1.48082686741e-8
#define CM_TDB0                  -6.55e-5
#define CM_TCB_0               2443144.5003725

#define CM_SEC_PER_JD         86400.0
#define CM_JD_PER_YEAR          365.25
#define CM_JD_PER_CENT        36525.0
#define CM_LIGHT          299792458.0
#define CM_M_PER_AU    149597870700.0

#define CM_MEAN_SIDEREAL_DAY  86164.09054 // s [ 23:56:04.09054 ]
#define CM_SIDEREAL_YEAR        365.25636 //d

#define CM_G__M_KG_S              6.67428e-11 // m3 kg-1 s-2
#define CM_G__M_KG_D              0.498136781 // m3 kg-1 day-2
#define CM_G__AU_KG_D             1.48789531e-34 // au3 kg-1 day-2

#define CM_PREC_IN_LONG        5028.83 // arcsec/Cy
#define CM_OBL_ECL_J2000      84381.412 // arcsec
#define CM_OBL_ECL_J2000_IAU  84381.448 // arcsec

#define CM_SUN_MERCURY_MR   6023600.0       // IAU2009
#define CM_SUN_VENUS_MR      408523.719     // IAU2009
#define CM_SUN_EM_MR         328900.56
#define CM_SUN_MARS_MR      3098703.59      // IAU2009
#define CM_SUN_JUPITER_MR      1047.348644  // IAU2009
#define CM_SUN_SATURN_MR       3497.9018    // IAU2009
#define CM_SUN_URANUS_MR      22902.98      // IAU2009
#define CM_SUN_NEPTUNE_MR     19412.26      // IAU2009
#define CM_SUN_PLUTO_MR           1.36566e8 // IAU2009
#define CM_SUN_ERIS_MR            1.191e8   // IAU2009

#define CM_CERES_SUN_MR           4.72e-10  // IAU2009
#define CM_PALLAS_SUN_MR          1.03e-10  // IAU2009
#define CM_VESTA_SUN_MR           1.35e-10  // IAU2009

#define CM_EM_MR                 81.30059
#define CM_ME_MR                  1.23000371e-2 // IAU2009
#define CM_GM_SUN                 1.32712440018e20 // m3 s-2

#define CM_LIGHT_TIME_PER_AU 499.004783836 //s

#define CM_SUN_GM_TCB   1.32712442099e20
#define CM_SUN_GM_TDB   1.32712440041e20

#define CM_EARTH_GM_TCB   3.986004418e14
#define CM_EARTH_GM_TDB   3.986004356e14
#define CM_EARTH_GM_TT    3.986004415e14

// Common mass constants in kg
#define CM_SUN_MASS
#define CM_MERCURY_MASS 3.2010e23
#define CM_VENUS_MASS   4.1380e24
#define CM_EARTH_MASS   5.9722e24
#define CM_MARS_MASS    6.4273e23
#define CM_JUPITER_MASS 1.89852e27
#define CM_SATURN_MASS  5.6846e26
#define CM_URANUS_MASS  8.6819e25
#define CM_NEPTUNE_MASS 1.02431e26

#define CM_MOON_MASS
#define CM_PHOBOS_MASS
#define CM_DEIMOS_MASS

#define CM_CERES_MASS      0.94e21
#define CM_PLUTO_MASS     13.05e21
#define CM_HAUMEA_MASS     4.01e21
#define CM_MAKEMAKE_MASS   3.0e21
#define CM_ERIS_MASS      16.7e21

/*!
 * Storage type for state vectors.
 */
typedef struct {
  double3 p;
  double3 v;
} cm_state_vectors_t;

/*!
 * Stores osculating elements, we use time of periapsis instead of mean motion
 * since this makes more sense for hyperbolic trajectories.
 */
typedef struct {
  double a;      //!< Semimajor axis
  double e;      //!< Eccentricity
  double i;      //!< Inclination
  double Omega;  //!< Long Ascending node
  double w;      //!< Arg periapsis
  double t_w;    //!< Time of periapsis
  double T;      //!< Period
} cm_orbital_elements_t;

#include <celmek/cm-time.h>
#include <celmek/cm-coordinates.h>
#include <celmek/cm-astro.h>
#include <celmek/cm-model.h>

/*!
 * \brief Initalises the celmek library.
 *
 * Initialises the celmek library (calling module inits, which among other
 * things  will convert tables in degrees to radians).
 * \note MUST BE CALLED BEFORE ANY OTHER FUNCTION.
 */

void cm_init(void);


#endif
