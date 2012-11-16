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

/* Convert time formats */
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <celmek/celmek.h>

#include "leap-sec-db.h"

static const int month_length[] = {31, 28, 31, 30, 31, 30,
                                   31, 31, 30, 31, 30, 31};
static const int month_length_leap[] = {31, 29, 31, 30, 31, 30,
                                        31, 31, 30, 31, 30, 31};

static const int month_offset[] = {  0,  31,  59,  90, 120, 151,
                                   181, 212, 243, 273, 304, 334};
static const int month_offset_leap[] = {  0,  31,  60,  91, 121, 152,
                                        182, 213, 244, 274, 305, 335};

bool
is_leap_year(int y)
{
  if (y % 400 == 0) return true;
  if (y % 100 == 0) return false;
  if (y % 4 == 0) return true;
  return false;
}

// Implementation of polynomial expressions for delta T from
//   http://eclipse.gsfc.nasa.gov/SEcat5/deltatpoly.html
// Note that for ELP2000, c = -0.000012932 * (y - 1955)^2 needs to be addded to
// values before 1955
double
cm_delta_t(int year, cm_month_t month)
{
  double y = year + ((month - 0.5) / 12.0);
  double delta_t;

  // TODO: Fix dispatching mechanism, this is most likelly very slow, but
  // hopefully, the function will not be called that often.
  if (year < -500) {
    double u = (y - 1820.0)/100.0;
    delta_t = -20.0 + 32 * u * u;
  } else if (year < 500) {
    double u = y / 100.0;
    delta_t = 10583.6 - 1014.41 * u + 33.78311 * u * u - 5.952053 * u * u * u
            - 0.1798452 * u * u * u * u + 0.022174192 * u * u * u * u * u
            + 0.0090316521 * u * u * u * u * u * u;
  } else if (year < 1600) {
    double u = (y-1000.0)/100.0;
    delta_t = 1574.2 - 556.01 * u + 71.23472 * u * u + 0.319781 * u * u * u
            - 0.8503463 * u * u * u * u - 0.005050998 * u * u * u * u * u
            + 0.0083572073 * u * u * u * u * u * u;
  } else if (year < 1700) {
    double t = y - 1600.0;
    delta_t = 120.0 - 0.9808 * t - 0.01532 * t * t + t * t * t / 7129.0;
  } else if (year < 1800) {
    double t = y - 1700.0;
    delta_t = 8.83 + 0.1603 * t - 0.0059285 * t*t + 0.00013336 * t*t*t
            - t*t*t*t / 1174000.0;
  } else if (year < 1860) {
    double t = y - 1800.0;
    delta_t = 13.72 - 0.332447 * t + 0.0068612 * t*t + 0.0041116 * t*t*t
            - 0.00037436 * t*t*t*t + 0.0000121272 * t*t*t*t*t
            - 0.0000001699 * t*t*t*t*t*t + 0.000000000875 * t*t*t*t*t*t*t;
  } else if (year < 1900) {
    double t = y - 1860.0;
    delta_t = 7.62 + 0.5737 * t - 0.251754 * t*t + 0.01680668 * t*t*t
            -0.0004473624 * t*t*t*t + t*t*t*t*t / 233174.0;
  } else if (year < 1920) {
    double t = y - 1900.0;
    delta_t = -2.79 + 1.494119 * t - 0.0598939 * t*t + 0.0061966 * t*t*t
            - 0.000197 * t*t*t*t;
  } else if (year < 1941) {
    double t = y - 1920.0;
    delta_t = 21.20 + 0.84493*t - 0.076100 * t*t + 0.0020936 * t*t*t;
  } else if (year < 1961) {
    double t = y - 1950.0;
    delta_t = 29.07 + 0.407*t - t*t/233.0 + t*t*t / 2547.0;
  } else if (year < 1986) {
    double t = y - 1975.0;
    delta_t = 45.45 + 1.067*t - t*t/260.0 - t*t*t / 718.0;
  } else if (year < 2005) {
    double t = y - 2000.0;
    delta_t = 63.86 + 0.3345 * t - 0.060374 * t*t + 0.0017275 * t*t*t
            + 0.000651814 * t*t*t*t + 0.00002373599 * t*t*t*t*t;
  } else if (year < 2050) {
    double t = y - 2000.0;
    delta_t = 62.92 + 0.32217 * t + 0.005589 * t*t;
  } else if (year < 2150) {
    double u = ((y-1820.0)/100.0);
    delta_t = -20.0 + 32.0 * u*u - 0.5628 * (2150.0 - y);
  } else {
    double u = (y-1820.0)/100.0;
    delta_t = -20.0 + 32.0 * u*u;
  }


  return delta_t;
}
// TDT = TT = ET ≈ TDB (within 2 ms at earth)

