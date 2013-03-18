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
#include <vmath/vmath.h>

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

#endif
