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

#include <stdlib.h>
#include <stdio.h>
#include <check.h>
#include <celmek/celmek.h>

#define ALEN(a) sizeof(a)/sizeof(a[0])

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

    ck_assert_int_eq(date.year, packed_date_data[i].expect.year);
    ck_assert_int_eq(date.month, packed_date_data[i].expect.month);
    ck_assert_int_eq(date.day, packed_date_data[i].expect.day);

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
  ck_assert_int_eq(dt.date.year, 2012);
  ck_assert_int_eq(dt.date.month, 3);
  ck_assert_int_eq(dt.date.day, 12);
  ck_assert_int_eq(dt.time.hh, 12);
  ck_assert_int_eq(dt.time.mm, 15);
  ck_assert_int_eq(dt.time.s, 55.456);

  cm_read_iso_date_string(&dt, "2012-03-12T12:15:123.456Z");
  ck_assert_int_eq(dt.date.year, 2012);
  ck_assert_int_eq(dt.date.month, 3);
  ck_assert_int_eq(dt.date.day, 12);
  ck_assert_int_eq(dt.time.hh, 12);
  ck_assert_int_eq(dt.time.mm, 17);
  fail_unless(ALMOST_EQUAL(dt.time.s, 3.456, 0.000005), "time.s expected 3.456, got %f", dt.time.s);

  cm_read_iso_date_string(&dt, "2012-03-12T12:15:55.456+1");
  ck_assert_int_eq(dt.date.year, 2012);
  ck_assert_int_eq(dt.date.month, 3);
  ck_assert_int_eq(dt.date.day, 12);
  ck_assert_int_eq(dt.time.hh, 11);
  ck_assert_int_eq(dt.time.mm, 15);
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

  ck_assert_int_eq(dt.date.year, 2000);
  ck_assert_int_eq(dt.date.month, 1);
  ck_assert_int_eq(dt.date.day, 1);
  ck_assert_int_eq(dt.time.hh, 12);
  ck_assert_int_eq(dt.time.mm, 0);
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
  ck_assert_str_eq(model->name, "vsop87");
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

  ck_assert_int_eq(elements.epoch, CM_J2000_0);
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



typedef struct {
  cm_date_time_t date_time;
  double x, y, z;
  double xp, yp, zp;
} gusp86_test_data_t;

gusp86_test_data_t miranda_test[] = {
  {{{1995, 07, 10}, {14,  0,  0}},  123566.483,  -19000.705,  -34625.500, -1.30507976,  2.51966201, -6.05500475},
  {{{1995, 07, 10}, {15,  0,  0}},  116776.465,   -9655.903,  -55704.358, -2.45651219,  2.65694226, -5.62224338},
  {{{1995, 07, 10}, {16,  0,  0}},  105982.316,      19.985,  -74873.280, -3.52324853,  2.70307511, -4.99693058},
  {{{1995, 07, 10}, {17,  0,  0}},   91555.784,    9695.167,  -91476.172, -4.46873301,  2.65656077, -4.20071817},
  {{{1995, 07, 10}, {18,  0,  0}},   73992.936,   19038.200, -104945.647, -5.26065948,  2.51909369, -3.26110062},
  {{{1995, 07, 10}, {19,  0,  0}},   53896.826,   27729.395, -114822.372, -5.87207110,  2.29549015, -2.21043851},
  {{{1995, 07, 10}, {20,  0,  0}},   31956.557,   35471.760, -120770.577, -6.28226364,  1.99350774, -1.08482116},
  {{{1995, 07, 10}, {21,  0,  0}},    8923.476,   42001.117, -122589.263, -6.47746413,  1.62356496,   .07718834},
  {{{1995, 07, 10}, {22,  0,  0}},  -14414.641,   47095.049, -120218.736, -6.45126600,  1.19837281,  1.23588019},
  {{{1995, 07, 10}, {23,  0,  0}},  -37260.517,   50580.373, -113742.309, -6.20481076,   .73249243,  2.35175048},
};

