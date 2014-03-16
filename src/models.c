/*
 * Copyright (c) 2012,2013 Mattias Holm <lorrden(at)openorbit.org>.
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
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <celmek/celmek.h>

// TODO: Physical parameters like radius are ment to allow a bounding sphere to
//       be created. Therefore, the equatorial or mean radius values are not
//       correct and should be changed to a bounding sphere radius.
cm_orbit_t orbits[] = {
  {"sun",      NULL,   NULL, "vsop87",       "iau-std-rot",
    .oparams = {}, .rparams = {}, .mass = 1.989e30,
    .radius = 6.96342e8},
  {"mercury", "sun",   NULL, "vsop87",       "iau-std-rot",
    .oparams = {}, .rparams = {}, .mass = 3.301e23, .radius = 2439.7e3},
  {"venus",   "sun",   NULL, "vsop87",       "iau-std-rot",
    .oparams = {}, .rparams = {}, .mass = 4.1380e24, .radius = 6051.8e3},
  {"earth",   "sun",   NULL, "vsop87",       "iau-std-rot",
    .oparams = {}, .rparams = {}, .mass = 5.972e24, .radius = 6378.1e3},
  {"moon",    "earth", NULL, "elp2000-82b",  "iau-std-rot",
    .oparams = {}, .rparams = {}, .mass = 7.346e22, .radius = 1738.14e3},
  {"mars",    "sun",   NULL, "vsop87",       "iau-std-rot",
    .oparams = {}, .rparams = {}, .mass = 6.4273e23, .radius = 3396.2e3},
  {"jupiter", "sun",   NULL, "vsop87",       "iau-std-rot",
    .oparams = {}, .rparams = {}, .mass = 1.89852e27, .radius = 71492.0e3},
  {"saturn",  "sun",   NULL, "vsop87",       "iau-std-rot",
    .oparams = {}, .rparams = {}, .mass = 5.6846e26, .radius = 60268.0e3},
  {"uranus",  "sun",   NULL, "vsop87",       "iau-std-rot",
    .oparams = {}, .rparams = {}, .mass = 8.6819e25, .radius = 25559.0e3},
  {"neptune", "sun",   NULL, "vsop87",       "iau-std-rot",
    .oparams = {}, .rparams = {}, .mass = 1.02431e26, .radius = 24764.0e3},
  {"pluto",   "sun",   NULL, "pluto",        "iau-std-rot",
    .oparams = {}, .rparams = {}, .mass = 1.31e22, .radius = 1153.0e3},

  // Need to double check all the parameters, they have been entered very
  // hastilly to do performance tests.
  {"phobos", "mars", NULL, "kepler",          "iau-std-rot",
    .oparams = {0.31891023, 9377.2e3, 0.0151, 1.093}, .rparams = {},
    .mass = 1.072e16, .radius = 11.1e3},
  {"deimos", "mars", NULL, "kepler",          "iau-std-rot",
    .oparams = {1.26244, 23460e3, 0.0002, 0.93}, .rparams = {},
    .mass = 1.48e15, .radius = 6.2e3},

  {"europa",   "jupiter", NULL, "kepler",        "iau-std-rot",
    .oparams = {3.551181, 670900e3, 0.009, 0.470}, .rparams = {},
    .mass = 4.80e22, .radius = 1560.8e3},
  {"ganymede", "jupiter", NULL, "kepler",        "iau-std-rot",
    .oparams = {7.15455296, 1070400e3, 0.0013, 0.20}, .rparams = {},
    .mass = 1.48e23, .radius = 2634.1e3},
  {"io",       "jupiter", NULL, "kepler",        "iau-std-rot",
    .oparams = {1.769137786, 421700e3, 0.0041, 0.05}, .rparams = {},
    .mass = 8.93e22, .radius = 1821.3e3},
  {"callisto", "jupiter", NULL, "kepler",        "iau-std-rot",
    .oparams = {16.6890184, 1882700e3 ,0.0074, 0.192}, .rparams = {},
    .mass = 1.08e23, .radius = 2410.3e3},
  
  {"titan",    "saturn", NULL, "kepler",        "iau-std-rot",
    .oparams = {15.945, 1221870e3, 0.0288, 0.34854}, .rparams = {},
    .mass = 1.35e23, .radius = 2576.0e3},
  {"tethys",   "saturn", NULL, "kepler",        "iau-std-rot",
    .oparams = {1.887802, 294619e3, 0.0001, 1.12}, .rparams = {},
    .mass = 6.17449e20, .radius = 531.1e3},
  {"dione",    "saturn", NULL, "kepler",        "iau-std-rot",
    .oparams = {2.736915, 377396e3, 0.0022, 0.019}, .rparams = {},
    .mass = 1.095452e21, .radius = 561.4e3},
  {"rhea",     "saturn", NULL, "kepler",        "iau-std-rot",
    .oparams = {4.518212, 527108e3, 0.0012583, 0.345}, .rparams = {},
    .mass = 2.306518e21, .radius = 763.8e3},
  {"iapetus",  "saturn", NULL, "kepler",        "iau-std-rot",
    .oparams = {79.3215, 3560820e3, 0.0286125, 15.47}, .rparams = {},
    .mass = 1.805635e21, .radius = 734.5e3},
  
  {"titania",  "uranus", NULL, "kepler",        "iau-std-rot",
    .oparams = {8.706234, 435910e3, 0.0011, 0.340}, .rparams = {},
    .mass = 3.52e21, .radius = 788.4e3},
  {"oberon",   "uranus", NULL, "kepler",        "iau-std-rot",
    .oparams = {13.463234, 583520e3, 0.0014, 0.058}, .rparams = {},
    .mass = 3.01e21, .radius = 761.4e3},
  {"ariel",    "uranus", NULL, "kepler",        "iau-std-rot",
    .oparams = {2.520, 191020e3, 0.0012, 0.260}, .rparams = {},
    .mass = 1.353e21, .radius = 578.9e3},
  {"umbriel",  "uranus", NULL, "kepler",        "iau-std-rot",
    .oparams = {4.144, 266000e3, 0.0039, 0.128}, .rparams = {},
    .mass = 1.172e21, .radius = 584.7e3},
  {"miranda",  "uranus", NULL, "kepler",        "iau-std-rot",
    .oparams = {1.413479, 129390e3, 0.0013, 4.232}, .rparams = {},
    .mass = 6.59e19, .radius = 235.8e3},
  
  {"triton",   "neptune", NULL, "kepler",        "iau-std-rot",
    .oparams = {-5.876854, 354759e3, 0.000016, 156.885}, .rparams = {},
    .mass = 2.14e22, 1353.4e3},
  {"nereid",   "neptune", NULL, "kepler",        "iau-std-rot",
    .oparams = {360.1362, 5513787e3, 0.7507, 32.55}, .rparams = {},
    .mass = 3.1e19, .radius = 170e3},
};

size_t orbital_model_count;
size_t orbital_model_length;
cm_orbital_model_t **orbital_models;
size_t rot_model_count;
size_t rot_model_length;
cm_rotational_model_t **rot_models;

cm_orbits_t _orbits = {0,0,NULL};

void
cm_add_orbit(cm_orbit_t *orbit)
{
  if (_orbits.orbits == NULL) {
    _orbits.alen = 8;
    _orbits.orbits = calloc(8, sizeof(cm_orbit_t*));
  }
  if (_orbits.alen >= _orbits.len) {
    _orbits.alen *= 2;
    _orbits.orbits = realloc(_orbits.orbits, _orbits.alen*sizeof(cm_orbit_t*));
  }

  _orbits.orbits[_orbits.len ++] = orbit;
}

const cm_orbits_t*
cm_get_orbits(void)
{
  return &_orbits;
}

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
  orbit->orbit_plane_q = QD_IDENT;

  if (om) om->init_object(orbit);
  if (rm) rm->init_object(orbit);

  // Unless no GM is given, compute from mass
  if (orbit->GM == 0.0) orbit->GM = CM_G__M_KG_S * orbit->mass;

  orbit->parent = cm_get_orbit_by_name(orbit->parent_name);
}

void
cm_init_orbits(void)
{
  for (int i = 0 ; i < sizeof(orbits)/sizeof(cm_orbit_t) ; i++) {
    cm_orbit_t *o = &orbits[i];
    cm_init_orbit(o);

    cm_add_orbit(o);
  }
}


double
cm_julian_centuries(double jde)
{
  return (jde - CM_J2000_0) / CM_JD_PER_CENT;
}

void
cm_orbit_compute(double jde)
{
  cm_world_t  state = {.jde = jde, .T = cm_julian_centuries(jde),
                       .d = jde - CM_J2000_0};

  // First, do the presteps so that models can compute common parameters
  for (int i = 0 ; i < orbital_model_count ; i++) {
    orbital_models[i]->step_model(orbital_models[i], &state);
  }

  for (int i = 0 ; i < rot_model_count ; i++) {
    rot_models[i]->step_model(rot_models[i], &state);
  }

  // Compute positions
  for (int i = 0 ; i < _orbits.len ; i++) {
    cm_orbit_t *o = _orbits.orbits[i];
    o->omod->step_object(o, &state);
  }

  // And rotations
  for (int i = 0 ; i < _orbits.len ; i++) {
    cm_orbit_t *o = _orbits.orbits[i];
    if (o->rmod && o->rmod_data) o->rmod->step_object(o, &state);
  }

  // Last thing to do is to execute the transform functions, this
  // function can be used to transform positions and rotations, this also fills
  // in the epoch value
  for (int i = 0 ; i < _orbits.len ; i++) {
    cm_orbit_t *o = _orbits.orbits[i];
    if (o->omod && o->omod->transform) o->omod->transform(o, &state);
    o->epoch = jde;
  }
}

quatd_t
cm_orbit_get_bodyq(cm_orbit_t *orbit)
{
  // Quaternion for the body
  return orbit->q;
}

quatd_t
cm_orbit_get_orbitq(cm_orbit_t *orbit)
{
  assert(orbit->parent);
  // Compute the quaternion of the orbit
  cm_state_vectors_t sv;
  sv.p = orbit->p - orbit->parent->p;
  sv.v = orbit->v - orbit->parent->v;
  sv.epoch = orbit->epoch;
  cm_kepler_elements_t ke;

  cm_state_vector_to_orbital_elements(&ke, &sv, orbit->parent->GM + orbit->GM);

  //printf("orbit rotation %s: %f %f %f\n",
  //       orbit->name,
  //       ke.long_asc*VMATH_DEG_PER_RAD,
  //       ke.incl*VMATH_DEG_PER_RAD,
  //       ke.arg_peri*VMATH_DEG_PER_RAD);
  quatd_t q = orbit->orbit_plane_q;
  q = qd_mul(q, qd_rot(0, 0, 1, ke.long_asc));
  q = qd_mul(q, qd_rot(1, 0, 0, ke.incl));
  q = qd_mul(q, qd_rot(0, 0, 1, ke.arg_peri));
  q = qd_normalise(q);

  return q;
}

static int _initialized;
void
cm_init(void)
{
  if (_initialized) return;
  _initialized = 1;

  cm_mean_orbits_init();

  // Orbital models
  vsop87_init();
  elp2000_82b_init();
  goffin_j2000_init();
  //  lieske_e2x3_init();
  //de4xx_init();
  kepler_init();
  gust86_init();

  // Rotational models
  iau_rot_init();

  cm_init_models();
  cm_init_orbits();

  // Ensure we have a reasonable initial state.
  cm_orbit_compute(CM_J2000_0);
}
