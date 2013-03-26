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

#include <assert.h>
#include <math.h>
#include <stdlib.h>
#include <celmek/celmek.h>

// This file implements the iau roatiaional model, for more info see the
// IAU WG Report on Cartographic Coordinates and Rotational Elements
// The tables are copied from the WG report and placed in machine readable
// format. The file is then parsed with a script that generates tables in C99.
// This table is include here, and no other place
typedef struct {
  const char *name;
  double alpha0;
  double alpha_T;
  double delta0;
  double delta_T;
  double w0;
  double wd;
} iau_rotational_elements_t;


#include "iau-rot-tables.h"

static double terms[IAU_TERM_COUNT];
static double cosines[IAU_COS_COUNT];
static double sines[IAU_SIN_COUNT];

#include <stdio.h>


static void
iau_rot_model_step(cm_rotational_model_t *model, cm_world_t *state)
{
  // First we compute all the terms, some of these are needed for
  // several bodies
  double d = state->d - CM_J2000_0;
  double T = state->T - CM_J2000_0/CM_JD_PER_CENT;

  for (int i = 0 ; i < sizeof(d_terms)/sizeof(iau_term_t) ; i ++) {
    terms[d_terms[i].term] = d_terms[i].a + d_terms[i].b * d;
  }

  for (int i = 0 ; i < sizeof(T_terms)/sizeof(iau_term_t) ; i ++) {
    terms[T_terms[i].term] = T_terms[i].a + T_terms[i].b * T;
  }
  
  for (int i = 0 ; i < sizeof(T2_terms)/sizeof(iau_term2_t) ; i ++) {
    terms[T2_terms[i].term] += T2_terms[i].a * T * T;
  }

  // The terms are used as arguments to sin and cos, we compute these here
  // Note that the cosine_rules and sine_rules tables are auto generated
  assert(IAU_COS_COUNT == sizeof(cosine_rules)/sizeof(iau_arg_rule_t));
  assert(IAU_SIN_COUNT == sizeof(sine_rules)/sizeof(iau_arg_rule_t));

  for (iau_sine_id_t i = 0 ; i < IAU_SIN_COUNT ; i++) {
    sines[i] = sin(sine_rules[i].m * terms[sine_rules[i].term]);
  }

  for (iau_cosine_id_t i = 0 ; i < IAU_COS_COUNT ; i++) {
    cosines[i] = cos(cosine_rules[i].m * terms[cosine_rules[i].term]);
  }
  
//  printf("t terms %d\n", (int)(sizeof(d_terms)/sizeof(iau_term_t)));
//  printf("d terms %d\n", (int)(sizeof(T_terms)/sizeof(iau_term_t)));
  
//  printf("sines %d\n", IAU_COS_COUNT);
//  printf("cosines %d\n", IAU_COS_COUNT);

}
#include <string.h>
int iau_body_comp(const iau_body_t *a, const iau_body_t *b)
{
  return strcmp(a->name, b->name);
}
static void
iau_rot_object_init(cm_orbit_t *orbit)
{
  iau_body_t query = {.name = orbit->name};
  iau_body_t *res = bsearch(&query, bodies, IAU_BODY_COUNT, sizeof(iau_body_t),
                            (int (*)(const void *, const void *))iau_body_comp);
  if (!res) printf("did not find %s\n", orbit->name);
  orbit->rmod_data = res;
}