gusp86_test_data_t ariel_test[] = {
  {{{1995, 07, 10}, {14,  0,  0}},  184904.768,  -47229.606,   12098.220,   .68335318,  1.30382491, -5.29988104},
  {{{1995, 07, 10}, {15,  0,  0}},  186368.403,  -42290.739,   -7012.500,   .12909352,  1.43746647, -5.30747363},
  {{{1995, 07, 10}, {16,  0,  0}},  185832.508,  -36898.147,  -26047.954,  -.42647184,  1.55566775, -5.25812639},
  {{{1995, 07, 10}, {17,  0,  0}},  183303.072,  -31109.736,  -44803.937,  -.97741186,  1.65717056, -5.15237949},
  {{{1995, 07, 10}, {18,  0,  0}},  178807.365,  -24987.628,  -63079.270, -1.51784661,  1.74089367, -4.99136895},
  {{{1995, 07, 10}, {19,  0,  0}},  172393.643,  -18597.499,  -80677.916, -2.04200805,  1.80594389, -4.77681462},
  {{{1995, 07, 10}, {20,  0,  0}},  164130.639,  -12007.891,  -97411.061, -2.54429980,  1.85162530, -4.51100224},
  {{{1995, 07, 10}, {21,  0,  0}},  154106.838,   -5289.483, -113099.105, -3.01935514,  1.87744654, -4.19676001},
  {{{1995, 07, 10}, {22,  0,  0}},  142429.547,    1485.656, -127573.569, -3.46209278,  1.88312604, -3.83742966},
  {{{1995, 07, 10}, {23,  0,  0}},  129223.772,    8244.825, -140678.879, -3.86776999,  1.86859520, -3.43683237},
};

gusp86_test_data_t umbriel_test[] = {
  {{{1995, 07, 10}, {14,  0,  0}}, -239231.405,   79673.496,  -81304.484, -1.72299096,  -.78325085,  4.28651774},
  {{{1995, 07, 10}, {15,  0,  0}}, -244947.486,   76695.011,  -65719.487, -1.45138070,  -.87096610,  4.36898332},
  {{{1995, 07, 10}, {16,  0,  0}}, -249675.017,   73407.039,  -49869.392, -1.17398137,  -.95514499,  4.43379855},
  {{{1995, 07, 10}, {17,  0,  0}}, -253395.200,   69822.932,  -33818.222,  -.89192819, -1.03544596,  4.48070880},
  {{{1995, 07, 10}, {18,  0,  0}}, -256093.358,   65957.242,  -17630.780,  -.60637438, -1.11154409,  4.50953445},
  {{{1995, 07, 10}, {19,  0,  0}}, -257758.991,   61825.657,   -1372.376,  -.31848603, -1.18313242,  4.52017142},
  {{{1995, 07, 10}, {20,  0,  0}}, -258385.813,   57444.937,   14891.444,  -.02943706, -1.24992325,  4.51259141},
  {{{1995, 07, 10}, {21,  0,  0}}, -257971.768,   52832.839,   31095.170,   .25959593, -1.31164926,  4.48684175},
  {{{1995, 07, 10}, {22,  0,  0}}, -256519.031,   48008.041,   47173.604,   .54743866, -1.36806466,  4.44304488},
  {{{1995, 07, 10}, {23,  0,  0}}, -254033.994,   42990.068,   63062.120,   .83292424, -1.41894609,  4.38139755},
};

