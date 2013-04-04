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

#include <stdlib.h>
#include <stdio.h>
#include <check.h>
#include <celmek/celmek.h>


#define ALMOST_EQUAL(a, b, p) (a >= b - p && a <= b + p)
#define REL_EQUAL(a, b, p) ((a != 0.0) ? fabs((a - b)/a) <= p : ((b != 0) ? fabs((a - b)/a) <= p : 1))

struct {
  const char *date_str;
  cm_date_t expect;
} packed_date_data[] = {
  {"K129U", {2012,  9, 30}},
  {"J609N", {1960,  9, 23}},
  {"J739F", {1973,  9, 15}},
  {"J77AO", {1977, 10, 24}},
  {"J77A4", {1977, 10,  4}},
  {"J9611", {1996,  1,  1}},
  {"J961A", {1996,  1, 10}},
  {"J969U", {1996,  9, 30}},
  {"J96A1", {1996, 10,  1}},
  {"K01AM", {2001, 10, 22}},
};

/*!
 * \test Test reading of packed dates from the MPC files.
 */
START_TEST(test_packed_dates)
{
  cm_date_t date;

  for (int i = 0 ; i < sizeof(packed_date_data)/sizeof(packed_date_data[0]) ;
       i++) {
    cm_read_packed_date(&date, packed_date_data[i].date_str);

    fail_unless(date.year  == packed_date_data[i].expect.year);
    fail_unless(date.month == packed_date_data[i].expect.month);
    fail_unless(date.day   == packed_date_data[i].expect.day);

  }
}
END_TEST;

/*!
 * \test Test reading of ISO formatted dates.
 */
START_TEST(test_iso_date_reader)
{
  cm_date_time_t dt;

  cm_read_iso_date_string(&dt, "2012-03-12T12:15:55.456");
  fail_unless(dt.date.year == 2012);
  fail_unless(dt.date.month == 3);
  fail_unless(dt.date.day == 12);
  fail_unless(dt.time.hh == 12);
  fail_unless(dt.time.mm == 15, "time.mm expected 15, got %d", (int)dt.time.mm);
  fail_unless(dt.time.s == 55.456);

  cm_read_iso_date_string(&dt, "2012-03-12T12:15:123.456Z");
  fail_unless(dt.date.year == 2012);
  fail_unless(dt.date.month == 3);
  fail_unless(dt.date.day == 12);
  fail_unless(dt.time.hh == 12);
  fail_unless(dt.time.mm == 17, "time.mm expected 17, got %d", (int)dt.time.mm);
  fail_unless(ALMOST_EQUAL(dt.time.s, 3.456, 0.000005), "time.s expected 3.456, got %f", dt.time.s);

  cm_read_iso_date_string(&dt, "2012-03-12T12:15:55.456+1");
  fail_unless(dt.date.year == 2012);
  fail_unless(dt.date.month == 3);
  fail_unless(dt.date.day == 12);
  fail_unless(dt.time.hh == 11);
  fail_unless(dt.time.mm == 15);
  fail_unless(dt.time.s == 55.456);

}
END_TEST;

typedef struct {
  double tcb;
  double tdb;
} tcb_tdb_test_pair_t;

// Values generated using IAU reference code
tcb_tdb_test_pair_t tcb_tdb_test_data[] = {
  {10000.000000,    10000.03772638570444541983},
  {2012345.500000,  2012345.50667962292209267616},
  {2456237.306285,  2456237.30608169268816709518},
  {2956237.200000,  2956237.19204439548775553703},
};

/*!
 * \test Test conversion from TCB time to TDB time.
 */
START_TEST(test_tcb_to_tdb)
{
  // We assert an accuracy here of around 0.04 s
  for (int i = 0 ; i < sizeof(tcb_tdb_test_data)/sizeof(tcb_tdb_test_data[0]); i++) {
    double tdb = cm_tcb_to_tdb(tcb_tdb_test_data[i].tcb);
    fail_unless(ALMOST_EQUAL(tdb, tcb_tdb_test_data[i].tdb, 0.0000005));
  }
}
END_TEST;

/*!
 * \test Test conversion from TDB time to TCB time.
 */
