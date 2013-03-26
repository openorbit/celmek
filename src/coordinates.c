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
#include <celmek/celmek.h>

double2
cm_equ_to_ecl(double ra, double decl, double obliq)
{
  double t_l_u = (sin(ra) * cos(obliq) + tan(decl) * sin(obliq));// /cos(ra);
  double s_b = sin(decl) * cos(obliq) - cos(decl) * sin(obliq) * sin(ra);
  double2 res;
  res.x = atan2(t_l_u, cos(ra));
  res.y = asin(s_b);
  
  return res;
}


double2
cm_ecl_to_equ(double lon, double lat, double obliq)
{
  double t_a_u = (sin(lon) * cos(obliq) - tan(lat) * sin(obliq)); // / cos(lon);
  double s_d = sin(lat) * cos(obliq) + cos(lat) * sin(obliq) * sin(lon);
  double2 res;
  res.x = atan2(t_a_u, cos(lon));
  res.y = asin(s_d);
  return res;
}

//alpha = ra
//delta = decl

// Note, proper motions not included
  
double2
cm_equ_epoch_conv(double ra0, double decl0, double jde0, double jde)
{
  double T = (jde0 - CM_J2000_0) / 36525.0;
  double t = (jde - jde0) / 36525.0;

  // Units in seconds of arc
  double c = (2306.2181 + 1.39656 * T - 0.000139 * T * T) * t
           + (0.30188 - 0.000344 * T) * t * t
           + 0.017998 * t * t * t;
  double z = (2306.2181 + 1.39656 * T - 0.000139 * T * T) * t
           + (1.09468 + 0.000066 * T) * t * t
           + 0.018203 * t * t * t;
  double theta = (2004.3109 - 0.85330 * T - 0.000217 * T * T) * t
               - (0.42665 + 0.000217 * T) * t * t
               - 0.041833 * t * t * t;
  // To degrees
  c = c / 3600.0;
  z = z / 3600.0;
  theta = theta / 3600.0;
  // To radians
  c = deg2rad(c);
  z = deg2rad(z);
  theta = deg2rad(theta);

  double A = cos(decl0) * sin(ra0 + c);
  double B = cos(theta) * cos(decl0) * cos(ra0 + c) - sin(theta) * sin(decl0);
  double C = sin(theta) * cos(decl0) * cos(ra0 + c) + cos(theta) * sin(decl0);

  double ra = atan2(A, B) + z;

  // TODO: Check the selection chriteria here
  double decl;
  if (decl0 > M_PI/4.0 - 0.1) decl = acos(sqrt(A*A+B*B));
  else decl = asin(C);

  double2 res = {ra, decl};
  return res;
}

// lambda = longitude
// beta = latitude
// Meeus p128
double2
cm_ecl_epoch_conv(double lon0, double lat0, double jde0, double jde)
{
  double T = (jde0 - CM_J2000_0) / 36525.0;
  double t = (jde - jde0) / 36525.0;

  double n = (47.0029 - 0.06603 * T + 0.000598 * T * T) * t
           + (-0.03302 + 0.000598 * T) * t * t
           + 0.000060 * t * t * t;
  double PI = /*174.876384 +*/ 3289.4789 * T
            + 0.60622 * T * T
            - (869.8089 + 0.50491 * T) * t
            + 0.03536 * t * t;
  double p = (5029.0966 + 2.22226 * T - 0.000042 * T * T) * t
           + (1.11113 - 0.000042 * T) * t * t
           - 0.000006 * t * t * t;
  
  // To degrees
  n /= 3600.0;
  PI /= 3600.0;
  p /= 3600.0;

  // Add term listed in degrees on p 128 Astronomical Algorithms 
  PI += 174.876384;
  
  n = deg2rad(n);
  PI = deg2rad(PI);
  p = deg2rad(p);

  double Ap = cos(n) * cos(lat0) * sin(PI - lon0) - sin(n) * sin(lat0);
  double Bp = cos(lat0) * cos(PI - lon0);
  double Cp = cos(n) * sin(lat0) + sin(n) * cos(lat0) * sin(PI - lon0);

  double lon =  p + PI -atan2(Ap, Bp);
  double lat = asin(Cp);
  double2 res = {lon, lat};
  return res;
}

double3
cm_spherical_to_rect(double lon, double lat, double r)
{
  double3 res;
  res.x = r * cos(lat) * cos(lon);
  res.y = r * cos(lat) * sin(lon);
  res.z = r * sin(lat);
  return res;
}

// Create a standard rotation matrix for rotating cooridnates on the ellipses
// For example, compute the true anomaly and the position on an ellipsis then
// rotate this point by this matrix
void
cm_zxz_rotmatrix(double3x3 R, double asc_node, double incl, double arg_peri)
{
  double cos_an = cos(asc_node);
  double sin_an = sin(asc_node);
  double cos_inc = cos(incl);
  double sin_inc = sin(incl);
  double cos_ap = cos(arg_peri);
  double sin_ap = sin(arg_peri);

  R[0].x = cos_an * cos_inc * cos_ap - sin_an * sin_ap;
  R[0].y = -cos_ap * sin_an - cos_an * cos_inc * sin_ap;
  R[0].z = cos_an * sin_inc;

  R[1].x = cos_an * sin_ap + cos_inc * cos_ap * sin_an;
  R[1].y = cos_an * cos_ap - cos_inc * sin_an * sin_ap;
  R[1].z = sin_an * sin_inc;

  R[2].x = -cos_ap * sin_inc;
  R[2].y = sin_inc * sin_ap;
  R[2].z = cos_inc;
}

