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

#include <celmek/celmek.h>
#include <math.h>
#include <stdbool.h>

void
cm_orbital_elements_init(cm_kepler_elements_t *oe)
{
  oe->semi_major = NAN;
  oe->ecc = NAN;
  oe->incl = NAN;
  oe->long_asc = NAN;
  oe->arg_peri = NAN;
  oe->mean_anomaly_at_epoch = NAN;
  oe->epoch = NAN;
  oe->mean_motion = NAN;
}

double
cm_semiminor_axis(double semimaj, double ecc)
{
  if (ecc < 1.0) {
    // Closed orbit
    return semimaj * sqrt(1.0 - ecc * ecc);
  }

  // Parabolic and hyperbolic
  return semimaj * sqrt(ecc * ecc - 1.0);
}

double
cm_period(double semimaj, double GM1, double GM2)
{
  return 2.0 * M_PI * sqrt(semimaj * semimaj * semimaj / (GM1 + GM2));
}

// TODO: Verify for relative negative epochs, should be fine
double
cm_mean_anomaly(double period, double t_peri, double epoch)
{
  double t = fmod(epoch - t_peri, period);
  double n = 2.0 * M_PI / period;
  return n * t;
}

double
cm_time_since_periapsis(double mean_anomaly, double mean_motion)
{
  return mean_anomaly / mean_motion;
}

double
cm_mean_motion(double period)
{
  return 2.0 * M_PI / period;
}

double
cm_long_periapsis(double arg_peri, double long_asc)
{
  return arg_peri + long_asc;
}

double
cm_arg_periapsis(double long_peri, double long_asc)
{
  return long_peri - long_asc;
}

void
cm_orbital_elements_set_semimajor_axis(cm_orbital_elements_t *oe, double a)
{
  oe->a = a;
}

void
cm_orbital_elements_set_eccentricity(cm_orbital_elements_t *oe, double e)
{
  oe->e = e;
}

void
cm_orbital_elements_set_inclination(cm_orbital_elements_t *oe, double i)
{
  oe->i = i;

}

void
cm_orbital_elements_set_long_asc(cm_orbital_elements_t *oe, double w)
{
  oe->Omega = w;
}

void
cm_orbital_elements_set_long_peri(cm_orbital_elements_t *oe, double pi)
{
  oe->w = pi; // Longitude of periapsis
}

void
cm_orbital_elements_set_long_peri_time(cm_orbital_elements_t *oe, double t)
{
  oe->t_w = t; // Time of periapsis
}

void
cm_orbital_elements_set_orbital_period(cm_orbital_elements_t *oe, double T)
{
  oe->T = T;
}


bool
cm_orbital_elements_check(const cm_kepler_elements_t *oe)
{
  if (isnan(oe->semi_major)) return false;
  if (isnan(oe->ecc)) return false;
  if (isnan(oe->incl)) return false;
  if (isnan(oe->long_asc)) return false;
  if (isnan(oe->arg_peri)) return false;
  if (isnan(oe->mean_anomaly_at_epoch)) return false;
  if (isnan(oe->mean_motion)) return false;
  if (isnan(oe->epoch)) return false;

  return true;
}
