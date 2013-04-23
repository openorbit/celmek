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


#ifndef celmek_cm_astro_h
#define celmek_cm_astro_h

#include <stdbool.h>

// Useful physical constants

#define CM_AU_IN_M 149597870700.0
#define CM_SEC_PER_DAY 86400.0

#define CM_MU 6.67384e-11

// Planets
#define CM_GM_SUN_KM 132712440018.0
#define CM_GM_SUN    1.32712440018e20
#define CM_GM_MERCURY_KM 22032.0
#define CM_GM_VENUS_KM 324859.0
#define CM_GM_EARTH_KM 398600.4418// km^3 / s^-2
#define CM_GM_MARS_KM 42828.0
#define CM_GM_JUPITER_KM 126686534.0
#define CM_GM_SATURN_KM 37931187.0
#define CM_GM_URANUS_KM 5793939.0
#define CM_GM_NEPTUNE_KM 6836529.0

// Moons
#define CM_GM_MOON_KM 4902.7779

// Dwarf planets
#define CM_GM_CERES_KM 63.1
#define CM_GM_PLUTO_KM 871.0
#define CM_GM_ERIS_KM 1108.0

/*!
 * cm_body_id_t provides a stable body IDs used by the different methods of
 * celmek. Methods may require a body reference will accept these values when one
 * refers to one of the more known bodies.
 *
 * Note that stable here means that the symbolic constants can always be used,
 * not that the underlying integer representation will remain constant. However,
 * it can be assumed that the sun and planet symbols are stable even in binary
 * representation.
 */
typedef enum {
  // Planets
  CM_SUN,
  CM_MERCURY,
  CM_VENUS,
  CM_EARTH,
  CM_MARS,
  CM_JUPITER,
  CM_SATURN,
  CM_URANUS,
  CM_NEPTUNE,

  // Special
  CM_SSB, // Solar system barycenter
  CM_EMB, // Earth-Moon barycenter
  // Moons
  CM_MOON,

  // Mars moons
  CM_PHOBOS,
  CM_DEIMOS,

  // Jovian moons
  CM_IO,
  CM_EUROPA,
  CM_GANYMEDE,
  CM_CALLISTO,
  CM_AMALTHEA,
  CM_HIMALIA,
  CM_ELARA,
  CM_PASIPHAE,
  CM_SINOPE,
  CM_LYSITHEA,
  CM_CARME,
  CM_ANANKE,
  CM_LEDA,
  CM_THEBE,
  CM_ADRASTEA,
  CM_METIS,
  CM_CALLIRRHOE,
  CM_THEMISTO,
  CM_MEGACLITE,
  CM_TAYGETE,
  CM_CHALDENE,
  CM_HARPALYKE,
  CM_KALYKE,
  CM_IOCASTE,
  CM_ERINOME,
  CM_ISONOE,
  CM_PRAXIDIKE,
  CM_AUTONOE,
  CM_THYONE,
  CM_HERMIPPE,
  CM_AITNE,
  CM_EURYDOME,
  CM_EUANTHE,
  CM_EUPORIE,
  CM_ORTHOSIE,
  CM_SPONDE,
  CM_KALE,
  CM_PASITHEE,
  CM_HEGEMONE,
  CM_MNEME,
  CM_AOEDE,
  CM_THELXINOE,
  CM_ARCHE,
  CM_KALLICHORE,
  CM_HELIKE,
  CM_CARPO,
  CM_EUKELADE,
  CM_CYLLENE,
  CM_KORE,
  CM_HERSE,


  // Saturnian moons
  CM_MIMAS,
  CM_ENCELADUS,
  CM_TETHYS,
  CM_DIONE,
  CM_RHEA,
  CM_TITAN,
  CM_HYPERION,
  CM_IAPETUS,
  CM_PHOEBE,
  CM_JANUS,
  CM_EPIMETHEUS,
  CM_HELENE,
  CM_TELESTO,
  CM_CALYPSO,
  CM_ATLAS,
  CM_PROMETHEUS,
  CM_PANDORA,
  CM_PAN,
  CM_YMIR,
  CM_PAALIAQ,
  CM_TARVOS,
  CM_IJIRAQ,
  CM_SUTTUNGR,
  CM_KIVIUQ,
  CM_MUNDILFARI,
  CM_ALBIORIX,
  CM_SKATHI,
  CM_ERRIAPUS,
  CM_SIARNAQ,
  CM_THRYMR,
  CM_NARVI,
  CM_METHONE,
  CM_PALLENE,
  CM_POLYDEUCES,
  CM_DAPHNIS,
  CM_AEGIR,
  CM_BEBHIONN,
  CM_BERGELMIR,
  CM_BESTLA,
  CM_FARBAUTI,
  CM_FENRIR,
  CM_FORNJOT,
  CM_HATI,
  CM_HYRROKKIN,
  CM_KARI,
  CM_LOGE,
  CM_SKOLL,
  CM_SURTUR,
  CM_ANTHE,
  CM_JARNSAXA,
  CM_GREIP,
  CM_TARQEQ,
  CM_AEGAEON,

  // Uranian moons
  CM_ARIEL,
  CM_UMBRIEL,
  CM_TITANIA,
  CM_OBERON,
  CM_MIRANDA,
  CM_CORDELIA,
  CM_OPHELIA,
  CM_BIANCA,
  CM_CRESSIDA,
  CM_DESDEMONA,
  CM_JULIET,
  CM_PORTIA,
  CM_ROSALIND,
  CM_BELINDA,
  CM_PUCK,
  CM_CALIBAN,
  CM_SYCORAX,
  CM_PROSPERO,
  CM_SETEBOS,
  CM_STEPHANO,
  CM_TRINCULO,
  CM_FRANCISCO,
  CM_MARGARET,
  CM_FERDINAND,
  CM_PERDITA,
  CM_MAB,
  CM_CUPID,

  // Neptunian moons
  CM_TRITON,
  CM_NEREID,
  CM_NAIAD,
  CM_THALASSA,
  CM_DESPINA,
  CM_GALATEA,
  CM_LARISSA,
  CM_PROTEUS,
  CM_HALIMEDE,
  CM_PSAMATHE,
  CM_SAO,
  CM_LAOMEDEIA,
  CM_NESO,

  // Dwarf planets (and their sats)
  CM_CERES,
  CM_PLUTO,
  CM_CHARON,
  CM_NIX,
  CM_HYDRA,
  CM_ERIS,
  CM_DYSNOMIA,
  CM_HAUMEA,
  CM_HIIAKA,
  CM_NAMAKA,
  CM_MAKEMAKE,

} cm_body_id_t;