START_TEST(test_tdb_to_tcb)
{
  for (int i = 0 ; i < sizeof(tcb_tdb_test_data)/sizeof(tcb_tdb_test_data[0]); i++) {
    double tcb = cm_tdb_to_tcb(tcb_tdb_test_data[i].tdb);
    fail_unless(ALMOST_EQUAL(tcb, tcb_tdb_test_data[i].tcb, 0.0000005));
  }
}
END_TEST;

/*!
 * \test Test conversion from gregorian dates to Julian Days.
 */
START_TEST(test_greg_to_jd)
{
  cm_date_time_t dt = {{2000,1,1}, {12,0,0.0}};
  double res = cm_date_time_to_jd(&dt);

  fail_unless(res == 2451545.0);
}
END_TEST;

/*!
 * \test Test conversion from Julian Days to Gregorian date-time.
 */
START_TEST(test_jd_to_greg)
{
  cm_date_time_t dt;
  cm_jd_to_date_time(2451545.0, &dt);

  fail_unless(dt.date.year == 2000);
  fail_unless(dt.date.month == 1);
  fail_unless(dt.date.day == 1);
  fail_unless(dt.time.hh == 12);
  fail_unless(dt.time.mm == 0);
  fail_unless(dt.time.s == 0.0);
}
END_TEST;

/*!
 * \test Test cm_get_orbital_model.
 */
START_TEST(test_get_orbital_model)
{
  cm_orbital_model_t *model = NULL;
  model = cm_get_orbital_model("vsop87");
  fail_if(strcmp(model->name, "vsop87"),
          "could not get vsop87 model");

}
END_TEST;

/*!
 * \test Test Goffin 2000 pluto simultation.
 */
START_TEST(test_goffin2000_pos_at_jde)
{
  double3 res = cm_goffin2000(2448908.5);

  fail_unless(ALMOST_EQUAL(rad2deg(res.x), 232.74009, 0.000005),
              "computation of l should be %f but was %f (deg)",
              232.74009, rad2deg(res.x));
  fail_unless(ALMOST_EQUAL(rad2deg(res.y), 14.58769, 0.000005),
              "computation of b should be %f but was %f (deg)",
              14.58769, rad2deg(res.y));
  fail_unless(ALMOST_EQUAL(res.z, 29.711383, 0.000005),
              "computation of r should be %f but was %f (au)",
              29.711383, res.z);
}
END_TEST;

/*!
 * \test Test ELP 2000 moon simultation.
 */
START_TEST(test_elp2000)
{
  double3 res = cm_elp2000_82b(2448724.5);
  fail_unless(ALMOST_EQUAL(rad2deg(res.x), 133.162659, 0.000001),
              "computation of longitude (lambda) should be %f but is %f",
              133.162659, rad2deg(res.x));
  fail_unless(ALMOST_EQUAL(rad2deg(res.y), -3.229127, 0.000001),
              "computation of latitude (beta) should be %f but is %f",
               -3.229127, rad2deg(res.y));
  fail_unless(ALMOST_EQUAL(res.z*1.e-3, 368409.7, 0.05), // to m
              "computation of distance (delta) should be %f but is %f",
              368409.7, res.z*1.e-3);
}
END_TEST;

/*!
 * \test Test conversion of equatorial coordinates between epochs.
 */
START_TEST(test_cm_equ_epoch_conv)
{
  double2 res = cm_equ_epoch_conv(deg2rad(41.054063), deg2rad(49.227750),
                                  CM_J2000_0, 2462088.69);
  // ra: 41.547214; decl: 49.348483
  fail_unless(ALMOST_EQUAL(rad2deg(res.x), 41.547214, 0.0000005),
              "convert equ epoch new ra not valid");
  fail_unless(ALMOST_EQUAL(rad2deg(res.y), 49.348483, 0.0000005),
              "convert equ epoch new decl not valid");
}
END_TEST;

/*!
 * \test Test conversion from equatorial coordinates to ecliptic coordinates.
 */
START_TEST(test_cm_equ_to_ecl)
{
  double2 res = cm_equ_to_ecl(deg2rad(116.328942), deg2rad(28.026183),
                              deg2rad(CM_J2000_OBL_DEG));
  fail_unless(ALMOST_EQUAL(rad2deg(res.x), 113.215630, 0.0000005),
              "convert equ to ecl new lon not valid");
  fail_unless(ALMOST_EQUAL(rad2deg(res.y), 6.684170, 0.0000005),
              "convert equ to ecl new lat not valid");
}
END_TEST;


