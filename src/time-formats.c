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
#include <string.h>
#include <stdio.h>
#include <celmek/celmek.h>

#if 0
double
cm_tcb_to_tdb(double tcb)
{
  double jdtcb = 2443144.5003725 + seconds / 86400.0;
  return tcb - 1.550519768e-8 * (x - 2443144.5003725) * 86400.0 + -6.55e-5;
}

#endif

// Parse and convert a value: "YYYY-MM-DDTHH:MM:SS.s"
// Returns NAN on error
double
cm_iso_string_to_jd(const char *str)
{
  int year, month, day, hour, minute;
  char tz;
  int tz_h_offset, tz_m_offset;
  double secs;

  int i = sscanf(str, "%d-%d-%dT%d:%d:%lf%[+-Z]%d:%d",
                 &year, &month, &day, &hour, &minute, &secs,
                 &tz, &tz_h_offset, &tz_m_offset);
    
  
  return NAN;
}