// TDB / TCB conversion
// Note that we use the JD for tcb and tdb here, therfore the expression is
// adjusted from the IAU definition a little bit, as that expression is defined
// in seconds. The TDB to TCB equation is derived from the TCB to TDB equation.
// Note that these function use JD dates and only double precision, this is
// good enough for our purposes, but if you really need higher precicion, use
// the IAU SOFA library instead, it implements quad (double-double) precicion
// arithmetic.
double
cm_tcb_to_tdb(double tcb)
{
  return tcb - CM_LB * (tcb - CM_TCB_0) + CM_TDB0 / 86400.0;
}

double
cm_tdb_to_tcb(double tdb)
{
  return (tdb - CM_TDB0 / 86400.0 - CM_LB * CM_TCB_0) / (1.0-CM_LB);
}


double
cm_tai_to_tt(double tai)
{
  return tai + CM_TT_TAI_OFFSET;
}

// Gregorian date time
double
cm_date_time_to_jd(const cm_date_time_t *date)
{
  double res;

  int a = (14-date->date.month) / 12;
  int y = date->date.year + 4800 - a;
  int m = date->date.month + 12 * a - 3;

  res = date->date.day + (153*m + 2)/5 + 365*y + y/4 - y/100 + y/400 - 32045;

  res += ((int)date->time.hh-12)/24.0 + (int)date->time.mm/1440.0 + date->time.s/86400.0;
  return res;
}


int
lsr_comp(const cm_date_time_t *key, const cm_leap_sec_range_t *memb)
{
  return 0;
}

int
cm_utc_leapseconds(cm_date_time_t *utc)
{
  if (utc->date.year >= 1972) {
    cm_leap_sec_range_t *lsr = bsearch(&utc, leap_secs_1972,
                                       sizeof(leap_secs_1972)/sizeof(leap_secs_1972[0]),
                                       sizeof(leap_secs_1972[0]),
                                       (int (*)(const void*, const void*))lsr_comp);
    if (lsr) return lsr->secs;
  }

  return 0;
}


void
cm_jd_to_date_time(double jd, cm_date_time_t *date)
{
  double J = jd + 0.5;
  double j = J + 32044.0;

  double g = floor(j / 146097.0);
  double dg = j - g * 146097.0;

  double c = floor((floor(dg / 36534.0) + 1.0) * 3.0 / 4.0);
  double dc = dg - c * 36524.0;

  double b = floor(dc/1461.0);
  double db = dc - b * 1461.0;
  double a = floor((floor(db/365.0) + 1) * 3.0 / 4.0);
  double da = db - a * 365;
  double y = g * 400.0 + c * 100.0 + b * 4.0 + a;

  double m = floor((da * 5 + 308) / 153.0) - 2.0;
  double d = da - floor((m + 4) * 153.0 / 5.0) + 122;

  double Y = y - 4800.0 + floor((m + 2.0) / 12.0);
  double M = (m + 2.0) - floor((m + 2) / 12.0) * 12.0 + 1.0;
  double D = d + 1.0;

  date->date.year = Y;
  date->date.month = M;
  date->date.day = D;

  date->time.hh = fmod(D, 1.0) * 24.0;
  date->time.mm = fmod(D, 1.0) * 24.0 * 60.0 - date->time.hh * 60.0;
  date->time.s = fmod(D, 1.0) * 86400.0 - date->time.hh * 3600.0 - date->time.mm * 60.0;
}