// From a to b
void
cm_vector_rotmatrix(double3x3 R, const double3 v0, const double3 v1)
{
  double a = acos(vd3_dot(v0, v1));
  double3 u = vd3_normalise(vd3_cross(v0, v1));

  R[0].x = cos(a) + u.x*u.x*(1.0-cos(a));
  R[0].y = u.x*u.y*(1.0-cos(a))-u.z*sin(a);
  R[0].z = u.x*u.z*(1.0-cos(a))+u.y*sin(a);
  R[1].x = u.y*u.x*(1.0-cos(a))+u.z*sin(a);
  R[1].y = cos(a) + u.y*u.y*(1.0-cos(a));
  R[1].z = u.y*u.z*(1.0-cos(a))-u.x*sin(a);
  R[2].x = u.z*u.x*(1.0-cos(a))-u.y*sin(a);
  R[2].y = u.y*u.y*(1.0-cos(a))+u.x*sin(a);
  R[2].z = cos(a) + u.x*u.x*(1.0-cos(a));
}

// For laplace planes:
//   ra and dec known for plane (this is the north pole of the plane)
//   this plane is in equatorial coordinates
//   create a vector to the plane pole
// We get rectangular coords for the laplace plane, this is not an equ plane
// we need to first transform the laplace plane to equ
void
cm_laplace_frame_rotate(double3x3 R, double ra, double decl)
{
  double3 equ_v = {0.0, 0.0, 1.0};
  double3 lf = cm_spherical_to_rect(ra, decl, 1.0);
  // Create rot matrix for the translation between the ra decl to eq coords
  // AHRG! This is confusing, is this pointing in the right direction or
  // should we do equ_v to lf?
  cm_vector_rotmatrix(R, lf, equ_v);
}


// TODO: Move elsewhere
// See http://downloads.rene-schwarz.com/download/M002-Cartesian_State_Vectors_to_Keplerian_Orbit_Elements.pdf
void
cm_state_vector_to_orbital_elements(cm_orbital_elements_t *oe,
                                    const cm_state_vectors_t *sv)
{
  double r2 = vd3_dot(sv->p, sv->p);
  double v2 = vd3_dot(sv->v, sv->v);
  double r = sqrt(r2);
  //double v = sqrt(v2);
  double GM = CM_MU;
  double3 h = vd3_cross(sv->p, sv->p);

  double3 e = vd3_cross(sv->v, h);
  e.x /= GM;
  e.y /= GM;
  e.z /= GM;
  double3 rn = {-sv->p.x/r, -sv->p.y/r, -sv->p.z/r};
  e = vd3_add(e, rn);
  double3 n = {-h.y, h.z, 0};

  double inc = acos(h.z/sqrt(vd3_dot(h, h)));
  double ta;
  if (vd3_dot(sv->p, sv->v) >= 0.0) {
    ta = acos(vd3_dot(e, sv->p)/sqrt(vd3_dot(e, e))*r);
  } else {
    ta = 2.0 * M_PI - acos(vd3_dot(e, sv->p)/sqrt(vd3_dot(e, e))*r);
  }
  // Semi-major axis
  double a = 1.0 / (2.0/r + v2/GM);
  // Eccentricity vector
  double3 E;
  E.x = v2 * sv->p.x / CM_MU - vd3_dot(sv->p, sv->v) * sv->v.x / CM_MU
      - sv->p.x / r;
  E.y = v2 * sv->p.y / CM_MU - vd3_dot(sv->p, sv->v) * sv->v.y / CM_MU
      - sv->p.y / r;
  E.z = v2 * sv->p.z / CM_MU - vd3_dot(sv->p, sv->v) * sv->v.z / CM_MU
      - sv->p.z / r;

  double ecc = sqrt(vd3_dot(E, E)); //Eccentricity
  double ecc_an = 2.0 * atan(tan(ta/2.0)/sqrt((1.0 + ecc)/(1.0-ecc)));

  double long_asc;
  double arg_peri;
  if (n.y >= 0.0) {
    long_asc = acos(n.z/sqrt(vd3_dot(n, n)));
  } else {
    long_asc = 2*M_PI - acos(n.z/sqrt(vd3_dot(n, n)));
  }

  if (e.z >= 0.0) {
    arg_peri = acos(vd3_dot(n, e)/sqrt(vd3_dot(n, n))*sqrt(vd3_dot(e, e)));
  } else {
    arg_peri = 2.0*M_PI - acos(vd3_dot(n, e)/sqrt(vd3_dot(n, n))*sqrt(vd3_dot(e, e)));
  }

  double M = ecc_an - ecc * sin(ecc_an);

  // TODO: Fix this
  oe->a = a;
  oe->e = ecc;
  oe->i = inc;
  //oe->L = M;
  oe->Omega = long_asc;
  oe->w = arg_peri;
}