gusp86_test_data_t titania_test[] = {
  {{{1995, 07, 10}, {14,  0,  0}}, -280128.779,  149463.486, -299251.829, -2.67820934,  -.04172649,  2.47087602},
  {{{1995, 07, 10}, {15,  0,  0}}, -289642.933,  149245.644, -290221.835, -2.60666546,  -.07923656,  2.54490241},
  {{{1995, 07, 10}, {16,  0,  0}}, -298895.308,  148892.894, -280929.464, -2.53278376,  -.11666555,  2.61661013},
  {{{1995, 07, 10}, {17,  0,  0}}, -307877.610,  148405.587, -271383.178, -2.45663211,  -.15397973,  2.68593555},
  {{{1995, 07, 10}, {18,  0,  0}}, -316581.792,  147784.198, -261591.664, -2.37828032,  -.19114547,  2.75281729},
  {{{1995, 07, 10}, {19,  0,  0}}, -325000.060,  147029.321, -251563.827, -2.29780016,  -.22812932,  2.81719618},
  {{{1995, 07, 10}, {20,  0,  0}}, -333124.885,  146141.670, -241308.778, -2.21526520,  -.26489801,  2.87901540},
  {{{1995, 07, 10}, {21,  0,  0}}, -340949.002,  145122.080, -230835.833, -2.13075085,  -.30141851,  2.93822044},
  {{{1995, 07, 10}, {22,  0,  0}}, -348465.423,  143971.502, -220154.495, -2.04433417,  -.33765804,  2.99475921},
  {{{1995, 07, 10}, {23,  0,  0}}, -355667.440,  142691.006, -209274.456, -1.95609389,  -.37358408,  3.04858207},
};

gusp86_test_data_t oberon_test[] = {
  {{{1995, 07, 10}, {14,  0,  0}},  551394.040, -158776.220,  104088.772,   .75311907,   .64540802, -2.99279482},
  {{{1995, 07, 10}, {15,  0,  0}},  554001.114, -156422.593,   93294.441,   .69497936,   .66198498, -3.00317166},
  {{{1995, 07, 10}, {16,  0,  0}},  556398.400, -154009.751,   82464.851,   .63657924,   .67831031, -3.01241017},
  {{{1995, 07, 10}, {17,  0,  0}},  558585.001, -151538.611,   71604.106,   .57794098,   .69437783, -3.02050699},
  {{{1995, 07, 10}, {18,  0,  0}},  560560.102, -149010.114,   60716.321,   .51908692,   .71018145, -3.02745918},
  {{{1995, 07, 10}, {19,  0,  0}},  562322.966, -146425.219,   49805.621,   .46003946,   .72571520, -3.03326425},
  {{{1995, 07, 10}, {20,  0,  0}},  563872.939, -143784.908,   38876.137,   .40082107,   .74097323, -3.03792015},
  {{{1995, 07, 10}, {21,  0,  0}},  565209.447, -141090.185,   27932.008,   .34145430,   .75594976, -3.04142530},
  {{{1995, 07, 10}, {22,  0,  0}},  566331.998, -138342.073,   16977.378,   .28196171,   .77063915, -3.04377853},
  {{{1995, 07, 10}, {23,  0,  0}},  567240.179, -135541.615,    6016.392,   .22236592,   .78503587, -3.04497914},
};

START_TEST(test_gust86)
  gust86_res_t res;
  for (int i = 0 ; i < ALEN(miranda_test) ; i ++) {
    double t = cm_date_time_to_jd(&miranda_test[i].date_time);
    cm_gust86(&res, &gust86_miranda, t);
  }

  for (int i = 0 ; i < ALEN(ariel_test) ; i ++) {
    double t = cm_date_time_to_jd(&ariel_test[i].date_time);
    cm_gust86(&res, &gust86_ariel, t);
  }

  for (int i = 0 ; i < ALEN(umbriel_test) ; i ++) {
    double t = cm_date_time_to_jd(&umbriel_test[i].date_time);
    cm_gust86(&res, &gust86_umbriel, t);
  }

  for (int i = 0 ; i < ALEN(titania_test) ; i ++) {
    double t = cm_date_time_to_jd(&titania_test[i].date_time);
    cm_gust86(&res, &gust86_titania, t);
  }

  for (int i = 0 ; i < ALEN(oberon_test) ; i ++) {
    double t = cm_date_time_to_jd(&oberon_test[i].date_time);
    cm_gust86(&res, &gust86_oberon, t);
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

  TCase *gust86 = tcase_create("gust86");
  tcase_add_test(gust86, test_gust86);
  suite_add_tcase(s, gust86);

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
