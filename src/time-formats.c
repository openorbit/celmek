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
// TDT = TT = ET ≈ TDB (within 2 ms at earth)

// TDB
double
cm_tcb_to_tdb(double tcb)
{
  return tcb - CM_LB * (tcb - CM_TCB_0) + CM_TDB0 / 86400.0;
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


