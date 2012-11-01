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

#ifndef celmek_celmek_h
#define celmek_celmek_h


#include <celmek/cm-math.h>

typedef struct {
  double3 p;
  double3 v;
} cm_state_vectors_t;

typedef struct {
  double L; // Mean longitude
  double a; // Semimajor axis
  double e; // Eccentricity
  double i; // Inclination
  double omega; // Longitude of ascending node
  double pi; // Longitude of periapsis
} cm_orbital_elements_t;

#include <celmek/cm-time.h>
#include <celmek/cm-coordinates.h>
#include <celmek/cm-astro.h>
#include <celmek/cm-model.h>

// Initializes all methods (calling every method init, which among other things
// will convert tables in degrees to radians). MUST BE CALLED BEFORE ANY OTHER
// FUNCTION

void cm_init(void);


#endif