/*!
 * \test Test conversion from ecliptic coordinates to equatorial coordinates.
 */
START_TEST(test_cm_ecl_to_equ)
{
  double2 res = cm_ecl_to_equ(deg2rad(113.215630), deg2rad(6.684170),
                              deg2rad(CM_J2000_OBL_DEG));

  fail_unless(ALMOST_EQUAL(rad2deg(res.x), 116.328942, 0.000001),
              "convert ecl to equ new ra not valid result = %f != %f",
              rad2deg(res.x), 116.328942);
  fail_unless(ALMOST_EQUAL(rad2deg(res.y), 28.026183, 0.000001),
              "convert ecl to equ new decl not valid");
}
END_TEST;



// VSOP87 E Test
#include "vsop87e-test.h"

/*!
 * \test Test vsop87e implementation.
 */
START_TEST(test_vsop87e)
{
  for (int i = 0 ; i < sizeof(vsop87e_test_data)/sizeof(vsop87e_test_t) ; i++) {

    cm_state_vectors_t sv = cm_vsop87(vsop87e_test_data[i].body,
                                      vsop87e_test_data[i].jd);

    fail_unless(ALMOST_EQUAL(sv.p.x, vsop87e_test_data[i].x, 0.0000000005),
                "x position for vsop87e solution, invalid for body %d",
                (int)vsop87e_test_data[i].body);
    fail_unless(ALMOST_EQUAL(sv.p.y, vsop87e_test_data[i].y, 0.0000000005),
                "y position for vsop87e solution, invalid for body %d",
                (int)vsop87e_test_data[i].body);
    fail_unless(ALMOST_EQUAL(sv.p.z, vsop87e_test_data[i].z, 0.0000000005),
                "z position for vsop87e solution, invalid for body %d",
                (int)vsop87e_test_data[i].body);

    // TODO: What can we do to improve precision?
    fail_unless(ALMOST_EQUAL(sv.v.x, vsop87e_test_data[i].vx, 0.0000005),
                "x velocity for vsop87e solution, invalid for body %d "
                "expected: %.10f, got: %.10f",
                (int)vsop87e_test_data[i].body,
                vsop87e_test_data[i].vx, sv.v.x);
    fail_unless(ALMOST_EQUAL(sv.v.y, vsop87e_test_data[i].vy, 0.0000005),
                "y velocity for vsop87e solution, invalid for body %d "
                "expected: %.10f, got: %.10f",
                (int)vsop87e_test_data[i].body,
                vsop87e_test_data[i].vy, sv.v.y);
    fail_unless(ALMOST_EQUAL(sv.v.z, vsop87e_test_data[i].vz, 0.0000005),
                "z velocity for vsop87e solution, invalid for body %d",
                (int)vsop87e_test_data[i].body);
  }
}
END_TEST

START_TEST(test_cm_mean_elements)
{
  cm_kepler_elements_t elements;
  cm_compute_mean_orbital_elements_j2000(&elements, CM_EARTH, CM_J2000_0);

  fail_unless(elements.epoch == CM_J2000_0, "epoch missmatched");
  fail_unless(REL_EQUAL(elements.ecc, 0.01670862, 0.00001), "eccentricity wrong");
  fail_unless(REL_EQUAL(elements.semi_major, 1.0, 0.00001), "semimajor axis wrong");
  fail_unless(REL_EQUAL(elements.long_asc, 174.873174 * VMATH_RAD_PER_DEG, 0.00001),
              "lonitude of ascending node wrong");
  fail_unless(REL_EQUAL(elements.incl, 0, 0.00001),
              "inclination wrong (%f != %f)", elements.incl, 0.0);

  // TOOD: Test rest of the elements
}
END_TEST

