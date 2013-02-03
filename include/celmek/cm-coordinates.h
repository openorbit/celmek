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


#ifndef celmek_cm_coordinates_h
#define celmek_cm_coordinates_h

// Base JDEs
#define CM_B1900_0 2415020.3135 //!< Julian date for the bessellian epoch B1900.0
#define CM_B1950_0 2433282.4235 //!< Julian date for the bessellian epoch B1950.0
#define CM_J2000_0 2451545.00   //!< Julian date for the epoch J2000.0
#define CM_J2050_0 2469807.00   //!< Julian date for J2050.0

#define CM_J2000_OBL 23.4392911 //!< Obliquity at epoch J2000
#define CM_B1950_OBL 23.4457889 //!< Obliquity at epoch B1950

// Reference frame for objects
typedef enum {
  CM_ECL_J2000,
  CM_EQU_J2000,
  CM_BODY_EQU,
} cm_reference_frame_t;

/*!
 * Convert equatorial coordinates to ecliptic
 *
 * \param ra Right ascension (rad)
 * \param decl Declination (rad)
 * \param obliq Obliquity (rad) of the body which we have the equatorial
 *              coordinates for.
 * \result Two element vector containing longitude (x) and latitude (y).
 */
double2 cm_equ_to_ecl(double ra, double decl, double obliq);

/*!
 * Convert ecliptic coordinates to equatorial coordinates.
 *
 * \param lon Longitude (rad)
 * \param lat Latitude (rad)
 * \result Two element vector containing right asscention (x) and declination (y)
 *         in radians.
 */
double2 cm_ecl_to_equ(double lon, double lat, double obliq);

/*!
 * Convert equatorial coordinates in one epoch to another.
 *
 * \param ra0 Right asscention (rad)
 * \param decl0 Declination (rad)
 * \param jde0 Julian day (ephemeris time) of the ra and decl values.
 * \param jde Julian day of the target epoch.
 * \result Two element vector of ra and decl in radians corresponding to the
 *         epoch jde.
 */
double2 cm_equ_epoch_conv(double ra0, double decl0, double jde0, double jde);

/*!
 * Convert ecliptic coordinates in one epoch to another.
 *
 * \param lon0 Longitude (rad)
 * \param lat0 Latitude (rad)
 * \param jde0 Julian day (ephemeris time) of the lon and lat values.
 * \param jde Julian day of the target epoch.
 * \result Two element vector of lon and lat in radians corresponding to the
 *         epoch jde.
 */
double2 cm_ecl_epoch_conv(double lon0, double lat0, double jde0, double jde);

/*!
 * Convert spherical coordinates to rectangular coordinates.
 *
 * \param lon Longitude / right asscention in rad
 * \param lat Latitude / declination in rad
 * \param r Distance from centre.
 */
double3 cm_spherical_to_rect(double lon, double lat, double r);

/*!
 * Create a rotation matrix corresponding to the ascending node, inclination and
 * argument of periapsis.
 *
 * \param[out] R Reference to the 3x3 rotation matrix to be created.
 * \param asc_node Ascending node in radians.
 * \param incl Inclination in radians.
 * \param arg_peri Argument of periapsis.
 */
void cm_zxz_rotmatrix(double3x3 R, double asc_node, double incl, double arg_peri);

/*!
 * Create a rotation matrix rotating from the vector v0 to the vector v1.
 *
 * Vectors v0 and v1 form a plane. The function creates a rotation in that plane
 * where the angle of rotation is the angle between the two vectors in that
 * plane.
 *
 * \param[out] R Reference to the 3x3 rotation matrix to be created.
 * \param v0 Vector to rotate from.
 * \param v1 Vector to rotate to.
 */
void cm_vector_rotmatrix(double3x3 R, const double3 v0, const double3 v1);

/*!
 * Create a rotation matrix for a given plane with respect to an equatorial
 * frame.
 *
 * A laplace plane is the average or most stable orbital plane of a body. This
 * is often the same as the equatorial plane. For the big bodies like Jupiter,
 * the satellites often have orbits published based on their laplace planes.
 *
 * \param[out] R Reference to the 3x3 rotation matrix to be created.
 * \param ra Right ascension of the plane's north pole.
 * \param decl Declination of the plane's north pole.
 */
void cm_laplace_frame_rotate(double3x3 R, double ra, double decl);

#endif
