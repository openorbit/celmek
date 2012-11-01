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

#include <stdlib.h>
#include <string.h>
#include <stdio.h>

#include <celmek/celmek.h>

cm_orbit_t orbits[] = {
  {"sun",      NULL,   NULL, "vsop87",       "iau-std-rot", .oparams = {}, .rparams = {}},
  {"mercury", "sun",   NULL, "vsop87",       "iau-std-rot", .oparams = {}, .rparams = {}},
  {"venus",   "sun",   NULL, "vsop87",       "iau-std-rot", .oparams = {}, .rparams = {}},
  {"earth",   "sun",   NULL, "vsop87",       "iau-std-rot", .oparams = {}, .rparams = {}},
  {"moon",    "earth", NULL, "elp2000-82b",  "iau-std-rot", .oparams = {}, .rparams = {}},
  {"mars",    "sun",   NULL, "vsop87",       "iau-std-rot", .oparams = {}, .rparams = {}},
  {"jupiter", "sun",   NULL, "vsop87",       "iau-std-rot", .oparams = {}, .rparams = {}},
  {"saturn",  "sun",   NULL, "vsop87",       "iau-std-rot", .oparams = {}, .rparams = {}},
  {"uranus",  "sun",   NULL, "vsop87",       "iau-std-rot", .oparams = {}, .rparams = {}},
  {"neptune", "sun",   NULL, "vsop87",       "iau-std-rot", .oparams = {}, .rparams = {}},
  {"pluto",   "sun",   NULL, "pluto",        "iau-std-rot", .oparams = {}, .rparams = {}},

  // Need to double check all the parameters, they have been entered very
  // hastilly to do performance tests
  {"phobos", "mars", NULL, "kepler",        "iau-std-rot",
   .oparams = {0.31891023, 9377.2, 0.0151, 1.093}, .rparams = {}},
  {"deimos", "mars", NULL, "kepler",        "iau-std-rot",
   .oparams = {1.26244,23460, 0.0002, 0.93}, .rparams = {}},

  {"europa",   "jupiter", NULL, "kepler",        "iau-std-rot",
   .oparams = {3.551181, 670900, 0.009, 0.470}, .rparams = {}},
  {"ganymede", "jupiter", NULL, "kepler",        "iau-std-rot",
   .oparams = {7.15455296, 1070400, 0.0013, 0.20}, .rparams = {}},
  {"io",       "jupiter", NULL, "kepler",        "iau-std-rot",
   .oparams = {1.769137786, 421700,0.0041, 0.05}, .rparams = {}},
  {"callisto", "jupiter", NULL, "kepler",        "iau-std-rot",
   .oparams = {16.6890184, 1882700,0.0074, 0.192}, .rparams = {}},
  
  {"titan",    "saturn", NULL, "kepler",        "iau-std-rot",
   .oparams = {15.945, 1221870, 0.0288, 0.34854}, .rparams = {}},
  {"tethys",   "saturn", NULL, "kepler",        "iau-std-rot",
   .oparams = {1.887802, 294619, 0.0001, 1.12}, .rparams = {}},
  {"dione",    "saturn", NULL, "kepler",        "iau-std-rot",
   .oparams = {2.736915, 377396, 0.0022, 0.019}, .rparams = {}},
  {"rhea",     "saturn", NULL, "kepler",        "iau-std-rot",
   .oparams = {4.518212, 527108, 0.0012583, 0.345}, .rparams = {}},
  {"iapetus",  "saturn", NULL, "kepler",        "iau-std-rot",
   .oparams = {79.3215, 3560820, 0.0286125, 15.47}, .rparams = {}},
  
  {"titania",  "uranus", NULL, "kepler",        "iau-std-rot",
   .oparams = {8.706234, 435910, 0.0011, 0.340}, .rparams = {}},
  {"oberon",   "uranus", NULL, "kepler",        "iau-std-rot",
   .oparams = {13.463234, 583520, 0.0014, 0.058}, .rparams = {}},
  {"ariel",    "uranus", NULL, "kepler",        "iau-std-rot",
   .oparams = {2.520, 191020, 0.0012, 0.260}, .rparams = {}},
  {"umbriel",  "uranus", NULL, "kepler",        "iau-std-rot",
   .oparams = {4.144, 266000, 0.0039, 0.128}, .rparams = {}},
  {"miranda",  "uranus", NULL, "kepler",        "iau-std-rot",
   .oparams = {1.413479, 129390, 0.0013, 4.232}, .rparams = {}},
  
  {"triton",   "neptune", NULL, "kepler",        "iau-std-rot",
   .oparams = {-5.876854, 354759, 0.000016, 156.885}, .rparams = {}},
  {"nereid",   "neptune", NULL, "kepler",        "iau-std-rot",
   .oparams = {360.1362, 5513787, 0.7507, 32.55}, .rparams = {}},
};

size_t orbital_model_count;
size_t orbital_model_length;
cm_orbital_model_t **orbital_models;
size_t rot_model_count;
size_t rot_model_length;
cm_rotational_model_t **rot_models;

// TODO: Check for malloc errors
void
cm_register_orbital_model(cm_orbital_model_t *model)
{
  printf("register: %s\n", model->name);
  if (orbital_models == NULL) {
    orbital_models = calloc(8, sizeof(cm_orbital_model_t*));
    orbital_model_count = 0;
    orbital_model_length = 8;
  }

  if (orbital_model_count >= orbital_model_length) {
    orbital_models = realloc(orbital_models,
                             (orbital_model_length+8)*sizeof(cm_orbital_model_t*));
    orbital_model_length += 8;
  }

  orbital_models[orbital_model_count++] = model;
}