cm_body_id_t cm_body_id_from_name(const char *name);

/*!
 * Initialise all fields in the orbital elements object to NaN.
 *
 * \param[out] oe Orbital elements object to initialise.
 */
void cm_orbital_elements_init(cm_kepler_elements_t *oe);

/*!
 * Compute semiminor axis from semimajor axis and eccentricity.
 *
 * \param semimaj Semimajor axis
 * \param ecc Eccentricity of the orbital ellipse.
 * \result Semiminor axis.
 */
double cm_semiminor_axis(double semimaj, double ecc);

/*!
 * Compute orbital period for an object.
 *
 * \param semimaj Semimajor axis of orbit.
 * \param GM1 Gravitational mass constant for center object.
 * \param GM2 Gravitational mass constant for the orbiting object.
 * \result Period of objects orbit. Unit depends on GM1 and GM2.
 */
double cm_period(double semimaj, double GM1, double GM2);

/*!
 * Compute mean anomaly for a given epoch.
 *
 * Computes the mean anomaly for the epoch (given in JDE / TT). Note that
 * for precision reasons, it may make sense to keep the epoch within one period
 * of the time of periapsis passage.
 *
 * \param period The period of the orbit.
 * \param t_peri Time of periapsis passage.
 * \param epoch Time for which you want the mean anomaly.
 * \result Mean anomaly at epoch.
 */
double cm_mean_anomaly(double period, double t_peri, double epoch);

/*!
 * Compute the time since periapsis.
 *
 * Given the mean anomaly and the mean motion, we can compute the time passed
 * since periapsis. The funciton is actually very simple as the time passed is
 * the mean anomaly divided by the mean motion.
 *
 * \param mean_anomaly Mean anomaly (usually in radians)
 * \param mean_motion Mean motion (usually in radians / day)
 * \result Time passed since periapsis passage (usually in days).
 */
double cm_time_since_periapsis(double mean_anomaly, double mean_motion);

/*!
 * Compute mean motion with respect to period. Unit will be radians per time unit.
 *
 * \param period Period of orbit.
 * \result Mean motion in radians per time unit of period.
 */
double cm_mean_motion(double period);

/*!
 * Compute longitude of periapsis
 *
 * \param arg_peri Argument of periapsis
 * \param long_asc Longitude of ascending node
 * \result Longitude of periapsis
 */
double cm_long_periapsis(double arg_peri, double long_asc);

/*!
 * Compute argument of periapsis
 *
 * \param long_peri Longitude of periapsis
 * \param long_asc Longitude of ascending node
 * \result Argument of periapsis.
 */
double cm_arg_periapsis(double long_peri, double long_asc);

/*!
 * Check consistency of an orbital elements object.
 *
 * \param[in] oe Orbital elements object.
 * \result
 *   - true: if the object is consistent.
 *   - false: if the object is NOT consistent.
 */
bool cm_orbital_elements_check(const cm_kepler_elements_t *oe);

/*!
 * Compute mean orbital elements at date.
 *
 * The tables used by the function is given by Jean Meeus,
 * Astronomical Algorithms, chapter 30.
 *
 * \param[out] elems The mean orbital element object to fill in.
 * \param planet Planet id, the valid values are all major planets.
 * \param T Julian day.
 */
void cm_mean_orbital_elements_at_date(cm_kepler_elements_t *elems,
                                      cm_body_id_t planet, double T);

