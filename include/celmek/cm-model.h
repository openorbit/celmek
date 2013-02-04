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


#ifndef celmek_cm_model_h
#define celmek_cm_model_h

typedef struct cm_orbital_model_t cm_orbital_model_t;
typedef struct cm_rotational_model_t cm_rotational_model_t;

#define CM_PARAM_COUNT 10

// "earth", "vsop87" "iau"
/*!
 * Orbital model interface structure.
 *
 * The cm_orbit_t structure is used to store information about an objects orbital
 * and rotational models and quantities. The structure tracks the name of the
 * object and the two active models (one orbital and one rotational).
 */
typedef struct cm_orbit_t {
  const char *name; //!< Name of body e.g. 'Venus'
  const char *parent_name; //!< Parent body name (used for scheduling)
  struct cm_orbit_t *parent; //!< Pointer to parent, filled in automatically

  const char *omod_name; //!< Name of orbital model to use
  const char *rmod_name; //!< Name of rotational model to use

  void *omod_data; //!< For use by orbital model
  void *rmod_data; //!< For use by rotational model
  cm_orbital_model_t *omod; //!< The orbital model handling the body.
  cm_rotational_model_t *rmod; //!< The rotational model of the object.
  double oparams[CM_PARAM_COUNT]; //!< Parameters to orbital model
  double rparams[CM_PARAM_COUNT]; //!< Parameters to rotational model

  double3 p; //!< Position
  double3 v; //!< Velocity
  double3 r; //!< Rotational vector
  double W;  //!< Rotation around vector
} cm_orbit_t;

/*!
 * World state computed once and used for all objects being updated.
 */
typedef struct {
  double jde; //!< Julian day
  double T;   //!< Julian Centuries
  double d;   //!< Days since J2000
} cm_world_t;

typedef void (*cm_orbital_model_init_t)(cm_orbital_model_t *);
typedef void (*cm_orbital_object_init_t)(cm_orbit_t *);
typedef void (*cm_orbital_model_step_t)(cm_orbital_model_t *, cm_world_t *);
typedef void (*cm_orbital_object_step_t)(cm_orbit_t *, cm_world_t *);
typedef void (*cm_orbital_model_transform_t)(cm_orbit_t *, cm_world_t *);

/*!
 * Orbital model plugin structure
 *
 * Different orbtial models are needed (e.g. VSOP87 and ELP2000 are two
 * different models) but they need a common interface. This structure allows the
 * models to register different step and initialisation functions.
 */
struct cm_orbital_model_t {
  const char *name; //!< Name of orbital model (matched to the cm_orbit__t name
                    //!< field)
  cm_orbital_object_init_t init_object; //!< Function to initialise an orbital
                                        //!< body
  cm_orbital_model_step_t step_model; //!< Model step function (should compute
                                      //!< common data for all or some objects)
  cm_orbital_object_step_t step_object; //!< Function to step one object.
  cm_orbital_model_transform_t transform; //!< Post step function (where model
                                          //!< may apply additional
                                          //!< transformations)
  void *data; //!< Model specific data
};

typedef void (*cm_rotational_model_init_t)(cm_rotational_model_t *);
typedef void (*cm_rotational_model_step_t)(cm_rotational_model_t *, cm_world_t *);
typedef void (*cm_rotational_object_init_t)(cm_orbit_t *);
typedef void (*cm_rotational_object_step_t)(cm_orbit_t *, cm_world_t *);

/*!
 * Rotational model plugin structure
 *
 * Different rotational models exist. For example the IAU model, DExxx for the
 * moon and other plain rotation models when nothing else exists.
 */
struct cm_rotational_model_t {
  const char *name; //!< Name of rotational model
  cm_rotational_model_step_t step_model; //!< Function to compute common data
                                         //!< for the entire model
  cm_rotational_object_init_t init_object; //!< Initialise one object
  cm_rotational_object_step_t step_object; //!< Stepping one objects rot model
  void *data; //!< Model specific data
};


/*!
 * Run the entire simulation and compute all body positions and rotations for a
 * given Julian day.
 *
 * \param jde Julian Day to compute planetary positions and rotations for.
 */
void cm_orbit_compute(double jde);

/*!
 * Register an orbital model plugin with the model registry.
 *
 * \param model Orbital model to register
 */
void cm_register_orbital_model(cm_orbital_model_t *model);

/*!
 * Register a rotational model plugin with the model registry.
 *
 * \param model Rotational model to register.
 */
void cm_register_rotational_model(cm_rotational_model_t *model);

/*!
 * Locate orbital model by name.
 *
 * \param name Name of orbital model.
 * \result
 *   - Pointer to the orbital model with the given name if it exists.
 *   - NULL if the model cannot be found.
 */
cm_orbital_model_t* cm_get_orbital_model(const char *name);

/*!
 * Locate rotational model by name.
 *
 * \param name Name of rotational model.
 * \result
 *   - Pointer to the rotational model with the given name if it exists.
 *   - NULL if the model cannot be found.
 */
cm_rotational_model_t* cm_get_rotational_model(const char *name);



// Initialization methods

/*!
 * Initialises the elp2000 lunar simulation model.
 */
void elp2000_82b_init(void);

/*!
 * Initialises the Goffin pluto simulation model.
 */
void goffin_j2000_init(void);

/*!
 * Initialises the VSOP87 planetary orbit model.
 */
void vsop87_init(void);

/*!
 * Initialises the kepler orbit model.
 */
void kepler_init(void);

/*!
 * Initialises the IAU rotation model.
 */
void iau_rot_init(void);


// Methods have some useful public functions, that allows you to run the raw
// method for a given body.

/*!
 * Run VSOP87-E for a given planetary body.
 *
 * VSOP87-E produces a state vector with dynamical coordinates and velocities.
 * This function returns these quantities in AU and AU/day and the reference
 * frame is that which is used by the VSOP87 documentation. The function exist to
 * be easy to integrate in test code primarily, but is also useful in a stand
 * alone manner.
 *
 * \param body Body to run the test for. Valid bodies are CM_SUN, CM_MERCURY,
 *             CM_EARTH, CM_MARS, CM_JUPITER, CM_SATURN, CM_URANUS and
 *             CM_NEPTUNE.
 * \param jde Julian Day in ephemeris time (TT/TBD).
 * \result State vectors for the body at the given time.
 */
cm_state_vectors_t cm_vsop87(cm_body_id_t body, double jde);

/*!
 * Run Lieske Jovian moon simulation in stand alone mode.
 *
 * \param sat Jovian satellite number.
 *            - 0: Jupiter I = Io
 *            - 1: Jupiter II = Europa
 *            - 2: Jupiter III = Ganymede
 *            - 3: Jupiter IV = Callisto
 * \param jde Julian day for the given satellite.
 * \result Position of satellite for the given day.
 */
double3 cm_lieske_e2x3(int sat, double jde);

/*!
 * Run Goffin Pluto simulation for J2000.
 *
 * \param jde Julian day for which to compute the position of Pluto.
 * \result Position of pluto for the given julian day.
 */
double3 cm_goffin2000(double jde);

/*!
 * Run ELP2000-82b lunar model.
 *
 * \param jde The julian day to compute the moons position for.
 * \result The lunar position for the givien Julian day.
 */
double3 cm_elp2000_82b(double jde);

/*!
 * Compute kepler orbital position for a given time.
 *
 * \param orb Orbital elements to use for computations.
 * \param jde Julian for computed orbital position.
 * \result Position of the body with the orbital elements and the given Julian
 *         day.
 */
double3 cm_kepler(cm_orbital_elements_t *orb, double jde);



#endif
