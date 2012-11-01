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
#include <check.h>
#include <celmek/celmek.h>


#define ALMOST_EQUAL(a, b, p) (a >= b - p && a <= b + p)

START_TEST(test_get_orbital_model)
{
  cm_orbital_model_t *model = NULL;
  model = cm_get_orbital_model("vsop87");
  fail_if(strcmp(model->name, "vsop87"),
          "could not get vsop87 model");
  
}
END_TEST;

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

START_TEST(test_cm_equ_to_ecl)
{
  double2 res = cm_equ_to_ecl(deg2rad(116.328942), deg2rad(28.026183),
                              deg2rad(CM_J2000_OBL));
  fail_unless(ALMOST_EQUAL(rad2deg(res.x), 113.215630, 0.0000005),
              "convert equ to ecl new lon not valid");
  fail_unless(ALMOST_EQUAL(rad2deg(res.y), 6.684170, 0.0000005),
              "convert equ to ecl new lat not valid");
}
END_TEST;


START_TEST(test_cm_ecl_to_equ)
{
  double2 res = cm_ecl_to_equ(deg2rad(113.215630), deg2rad(6.684170),
                              deg2rad(CM_J2000_OBL));

  fail_unless(ALMOST_EQUAL(rad2deg(res.x), 116.328942, 0.000001),
              "convert ecl to equ new ra not valid result = %f != %f",
              rad2deg(res.x), 116.328942);
  fail_unless(ALMOST_EQUAL(rad2deg(res.y), 28.026183, 0.000001),
              "convert ecl to equ new decl not valid");
}
END_TEST;



// VSOP87 E Test
#include "vsop87e-test.h"

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

int main (int argc, char const *argv[])
{ 
  Suite *s = suite_create("orbital-sim");

  // Set up initial state, these functions have no unit test
  cm_init();

  TCase *modelquery = tcase_create("model-querying");
  tcase_add_test(modelquery, test_get_orbital_model);
  suite_add_tcase(s, modelquery);
  
  
  TCase *celcoord = tcase_create("celestial-coordinates");
  tcase_add_test(celcoord, test_cm_equ_epoch_conv);
  tcase_add_test(celcoord, test_cm_equ_to_ecl);
  tcase_add_test(celcoord, test_cm_ecl_to_equ);
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


  SRunner *sr = srunner_create(s);
  srunner_run_all(sr, CK_NORMAL);
  int failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  
  return (failed == 0) ? EXIT_SUCCESS : EXIT_FAILURE;
}