static void
iau_rot_object_step(cm_orbit_t *orbit, cm_world_t *state)
{
  iau_body_t *body = orbit->rmod_data;

  double d = state->d;
  double T = state->T;

  double alpha = 0.0;
  double delta = 0.0;
  double w = 0.0;

  alpha += body->a0 + body->aT * T;
  delta += body->d0 + body->dT * T;
  w += body->w0 + body->wd * d;
  w += body->wd_2 * d * d;
  w += body->wT_2 * T * T;

  for (int i = 0 ; i < body->asin_count ; i ++) {
    alpha += body->alpha_sines[i].a * sines[body->alpha_sines[i].sine];
  }

  for (int i = 0 ; i < body->dcos_count ; i ++) {
    delta += body->delta_cosines[i].a * cosines[body->delta_cosines[i].cosine];
  }

  for (int i = 0 ; i < body->wsin_count ; i ++) {
    w += body->w_sines[i].a * sines[body->w_sines[i].sine];      
  }

  for (int i = 0 ; i < body->wcos_count ; i ++) {
    w += body->w_cosines[i].a * cosines[body->w_cosines[i].cosine];      
  }

  orbit->r.x = alpha; // Right asencion
  orbit->r.y = delta; // Declination
  orbit->W = w;//fmod(w, 2.0 * M_PI); // Rot around axis

  // For a progam using these routines, the derivative of w is very interesting
  // the derivative allow us to for example easily compute a rough estimate of
  // airspeed.

  // The general equation for the w parameter is:
  //   w = a0 + a1 * d + a2 sin (a3 + a4 d) + a5 cos (a6 + a7 d)
  // This means that:
  //   w' = a1 + a2 a4 cos(a3 + a4 d) + a5 a7 sin (a6 + a7 d)
  // Note that the parameters to the sin and cos function are sometimes given
  // in julian centuries (i.e. d/36525).
  double w_prime = body->wd + 2.0 * body->wd_2 * d
                 + 2.0 * body->wT_2 * T/CM_JD_PER_CENT;
  //for (int i = 0 ; i < body->wsin_count ; i ++) {
  //  w_prime += body->w_sines[i].a * sines[body->w_sines[i].sine];
  //}

  //for (int i = 0 ; i < body->wcos_count ; i ++) {
  //  w_prime += body->w_cosines[i].a * cosines[body->w_cosines[i].cosine];
  //}
  orbit->W_prime = w_prime; // TODO: Ensure we add cos and sine terms

  orbit->q = q_rot(0,0,1, M_PI_2 + alpha);
  orbit->q = q_normalise(q_mul(orbit->q, q_rot(1,0,0, M_PI_2 - delta)));
  orbit->q = q_normalise(q_mul(orbit->q, q_rot(0,0,1, w)));
}

static cm_rotational_model_t iau_rot_model = {
  .name = "iau-std-rot",
  .step_model = iau_rot_model_step,
  .init_object = iau_rot_object_init,
  .step_object = iau_rot_object_step
};

void
iau_rot_init(void)
{
  // Radianize tables, they are in degrees for readability purposese
  for (int i = 0 ; i < IAU_BODY_COUNT ; i ++) {
    bodies[i].a0 = deg2rad(bodies[i].a0);
    bodies[i].aT = deg2rad(bodies[i].aT);
    bodies[i].d0 = deg2rad(bodies[i].d0);
    bodies[i].dT = deg2rad(bodies[i].dT);
    bodies[i].w0 = deg2rad(bodies[i].w0);
    bodies[i].wd = deg2rad(bodies[i].wd);
    bodies[i].wd_2 = deg2rad(bodies[i].wd_2);
    bodies[i].wT_2 = deg2rad(bodies[i].wT_2);
  }

  for (int i = 0 ; i < sizeof(d_terms)/sizeof(iau_term_t) ; i ++) {
    d_terms[i].a = deg2rad(d_terms[i].a);
    d_terms[i].b = deg2rad(d_terms[i].b);
  }

  for (int i = 0 ; i < sizeof(T_terms)/sizeof(iau_term_t) ; i ++) {
    T_terms[i].a = deg2rad(T_terms[i].a);
    T_terms[i].b = deg2rad(T_terms[i].b);
  }

  for (int i = 0 ; i < sizeof(T2_terms)/sizeof(iau_term2_t) ; i ++) {
    T2_terms[i].a = deg2rad(T2_terms[i].a);
  }


  cm_register_rotational_model(&iau_rot_model);
}
