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

#include <stdio.h>

/*
Packed Dates
Dates of the form YYYYMMDD may be packed into five characters to conserve space.
The first two digits of the year are packed into a single character in
column 1 (I = 18, J = 19, K = 20). Columns 2-3 contain the last two digits of
the year. Column 4 contains the month and column 5 contains the day, coded as
detailed below:
   Month     Day      Character         Day      Character
                     in Col 4 or 5              in Col 4 or 5
   Jan.       1           1             17           H
   Feb.       2           2             18           I
   Mar.       3           3             19           J
   Apr.       4           4             20           K
   May        5           5             21           L
   June       6           6             22           M
   July       7           7             23           N
   Aug.       8           8             24           O
   Sept.      9           9             25           P
   Oct.      10           A             26           Q
   Nov.      11           B             27           R
   Dec.      12           C             28           S
             13           D             29           T
             14           E             30           U
             15           F             31           V
             16           G

Examples:

   1996 Jan. 1    = J9611
   1996 Jan. 10   = J961A
   1996 Sept.30   = J969U
   1996 Oct. 1    = J96A1
   2001 Oct. 22   = K01AM
This system can be extended to dates with non-integral days. The decimal
fraction of the day is simply appended to the five characters defined above.
Examples:

   1998 Jan. 18.73     = J981I73
   2001 Oct. 22.138303 = K01AM138303
     */

// Note that this function assumes that the packed date is valid, if it isn't,
// it generates jibberish dates
void
cm_read_packed_date(const char *date)
{
  // I, J, K designate 1800, 1900 and 2000, we assume this is consistent
  // in the future
  int year = (date[0] - 'I') * 100 + 1800;
  year += 10 * (date[1] - '0');
  year += date[2] - '0';
  int month = -1;
  if ('0' <= date[3] && date[3] <= '9') {
    month = date[3] - '0';
  } else {
    month = date[3] - 'A' + 10;
  }
  int day = -1;
  if ('0' <= date[4] && date[4] <= '9') {
    day = date[4] - '0';
  } else {
    day = date[4] - 'A' + 10;
  }

  printf("%s = %04d-%02d-%02d\n", date, year, month, day);
}

#define TEST_LINE "00001    3.34  0.12 K129U 285.41022   71.87913   80.33049   10.59313  0.0769128  0.21377719   2.7700980  0 MPO216418  6332  99 1802-2011 0.60 M-v 30h MPCLINUX   0000      (1) Ceres              20111215"

/*
  1 -   7  a7     Number or provisional designation
                    (in packed form)
  9 -  13  f5.2   Absolute magnitude, H
 15 -  19  f5.2   Slope parameter, G

 21 -  25  a5     Epoch (in packed form, .0 TT)
 27 -  35  f9.5   Mean anomaly at the epoch, in degrees

 38 -  46  f9.5   Argument of perihelion, J2000.0 (degrees)
 49 -  57  f9.5   Longitude of the ascending node, J2000.0
                    (degrees)
 60 -  68  f9.5   Inclination to the ecliptic, J2000.0 (degrees)

 71 -  79  f9.7   Orbital eccentricity
 81 -  91  f11.8  Mean daily motion (degrees per day)
 93 - 103  f11.7  Semimajor axis (AU)

106        i1     Uncertainty parameter, U
                  If this column contains `E' it indicates
                  that the orbital eccentricity was assumed.
                  For one-opposition orbits this column can
                  also contain `D' if a double (or multiple)
                  designation is involved or `F' if an e-assumed
                  double (or multiple) designation is involved.

108 - 116  a10    Reference
118 - 122  i5     Number of observations
124 - 126  i3     Number of oppositions

   For multiple-opposition orbits:
   128 - 131  i4     Year of first observation
   132        a1     '-'
   133 - 136  i4     Year of last observation

   For single-opposition orbits:
   128 - 131  i4     Arc length (days)
   133 - 136  a4     'days'

138 - 141  f4.2   r.m.s residual (")
143 - 145  a3     Coarse indicator of perturbers
                  (blank if unperturbed one-opposition object)
147 - 149  a3     Precise indicator of perturbers
                  (blank if unperturbed one-opposition object)
151 - 160  a10    Computer name
*/


/*
 9 -  13  f5.2   Absolute magnitude, H
15 -  19  f5.2   Slope parameter, G

21 -  25  a5     Epoch (in packed form, .0 TT)
27 -  35  f9.5   Mean anomaly at the epoch, in degrees

38 -  46  f9.5   Argument of perihelion, J2000.0 (degrees)
49 -  57  f9.5   Longitude of the ascending node, J2000.0
                   (degrees)
60 -  68  f9.5   Inclination to the ecliptic, J2000.0 (degrees)

71 -  79  f9.7   Orbital eccentricity
81 -  91  f11.8  Mean daily motion (degrees per day)
93 - 103  f11.7  Semimajor axis (AU)
  
167 - 194  a      Readable designation
  195 - 202 (end)
*/