double
cm_julian_centuries_from_epoch(double jd, double epoch)
{
  return (jd-epoch)/36525.0;
}

// Parse and convert a value: "YYYY-MM-DDTHH:MM:SS.s"
int
cm_read_iso_date_string(cm_date_time_t *dt, const char *str)
{
  int year = 0, month = 1, day = 1, hour = 0, minute = 0;
  char tz = 0, dummy;
  int tz_h_offset = 0, tz_m_offset = 0;
  double secs = 0.0;

  int iso_date_bytes;
  int i = sscanf(str, "%d-%2d-%2d%c%n",
                 &year, &month, &day, &dummy, &iso_date_bytes);

  if (i == 4) {
    int hhmm_bytes = 0, sec_bytes = 0;

    int j = sscanf(str + iso_date_bytes, "%d:%d%n", &hour, &minute, &hhmm_bytes);
    j = sscanf(str + iso_date_bytes + hhmm_bytes, ":%lf%n", &secs, &sec_bytes);

    j = sscanf(str + iso_date_bytes + hhmm_bytes + sec_bytes, "%c%d:%d",
           &tz, &tz_h_offset, &tz_m_offset);

  }

  if (month < 1 || 12 < month ) {
    // Bad date
    return -1;
  }

  if (day < 1 || 31 < day ) {
    // Bad date
    return -1;
  }

  dt->date.year = year;
  dt->date.month = month;
  dt->date.day = day;
  dt->time.hh = hour;
  dt->time.mm = minute;
  dt->time.s = secs;

  if (tz == '+') {
    dt->time.hh -= tz_h_offset;
    dt->time.mm -= tz_m_offset;
  }

  if (tz == '-') {
    dt->time.hh += tz_h_offset;
    dt->time.mm -= tz_m_offset;
  }

  if (dt->time.s >= 60.0) {
    dt->time.mm += dt->time.s / 60.0;
    dt->time.s = fmod(dt->time.s, 60.0);
  }

  if (dt->time.mm >= 60) {
    dt->time.hh += dt->time.mm / 60;
    dt->time.mm = dt->time.mm % 60;
  }

  if (dt->time.hh >= 24) {
    dt->date.day += dt->time.hh / 24;
    dt->time.hh = dt->time.hh % 24;
  }

  if (is_leap_year(dt->date.year)) {
    if (dt->date.day-1 >= month_length_leap[dt->date.month - 1]) {
      dt->date.month += (dt->date.day-1) / month_length_leap[dt->date.month - 1];
      dt->date.day = (dt->date.day-1) % month_length_leap[dt->date.month - 1] + 1;
    }
  } else {
    if (dt->date.day-1 >= month_length[dt->date.month - 1]) {
      dt->date.month += (dt->date.day-1) / month_length[dt->date.month - 1];
      dt->date.day = (dt->date.day-1) % month_length[dt->date.month - 1] + 1;
    }
  }

  if (dt->date.month-1 >= 12) {
    dt->date.year += (dt->date.month-1) / 12;
    dt->date.month = (dt->date.month-1) % 12 + 1;
  }

  return 0;
}


// Packed date reader for MPC files
// Note that this function assumes that the packed date is valid, if it isn't,
// it generates jibberish dates
void
cm_read_packed_date(cm_date_t *date, const char *date_str)
{
  // I, J, K designate 1800, 1900 and 2000, we assume this is consistent
  // in the future
  int year = (date_str[0] - 'I') * 100 + 1800;
  year += 10 * (date_str[1] - '0');
  year += date_str[2] - '0';
  int month = -1;
  if ('0' <= date_str[3] && date_str[3] <= '9') {
    month = date_str[3] - '0';
  } else {
    month = date_str[3] - 'A' + 10;
  }
  int day = -1;
  if ('0' <= date_str[4] && date_str[4] <= '9') {
    day = date_str[4] - '0';
  } else {
    day = date_str[4] - 'A' + 10;
  }

  date->year = year;
  date->month = month;
  date->day = day;
}


