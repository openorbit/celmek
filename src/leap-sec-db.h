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
  double jd_start;
  double jd_end;
  double offset;
  double mjd_offs;
  double mjd_scale;
} cm_leap_sec_range_t;

static cm_leap_sec_range_t leap_secs[] = {
  {2437300.5, 2437512.5,  1.4228180, -37300, 0.001296}, // 1961-01-01
  {2437512.5, 2437665.5,  1.3728180, -37300, 0.001296},
  {2437665.5, 2438334.5,  1.8458580, -37665, 0.0011232},
  {2438334.5, 2438395.5,  1.9458580, -37665, 0.0011232},
  {2438395.5, 2438486.5,  3.2401300, -38761, 0.001296},
  {2438486.5, 2438639.5,  3.3401300, -38761, 0.001296},
  {2438639.5, 2438761.5,  3.4401300, -38761, 0.001296},
  {2438761.5, 2438820.5,  3.5401300, -38761, 0.001296},
  {2438820.5, 2438942.5,  3.6401300, -38761, 0.001296},
  {2438942.5, 2439004.5,  3.7401300, -38761, 0.001296},
  {2439004.5, 2439126.5,  3.8401300, -38761, 0.001296},
  {2439126.5, 2439887.5,  4.3131700, -39126, 0.002592},
  {2439887.5, 2441317.5,  4.2131700, -39126, 0.002592},
  {2441317.5, 2441499.5, 10.0,            0, 0.0}, //1972-01-01
  {2441499.5, 2441683.5, 11.0,            0, 0.0},
  {2441683.5, 2442048.5, 12.0,            0, 0.0},
  {2442048.5, 2442413.5, 13.0,            0, 0.0},
  {2442413.5, 2442778.5, 14.0,            0, 0.0},
  {2442778.5, 2443144.5, 15.0,            0, 0.0},
  {2443144.5, 2443509.5, 16.0,            0, 0.0},
  {2443509.5, 2443874.5, 17.0,            0, 0.0},
  {2443874.5, 2444239.5, 18.0,            0, 0.0},
  {2444239.5, 2444786.5, 19.0,            0, 0.0},
  {2444786.5, 2445151.5, 20.0,            0, 0.0},
  {2445151.5, 2445516.5, 21.0,            0, 0.0},
  {2445516.5, 2446247.5, 22.0,            0, 0.0},
  {2446247.5, 2447161.5, 23.0,            0, 0.0},
  {2447161.5, 2447892.5, 24.0,            0, 0.0},
  {2447892.5, 2448257.5, 25.0,            0, 0.0},
  {2448257.5, 2448804.5, 26.0,            0, 0.0},
  {2448804.5, 2449169.5, 27.0,            0, 0.0},
  {2449169.5, 2449534.5, 28.0,            0, 0.0},
  {2449534.5, 2450083.5, 29.0,            0, 0.0},
  {2450083.5, 2450630.5, 30.0,            0, 0.0},
  {2450630.5, 2451179.5, 31.0,            0, 0.0},
  {2451179.5, 2453736.5, 32.0,            0, 0.0},
  {2453736.5, 2454832.5, 33.0,            0, 0.0},
  {2454832.5, 2456109.5, 34.0,            0, 0.0},
  {2456109.5, 9999999.0, 35.0,            0, 0.0}, // 2012-07-01
};

#endif
