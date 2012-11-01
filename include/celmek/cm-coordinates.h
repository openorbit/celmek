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


#ifndef celmek_cm_coordinates_h
#define celmek_cm_coordinates_h

// Base JDEs
#define CM_B1900_0 2415020.3135
#define CM_B1950_0 2433282.4235
#define CM_J2000_0 2451545.00
#define CM_J2050_0 2469807.00

#define CM_J2000_OBL 23.4392911
#define CM_B1950_OBL 23.4457889

// Reference frame for objects
typedef enum {
  CM_ECL_J2000,
  CM_EQU_J2000,
  CM_BODY_EQU,
} cm_reference_frame_t;

double2 cm_equ_to_ecl(double ra, double decl, double obliq);
double2 cm_ecl_to_equ(double lon, double lat, double obliq);
double2 cm_equ_epoch_conv(double ra0, double decl0, double jde0, double jde);
double2 cm_ecl_epoch_conv(double lon0, double lat0, double jde0, double jde);

double3 cm_spherical_to_rect(double lon, double lat, double r);
void cm_zxz_rotmatrix(double3x3 R, double asc_node, double incl, double arg_peri);

void cm_vector_rotmatrix(double3x3 R, const double3 v0, const double3 v1);
void cm_laplace_frame_rotate(double3x3 R, double ra, double decl);

#endif