void
cm_register_rotational_model(cm_rotational_model_t *model)
{
  if (rot_models == NULL) {
    rot_models = calloc(8, sizeof(cm_rotational_model_t*));
    rot_model_count = 0;
    rot_model_length = 8;
  }

  if (rot_model_count >= rot_model_length) {
    rot_models = realloc(rot_models,
                         (rot_model_length+8)*sizeof(cm_rotational_model_t*));
    rot_model_length += 8;
  }

  rot_models[rot_model_count++] = model;  
}

int
orb_model_comp(const void *a, const void *b)
{
  const cm_orbital_model_t * const * app = a;
  const cm_orbital_model_t * const * bpp = b;
  const cm_orbital_model_t * ap = *app;
  const cm_orbital_model_t * bp = *bpp;

  return strcmp(ap->name, bp->name);
}

int
rot_model_comp(const void *a, const void *b)
{
  const cm_rotational_model_t * const * app = a;
  const cm_rotational_model_t * const * bpp = b;
  const cm_rotational_model_t * ap = *app;
  const cm_rotational_model_t * bp = *bpp;

  return strcmp(ap->name, bp->name);
}


cm_orbital_model_t*
cm_get_orbital_model(const char *name)
{
  cm_orbital_model_t tmp = {.name = name};
  cm_orbital_model_t *tmp2 = &tmp;

  cm_orbital_model_t **found = bsearch(&tmp2, orbital_models,
                                       orbital_model_count,
                                       sizeof(cm_orbital_model_t *),
                                       orb_model_comp);
  return found ? *found : NULL;
}

cm_rotational_model_t*
cm_get_rotational_model(const char *name)
{
  cm_rotational_model_t tmp = {.name = name};
  cm_rotational_model_t *tmp2 = &tmp;
  cm_rotational_model_t **found = bsearch(&tmp2, rot_models, rot_model_count,
                                          sizeof(cm_rotational_model_t *),
                                          rot_model_comp);
  return found ? *found : NULL;
}


void
cm_init_models(void)
{
  qsort(orbital_models, orbital_model_count, sizeof(cm_orbital_model_t*),
        orb_model_comp);
  qsort(rot_models, rot_model_count, sizeof(cm_rotational_model_t*),
        rot_model_comp);
}

// This function is ridiculously slow, so it should be rewritten at some point
// Maintain sorted array or avl-tree of orbits.
cm_orbit_t*
cm_get_orbit_by_name(const char *name)
{
  if (!name) return NULL;
  for (int i = 0 ; i < sizeof(orbits)/sizeof(cm_orbit_t) ; i ++) {
    if (!strcmp(orbits[i].name, name)) return &orbits[i]; 
  }
  return NULL;
}

void
cm_init_orbit(cm_orbit_t *orbit)
{
  cm_orbital_model_t *om = cm_get_orbital_model(orbit->omod_name);
  cm_rotational_model_t *rm = cm_get_rotational_model(orbit->rmod_name);

  orbit->omod = om;
  orbit->rmod = rm;

  if (om) om->init_object(orbit);
  if (rm) rm->init_object(orbit);

  orbit->parent = cm_get_orbit_by_name(orbit->parent_name);
}

void
cm_init_orbits(void)
{
  for (int i = 0 ; i < sizeof(orbits)/sizeof(cm_orbit_t) ; i++) {
    cm_orbit_t *o = &orbits[i];
    cm_init_orbit(o);
  }
}


double
cm_julian_centuries(double jde)
{
  return (jde - 2451545.0) / 36525.0;  
}

void
cm_orbit_compute(double jde)
{
  cm_world_t  state = {.jde = jde, .T = cm_julian_centuries(jde),
                       .d = jde - 2451545.0};

  // First, do the presteps so that models can compute common parameters
  for (int i = 0 ; i < orbital_model_count ; i++) {
    orbital_models[i]->step_model(orbital_models[i], &state);
  }

  for (int i = 0 ; i < rot_model_count ; i++) {
    rot_models[i]->step_model(rot_models[i], &state);
  }

  // Compute positions
  for (int i = 0 ; i < sizeof(orbits)/sizeof(cm_orbit_t) ; i++) {
    cm_orbit_t *o = &orbits[i];
    o->omod->step_object(o, &state);
  }

  // And rotations
  for (int i = 0 ; i < sizeof(orbits)/sizeof(cm_orbit_t) ; i++) {
    cm_orbit_t *o = &orbits[i];
    if (o->rmod && o->rmod_data) o->rmod->step_object(o, &state);
  }

  // Last thing to do is to execute the transform functions, this
  // function can be used to transform positions and rotations
  for (int i = 0 ; i < sizeof(orbits)/sizeof(cm_orbit_t) ; i++) {
    cm_orbit_t *o = &orbits[i];
    if (o->omod && o->omod->transform) o->omod->transform(o, &state);
  }
}

void
cm_init(void)
{
  // Orbital models
  vsop87_init();
  elp2000_82b_init();
  goffin_j2000_init();
  //  lieske_e2x3_init();
  //de4xx_init();
  kepler_init();

  // Rotational models
  iau_rot_init();

  cm_init_models();
  cm_init_orbits();
}
