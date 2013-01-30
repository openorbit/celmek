/*
 * Copyright (c) 2012,2013, Mattias Holm <lorrden(at)openorbit.org>.
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

#define CM_MJD 2400000.5 //!< Modified Julian Date offset from JD

/*!
 * Time format identifiers
 */
typedef enum {
  CM_UNIX, //!< Unix time (seconds since 1970-01-01)
  CM_TAI,  //!< Atomic coordinated time at sea-level
  CM_UTC,  //!< TAI adjusted with leap seconds
  CM_TDB,  //!< Barycentric dyanamical time = TT
  CM_TCB,  //!< Barycentric coordinate time (outside the gravity well)
  CM_TT,   //!< Terrestial time = TAI
} cm_time_format_t;

/*!
 * Month constants
 */
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

/*!
 * Componentised date structure
 */
typedef struct {
  int year;
  cm_month_t month;
  uint8_t day;
} cm_date_t;

/*!
 * Componentised time structure
 */
typedef struct {
  uint8_t hh;
  uint8_t mm;
  double s;
} cm_time_t;

/*!
 * Date time structure capable of storing both date and time.
 */
typedef struct {
  cm_date_t date;
  cm_time_t time;
} cm_date_time_t;


/*!
 * Reads a packed date as formatted in the MPC databases.
 *
 * Packed dates are strings that can be embedded in text files and takes less
 * space than a full ISO date string. An example date follows:
 *  K129U = 2012-09-30
 * Here K designates the 21st century, the next two didgits are the lower digtits
 * of the year, 9 means september (A meaning october) and the U indicates which day
 * of the month it is.
 *
 * \param date Resulting date structure to fill in, must be a valid pointer.
 * \param date_str A pointer to the first character of the packed date string. This
 *                 string does not have to be nul terminated and parsing works on
 *                 substrings without any problems.
 *
 * \bug Invalid date strings are not detected and results in and undefined date
 *      object.
 */
void cm_read_packed_date(cm_date_t *date, const char *date_str);

/*!
 * Converts an ISO date string to a date time object.
 *
 * An ISO formatted date is in the form of "YYYY-MM-DDTHH:MM:SS.s", where YYYY is a
 * four digit year, possibly with a '-' infront. MM is a two digit month number
 * in the range [01,12] and DD is a 2 digit day number in the month in the range
 * [01,31]. 'T' is either the letter 'T' or a space and designates the date-time
 * separation point in the string. The time is formatted using a 24 h clock with the
 * hours in the range [00, 23], minutes int the range [00,59] and seconds in the
 * range [0,60] (where 60 indicates a leap second). Note that the second may contain
 * a decimal point and fractions of a second.
 *
 * In addtion to the normal date time, the ISO dates may contain a time zone marker,
 * this marker is in the format +HH:MM or -HH:MM or simply Z for the zulu or UTC+0
 * timezone. If such a timezone marker is found at the end of the date time string,
 * the date object being filled in will be adjusted to represent the zulu time.
 *
 * \param dt Date object to be filled from the string.
 * \param str String with an ISO date.
 * \result -1 on failure and 0 on success.
 */
int cm_read_iso_date_string(cm_date_time_t *dt, const char *str);


/*!
 * Convert JD in TCB base to JD in TDB base.
 *
 * TCB is barycentric coordinate time. TDB is a linear transformation of TCB and is
 * known as barycentric dynamical time. TDB is useful as it roughly is identicial to
 * TT which most ephemeris packages use.
 *
 * \param tcb Julian date in TCB format.
 * \result Julian date in TDB format.
 */
double cm_tcb_to_tdb(double tcb);

/*!
 * Convert JD in TDB base to JD in TCB base.
 *
 * \param tdb Julian date in TDB format.
 * \result Julian date in TCB format.
 */
double cm_tdb_to_tcb(double tdb);

/*!
 * Convert componentised date time objcet to JD.
 * \param date The date time object to convert.
 * \result The Julian Date value corresponding to the given date.
 */
double cm_date_time_to_jd(const cm_date_time_t *date);

/*!
 * Convert Julian Date to componentised date time object.
 * \param jd The Julian Date to convert to componentised time.
 * \param date The object to place the date time in.
 */
void cm_jd_to_date_time(double jd, cm_date_time_t *date);

/*!
 * Compute Delta T i.e. TT-UT, it should work for any historical date.
 *
 */
double cm_delta_t(int year, cm_month_t month);

/*!
 * Compute Delta AT i.e. TAI-UTC, this value is only defined from 1961-01-01 and
 * onwards, note that before leapseconds existed, broadcast time was adjusted
 * with smaller leap ticks of 100 ms or so. Function shoudl be correct for UTC
 * JDs, but TAI JDs should give a good estimate, except for during and around
 * the leap seconds.
 */
double cm_delta_at(double jd);

#endif
