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


#ifndef celmek_cm_math_h
#define celmek_cm_math_h

#include <math.h>

// Mathematical constants
#define CM_RAD_PER_DEG (M_PI/180.0)
#define CM_DEG_PER_RAD (180.0/M_PI)


// Many of the types and functions here are fallback methods for when vmath is
// not avaialable

static inline double
deg2rad(double x)
{
  return x * M_PI/180.0;
}
static inline double
rad2deg(double x)
{
  return x * 180.0/M_PI;
}



typedef struct {
  double x, y;
} double2;

typedef struct {
  double x, y, z;
} double3;

typedef double3 double3x3[3];



static inline double3
v3d_add(double3 a, double3 b)
{
  double3 res = {.x = a.x + b.x, .y = a.y + b.y, .z = a.z + b.z };
  return res;
}

static inline double
v3d_dot(double3 a, double3 b)
{
  return a.x * b.x + a.y * b.y + a.z * b.z;
}


static inline double3
v3d_cross(double3 a, double3 b)
{
  double3 res;
  res.x = a.y*b.z - a.z*b.y;
  res.y = a.z*b.x - a.x*b.z;
  res.z = a.x*b.y - a.y*b.x;
  return res;
}

static inline double3
v3d_norm(double3 a)
{
  double len_recip = 1.0/sqrt(v3d_dot(a, a));

  double3 res;
  res.x = a.x * len_recip;
  res.y = a.y * len_recip;
  res.z = a.z * len_recip;
  return res;
}

static inline double3
m3vd_mul(double3x3 M, double3 v)
{
  double3 res;
  res.x = M[0].x * v.x + M[0].y * v.y + M[0].z * v.z;
  res.y = M[1].x * v.x + M[1].y * v.y + M[1].z * v.z;
  res.z = M[2].x * v.x + M[2].y * v.y + M[2].z * v.z;
  return res;
}

static inline void
m3d_transpose(double3x3 A, const double3x3 B)
{
  A[1].x = B[0].y;
  A[2].x = B[0].z;
  A[0].y = B[1].x;
  A[2].y = B[1].z;
  A[0].z = B[2].x;
  A[1].z = B[2].y;
}


static inline void
m3m_mul(double3x3 A, const double3x3 B, const double3x3 C)
{
  double3x3 temp;
  double3x3 Ct;
  m3d_transpose(Ct, C);

  for (int i = 0 ; i < 3 ; i ++) {
    temp[i].x = v3d_dot(B[i], Ct[0]);
    temp[i].y = v3d_dot(B[i], Ct[1]);
    temp[i].z = v3d_dot(B[i], Ct[2]);
  }
}


#endif
