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

#ifndef celmek_leap_sec_db_h
#define celmek_leap_sec_db_h

typedef struct {
  cm_date_t start;
  cm_date_t end;
  double offset;
  double mjd_offs;
  double mjd_scale;
} cm_old_style_leap_sec_range_t;

cm_old_style_leap_sec_range_t leap_secs_1961[] = {
  {{1961,  1, 1},  {1961,  8, 1},  1.4228180, -37300, 0.001296},
  {{1961,  8, 1},  {1962,  1, 1},  1.3728180, -37300, 0.001296},
  {{1962,  1, 1},  {1963, 11, 1},  1.8458580, -37665, 0.0011232},
  {{1963, 11, 1},  {1964,  1, 1},  1.9458580, -37665, 0.0011232},
  {{1964,  1, 1},  {1964,  4, 1},  3.2401300, -38761, 0.001296},
  {{1964,  4, 1},  {1964,  9, 1},  3.3401300, -38761, 0.001296},
  {{1964,  9, 1},  {1965,  1, 1},  3.4401300, -38761, 0.001296},
  {{1965,  1, 1},  {1965,  3, 1},  3.5401300, -38761, 0.001296},
  {{1965,  3, 1},  {1965,  7, 1},  3.6401300, -38761, 0.001296},
  {{1965,  7, 1},  {1965,  9, 1},  3.7401300, -38761, 0.001296},
  {{1965,  9, 1},  {1966,  1, 1},  3.8401300, -38761, 0.001296},
  {{1966,  1, 1},  {1968,  2, 1},  4.3131700, -39126, 0.002592},
  {{1968,  2, 1},  {1972,  1, 1},  4.2131700, -39126, 0.002592},
};

typedef struct {
  cm_date_t start;
  cm_date_t end;
  int secs;
} cm_leap_sec_range_t;

cm_leap_sec_range_t leap_secs_1972[] = {
  {{1972, 1,  1}, {1972,  7,  1}, 10},
  {{1972, 7,  1}, {1973,  1,  1}, 11},
  {{1973, 1,  1}, {1974,  1,  1}, 12},
  {{1974, 1,  1}, {1975,  1,  1}, 13},
  {{1975, 1,  1}, {1976,  1,  1}, 14},
  {{1976, 1,  1}, {1977,  1,  1}, 15},
  {{1977, 1,  1}, {1978,  1,  1}, 16},
  {{1978, 1,  1}, {1979,  1,  1}, 17},
  {{1979, 1,  1}, {1980,  1,  1}, 18},
  {{1980, 1,  1}, {1981,  7,  1}, 19},
  {{1981, 7,  1}, {1982,  7,  1}, 20},
  {{1982, 7,  1}, {1983,  7,  1}, 21},
  {{1983, 7,  1}, {1985,  7,  1}, 22},
  {{1985, 7,  1}, {1988,  1,  1}, 23},
  {{1988, 1,  1}, {1990,  1,  1}, 24},
  {{1990, 1,  1}, {1991,  1,  1}, 25},
  {{1991, 1,  1}, {1992,  7,  1}, 26},
  {{1992, 7,  1}, {1993,  7,  1}, 27},
  {{1993, 7,  1}, {1994,  7,  1}, 28},
  {{1994, 7,  1}, {1996,  1,  1}, 29},
  {{1996, 1,  1}, {1997,  7,  1}, 30},
  {{1997, 7,  1}, {1999,  1,  1}, 31},
  {{1999, 1,  1}, {2006,  1,  1}, 32},
  {{2006, 1,  1}, {2009,  1,  1}, 33},
  {{2009, 1,  1}, {2012,  7,  1}, 34},
  {{2012, 7,  1}, {9999, -1, -1}, 35},
};

#endif
