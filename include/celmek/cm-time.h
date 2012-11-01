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

#ifndef celmek_cm_time_h
#define celmek_cm_time_h

typedef enum {
  CM_UNIX, // Unix time (seconds since 1970-01-01)
  CM_TAI, // Atomic coordinated time at sea-level
  CM_UTC, // TAI adjusted with leap seconds
  CM_TDB, // Barycentric dyanamical time = TT
  CM_TCB, // Barycentric coordinate time (outside the gravity well)
  CM_TT, // Terrestial time = TAI
} cm_time_format_t;



#endif
