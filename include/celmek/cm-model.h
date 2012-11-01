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


#ifndef celmek_cm_model_h
#define celmek_cm_model_h

typedef struct cm_orbital_model_t cm_orbital_model_t;
typedef struct cm_rotational_model_t cm_rotational_model_t;

#define CM_PARAM_COUNT 10
// "earth", "vsop87" "iau"
typedef struct cm_orbit_t {
  const char *name;
  const char *parent_name;
  struct cm_orbit_t *parent;

  const char *omod_name;
  const char *rmod_name;

  void *omod_data;
  void *rmod_data;
  cm_orbital_model_t *omod;
  cm_rotational_model_t *rmod;
  double oparams[CM_PARAM_COUNT];
  double rparams[CM_PARAM_COUNT];

  double3 p;
  double3 v;
  double3 r;
  double W;
} cm_orbit_t;

typedef struct {
  double jde;
  double T; // Julian Centuries
  double d; // Days since J2000
} cm_world_t;

typedef void (*cm_orbital_model_init_t)(cm_orbital_model_t *);
typedef void (*cm_orbital_object_init_t)(cm_orbit_t *);
typedef void (*cm_orbital_model_step_t)(cm_orbital_model_t *, cm_world_t *);
typedef void (*cm_orbital_object_step_t)(cm_orbit_t *, cm_world_t *);
typedef void (*cm_orbital_model_transform_t)(cm_orbit_t *, cm_world_t *);

struct cm_orbital_model_t {
  const char *name;
  cm_orbital_object_init_t init_object;
  cm_orbital_model_step_t step_model;
  cm_orbital_object_step_t step_object;
  cm_orbital_model_transform_t transform;
  void *data;
};

typedef void (*cm_rotational_model_init_t)(cm_rotational_model_t *);
typedef void (*cm_rotational_model_step_t)(cm_rotational_model_t *, cm_world_t *);
typedef void (*cm_rotational_object_init_t)(cm_orbit_t *);
typedef void (*cm_rotational_object_step_t)(cm_orbit_t *, cm_world_t *);

struct cm_rotational_model_t {
  const char *name;
  cm_rotational_model_step_t step_model;
  cm_rotational_object_init_t init_object;
  cm_rotational_object_step_t step_object;
  void *data;
};


// Compute all orbits and rotational models for JDE
void cm_orbit_compute(double jde);


void cm_register_orbital_model(cm_orbital_model_t *model);
void cm_register_rotational_model(cm_rotational_model_t *model);
cm_orbital_model_t* cm_get_orbital_model(const char *name);
cm_rotational_model_t* cm_get_rotational_model(const char *name);


// Initialization methods
void elp2000_82b_init(void);
void goffin_j2000_init(void);
void vsop87_init(void);
void kepler_init(void);
void iau_rot_init(void);


// Methods have some useful public functions, that allows you to run the raw
// method for a given body
cm_state_vectors_t cm_vsop87(cm_body_id_t body, double jde);
double3 cm_lieske_e2x3(int sat, double jde);
double3 cm_goffin2000(double jde);
double3 cm_elp2000_82b(double jde);
double3 cm_kepler(cm_orbital_elements_t *orb, double jde);



#endif