#define MPORBIT_MAX_LINE_LENGTH 202
#include <string.h>
void
cm_read_line(const char *line)
{
  double H, G, M, arg_peri, long_asc, incl, ecc, mean_motion, semimajor;

  size_t slen = strlen(line);
  
  sscanf(&line[8], "%8lf", &H);
  sscanf(&line[14], "%8lf", &G);
  sscanf(&line[26], "%15lf", &M);
  sscanf(&line[37], "%15lf", &arg_peri);
  sscanf(&line[48], "%15lf", &long_asc);
  sscanf(&line[59], "%15lf", &incl);
  sscanf(&line[70], "%17lf", &ecc);
  sscanf(&line[80], "%20lf", &mean_motion);
  sscanf(&line[92], "%19lf", &semimajor);
  
  if (slen > 166) {
    char name[194-167+2]; // Add 1 as range is inclusive, and 1 for null char
    memset(name, 0, sizeof(name));
    sscanf(&line[166], "%28c", name);
    char *p = strrchr(name, '\0');
    p --;
    while (*p == ' ') p--;
    p[1] = '\0';
    printf("name = '%s'\n", name); //"(1) Ceres"
  }
  
  printf("abs mag: %f\n", H); // 3.34
  printf("slope: %f\n", G); //0.12
  printf("mean anomaly at epoch: %f\n", M); //285.41022
  printf("arg of perihelion: %f\n", arg_peri); //71.87913
  printf("longitude of ascending node: %f\n", long_asc);//80.33049
  printf("inclination: %f\n", incl);//10.59313
  printf("eccentricity: %f\n", ecc);//0.0769128
  printf("mean motion: %f\n", mean_motion);//0.21377719
  printf("semimajor: %f\n", semimajor);//2.7700980
}


/*
  http://www.minorplanetcenter.net/iau/MPCORB/CometEls.txt
Ephemerides and Orbital Elements Format
The column headed `F77' indicates the Fortran 77 format specifier that should be used to read the specified value.
   Columns   F77    Use

    1 -   4  i4     Periodic comet number
    5        a1     Orbit type (generally `C', `P' or `D')
    6 -  12  a7     Provisional designation (in packed form)

   15 -  18  i4     Year of perihelion passage
   20 -  21  i2     Month of perihelion passage
   23 -  29  f7.4   Day of perihelion passage (TT)

   31 -  39  f9.6   Perihelion distance (AU)
   42 -  49  f8.6   Orbital eccentricity

   52 -  59  f8.4   Argument of perihelion, J2000.0 (degrees)
   62 -  69  f8.4   Longitude of the ascending node, J2000.0
                      (degrees)
   72 -  79  f8.4   Inclination in degrees, J2000.0 (degrees)

   82 -  85  i4     Year of epoch for perturbed solutions
   86 -  87  i2     Month of epoch for perturbed solutions
   88 -  89  i2     Day of epoch for perturbed solutions

   92 -  95  f4.1   Absolute magnitude
   97 - 100  f5.1   Slope parameter

  103 -      a      Name
  
  
  
*/
int
main(int argc, char const *argv[argc])
{
  //K129U = 2012-09-30
  //J609N = 1960-09-23
  //J739F = 1973-09-15
  //J77AO = 1977-10-24
  //J77A4 = 1977-10-04
  
  //1996 Jan. 1    = J9611
  //1996 Jan. 10   = J961A
  //1996 Sept.30   = J969U
  //1996 Oct. 1    = J96A1
  //2001 Oct. 22   = K01AM
  
  cm_read_packed_date("K129U");
  cm_read_packed_date("J609N");
  cm_read_packed_date("J739F");
  cm_read_packed_date("J77AO");
  cm_read_packed_date("J77A4");
 
  
  cm_read_packed_date("J9611");
  cm_read_packed_date("J961A");
  cm_read_packed_date("J969U");
  cm_read_packed_date("J96A1");
  cm_read_packed_date("K01AM");    
    
  cm_read_line(TEST_LINE);
  return 0;
}

/*
Export Format for Natural Satellite Orbits
This document describes the format used for orbits of natural satellites, a
used in the Natural Satellites Observers Page.
Orbital elements for natural satellites are planet-barycentric.
The column headed `F77' indicates the Fortran 77 format specifier that should be
used to read the specified value.
   Columns   F77    Use

    1 -  12  a12    Number or provisional designation
                      (in packed form)

   14 -  18  a5     Epoch (in packed form, .0 TT)

   20 -  32  f13.5  Time of perihelion (JDT)

   34 -  42  f9.5   Argument of perihelion, J2000.0 (degrees)
   44 -  52  f9.5   Longitude of the ascending node, J2000.0
                      (degrees)
   54 -  62  f9.5   Inclination to the ecliptic, J2000.0 (degrees)

   64 -  72  f9.7   Orbital eccentricity
   74 -  82  f9.7   Periapsis distance (AU)

   84 -  85  i2     Central body (05 = Jupiter, 06 = Saturn,
                      07 = Uranus, 08 = Neptune)
   87 -  91  f5.2   Absolute magnitude, H

   93 -  98  i6     Observed arc (days)
  100 - 104  i5     Number of observations

  106 - 108  a3     Orbit computer
  110 - 118  a9     Publication reference

  120 - 124  a5     Date (UT) of first included observation (in packed form)
  126 - 130  a5     Date (UT) of last included observation (in packed form)

  132 - 136  f5.2   r.m.s residual (")
  138 - 140  a3     Coarse indicator of perturbers
  142 - 144  a3     Precise indicator of perturbers
                    (blank if unperturbed one-opposition object)
  146 -      a      Name of satellite
 
  
*/