/*!
 * Compute mean longitude at date.
 *
 * The tables used by the function is given by Jean Meeus,
 * Astronomical Algorithms, chapter 30.
 *
 * \param planet Planet id, the valid values are all major planets.
 * \param T Julian day.
 * \result The mean longitude at date T
 */
double cm_mean_long_at_date(cm_body_id_t planet, double T);

/*!
 * Compute semimajor axis at date.
 *
 * The tables used by the function is given by Jean Meeus,
 * Astronomical Algorithms, chapter 30.
 *
 * \param planet Planet id, the valid values are all major planets.
 * \param T Julian day.
 * \result The semimajor axis at date T.
 */
double cm_semimajor_at_date(cm_body_id_t planet, double T);

/*!
 * Compute eccentricity at date.
 *
 * The tables used by the function is given by Jean Meeus,
 * Astronomical Algorithms, chapter 30.
 *
 * \param planet Planet id, the valid values are all major planets.
 * \param T Julian day.
 * \result The eccentricity at date T.
 */
double cm_eccentricity_at_date(cm_body_id_t planet, double T);

/*!
 * Compute inclination towards the ecliptic at date.
 *
 * The tables used by the function is given by Jean Meeus,
 * Astronomical Algorithms, chapter 30.
 *
 * \param planet Planet id, the valid values are all major planets.
 * \param T Julian day.
 * \result The inclination towards the ecliptic at date T.
 */
double cm_incl_to_ecl_at_date(cm_body_id_t planet, double T);

/*!
 * Compute longitude of ascending node at date.
 *
 * The tables used by the function is given by Jean Meeus,
 * Astronomical Algorithms, chapter 30.
 *
 * \param planet Planet id, the valid values are all major planets.
 * \param T Julian day.
 * \result The longitude of the ascending node at date T.
 */
double cm_long_of_asc_node_at_date(cm_body_id_t planet, double T);

/*!
 * Compute longitude of periapsis at date.
 *
 * The tables used by the function is given by Jean Meeus,
 * Astronomical Algorithms, chapter 30.
 *
 * \param planet Planet id, the valid values are all major planets.
 * \param T Julian day.
 * \result The longitude of the periapsis at date T.
 */
double cm_long_of_peri_at_date(cm_body_id_t planet, double T);

/*!
 * Compute the mean orbital elements of a given planetary body with respect to
 * the J2000.0 frame.
 *
 * \param[out] elems The elements object to fill in.
 * \param planet The planetary body id. Valid value is all major planets.
 * \param T Time as julian day (JDE).
 */
void
cm_compute_mean_orbital_elements_j2000(cm_kepler_elements_t *elems,
                                       cm_body_id_t planet, double T);

/*!
 * Computes the mean longitude of a planet with respect to J2000.
 *
 * \param planet Planet id, the valid values are all major planets.
 * \param T Time as julian day.
 * \result The mean longitude in the J2000 frame.
 */
double cm_mean_long_j2000(cm_body_id_t planet, double T);

/*!
 * Computes the semimajor axis of a planet with respect to J2000.
 *
 * \param planet Planet id, the valid values are all major planets.
 * \param T Time as julian day.
 * \result The semimajor axis in the J2000 frame.
 */
double cm_semimajor_j2000(cm_body_id_t planet, double T);

/*!
 * Computes the eccentricity of a planet with respect to J2000.
 *
 * \param planet Planet id, the valid values are all major planets.
 * \param T Time as julian day.
 * \result The eccentricity in the J2000 frame.
 */
double cm_eccentricity_j2000(cm_body_id_t planet, double T);

/*!
 * Computes the inclination towards the ecliptic  of a planet with respect to
 * J2000.
 *
 * \param planet Planet id, the valid values are all major planets.
 * \param T Time as julian day.
 * \result The inclination towards the ecliptic in the J2000 frame.
 */
double cm_incl_to_ecl_j2000(cm_body_id_t planet, double T);

/*!
 * Computes the longitude of the ascending node  of a planet with respect to
 * J2000.
 *
 * \param planet Planet id, the valid values are all major planets.
 * \param T Time as julian day.
 * \result The longitude of the ascending node in the J2000 frame.
 */
double cm_long_of_asc_node_j2000(cm_body_id_t planet, double T);

/*!
 * Computes the longitude of periapsis of a planet with respect to J2000.
 *
 * \param planet Planet id, the valid values are all major planets.
 * \param T Time as julian day.
 * \result The longitude of periapsis in the J2000 frame.
 */
double cm_long_of_peri_j2000(cm_body_id_t planet, double T);

/*!
 * Convert state vectors to orbital elements.
 *
 * \param[out] oe The orbital elements.
 * \param[in] sv The state vectors to convert to orbital elements.
 * \param GM Gravitational parameter of central body.
 */
void cm_state_vector_to_orbital_elements(cm_kepler_elements_t *oe,
                                         const cm_state_vectors_t *sv,
                                         double GM);

void cm_mean_orbits_init(void);

#endif
