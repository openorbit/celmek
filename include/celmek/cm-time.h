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

#ifndef celmek_cm_time_h
#define celmek_cm_time_h

#include <stdint.h>

#define CM_MJD 2400000.5

typedef enum {
  CM_UNIX, // Unix time (seconds since 1970-01-01)
  CM_TAI, // Atomic coordinated time at sea-level
  CM_UTC, // TAI adjusted with leap seconds
  CM_TDB, // Barycentric dyanamical time = TT
  CM_TCB, // Barycentric coordinate time (outside the gravity well)
  CM_TT, // Terrestial time = TAI
} cm_time_format_t;

typedef enum {
  CM_MONTH_JANUARY = 1,
  CM_MONTH_FEBRUARY,
  CM_MONTH_MARS,
  CM_MONTH_APRIL,
  CM_MONTH_MAY,
  CM_MONTH_JUNE,
  CM_MONTH_JULY,
  CM_MONTH_AUGUST,
  CM_MONTH_SEPTEMBER,
  CM_MONTH_OCTOBER,
  CM_MONTH_NOVEMBER,
  CM_MONTH_DECEMBER,
} cm_month_t;

typedef struct {
  int year;
  cm_month_t month;
  uint8_t day;
} cm_date_t;

typedef struct {
  uint8_t hh;
  uint8_t mm;
  double s;
} cm_time_t;


typedef struct {
  cm_date_t date;
  cm_time_t time;
} cm_date_time_t;

void cm_read_packed_date(cm_date_t *date, const char *date_str);
int cm_read_iso_date_string(cm_date_time_t *dt, const char *str);


// Convert JD tcb to JD tdb
double cm_tcb_to_tdb(double tcb);
double cm_tdb_to_tcb(double tdb);


double cm_date_time_to_jd(const cm_date_time_t *date);
void cm_jd_to_date_time(double jd, cm_date_time_t *date);

// Compute Delta T i.e. TT-UT, it should work for any historical date
double cm_delta_t(int year, cm_month_t month);

// Compute Delta AT i.e. TAI-UTC, this value is only defined from 1961-01-01 and
// onwards, note that before leapseconds existed, broadcast time was adjusted
// with smaller leap ticks of 100 ms or so. Function shoudl be correct for UTC
// JDs, but TAI JDs should give a good estimate, except for during and around
// the leap seconds.
double cm_delta_at(double jd);

#endif