START_TEST(test_cm_state_vector_to_orbital_elements)
{
  // This test works by using the cm_compute_mean_orbital_elements_j2000
  // function to compute the expected value for the epoch J2000 for the orbital
  // elements of Mercury. Then it computes the rectangular coordinates
  // (adjusting for not being heliocentric) using vsop87.
  // The translated state vector is then fed into
  // cm_state_vector_to_orbital_elements which should return roughly the same
  // value as the cm_compute_mean_orbital_elements_j2000 does.
  cm_kepler_elements_t oe;

  cm_kepler_elements_t oe_expect;
  // Reference body, we use mercury as it has a large eccentricity.
  cm_compute_mean_orbital_elements_j2000(&oe_expect, CM_MERCURY, CM_J2000_0);
  //oe_expect.semi_major *= CM_AU_IN_M;

  cm_state_vectors_t svsun = cm_vsop87(CM_SUN, CM_J2000_0);
  cm_state_vectors_t sve = cm_vsop87(CM_MERCURY, CM_J2000_0);
  sve.p = sve.p - svsun.p;
  sve.v = sve.v - svsun.v;

  cm_state_vector_to_orbital_elements(&oe, &sve,
                                      (CM_SUN_MASS+CM_MERCURY_MASS) * CM_G__AU_KG_D);

  fail_unless(REL_EQUAL(oe.semi_major, oe_expect.semi_major, 0.001),
              "a: got: %f, expected: %f", oe.semi_major, oe_expect.semi_major);
  fail_unless(REL_EQUAL(oe.ecc, oe_expect.ecc, 0.001),
              "e: got: %f, expected: %f", oe.ecc, oe_expect.ecc);
  fail_unless(REL_EQUAL(oe.incl, oe_expect.incl, 0.001),
              "i: got: %f, expected: %f", oe.incl, oe_expect.incl);
  fail_unless(REL_EQUAL(oe.long_asc, oe_expect.long_asc, 0.001),
              "long_asc: got: %f, expected: %f", oe.long_asc, oe_expect.long_asc);
  fail_unless(REL_EQUAL(oe.arg_peri, oe_expect.arg_peri, 0.001),
              "w: got: %f, expected: %f", oe.arg_peri, oe_expect.arg_peri);
  fail_unless(REL_EQUAL(oe.mean_motion, oe_expect.mean_motion, 0.001),
              "n: got: %f, expected: %f", oe.mean_motion, oe_expect.mean_motion);
  fail_unless(REL_EQUAL(oe.mean_anomaly_at_epoch,
                           oe_expect.mean_anomaly_at_epoch, 0.001),
              "M: got: %f, expected: %f", oe.mean_anomaly_at_epoch,
              oe_expect.mean_anomaly_at_epoch);
  fail_unless(REL_EQUAL(oe.epoch, oe_expect.epoch, 0.001),
              "epoch: got: %f, expected: %f", oe.epoch, oe_expect.epoch);

}
END_TEST

int main (int argc, char const *argv[])
{
  Suite *s = suite_create("orbital-sim");

  // Set up initial state, these functions have no unit test
  cm_init();

  TCase *modelquery = tcase_create("model-querying");
  tcase_add_test(modelquery, test_get_orbital_model);
  suite_add_tcase(s, modelquery);

  TCase *meanorbits = tcase_create("mean-orbits");
  tcase_add_test(meanorbits, test_cm_mean_elements);
  suite_add_tcase(s, meanorbits);

  TCase *celcoord = tcase_create("celestial-coordinates");
  tcase_add_test(celcoord, test_cm_equ_epoch_conv);
  tcase_add_test(celcoord, test_cm_equ_to_ecl);
  tcase_add_test(celcoord, test_cm_ecl_to_equ);
  tcase_add_test(celcoord, test_cm_state_vector_to_orbital_elements);
  suite_add_tcase(s, celcoord);

  TCase *elp = tcase_create("elp2000-82b");
  tcase_add_test(elp, test_elp2000);
  suite_add_tcase(s, elp);

  TCase *pluto = tcase_create("pluto");
  tcase_add_test(pluto, test_goffin2000_pos_at_jde);
  suite_add_tcase(s, pluto);

  TCase *vsop87 = tcase_create("vsop87");
  tcase_add_test(vsop87, test_vsop87e);
  suite_add_tcase(s, vsop87);

  TCase *date_tests = tcase_create("dates");
  tcase_add_test(date_tests, test_packed_dates);
  tcase_add_test(date_tests, test_greg_to_jd);
  tcase_add_test(date_tests, test_jd_to_greg);
  tcase_add_test(date_tests, test_iso_date_reader);
  tcase_add_test(date_tests, test_tcb_to_tdb);
  tcase_add_test(date_tests, test_tdb_to_tcb);
  suite_add_tcase(s, date_tests);


  SRunner *sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  int failed = srunner_ntests_failed(sr);
  srunner_free(sr);

  return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}
