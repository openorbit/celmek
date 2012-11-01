typedef enum {
  IAU_TERM_E1,
  IAU_TERM_E10,
  IAU_TERM_E11,
  IAU_TERM_E12,
  IAU_TERM_E13,
  IAU_TERM_E2,
  IAU_TERM_E3,
  IAU_TERM_E4,
  IAU_TERM_E5,
  IAU_TERM_E6,
  IAU_TERM_E7,
  IAU_TERM_E8,
  IAU_TERM_E9,
  IAU_TERM_J1,
  IAU_TERM_J2,
  IAU_TERM_J3,
  IAU_TERM_J4,
  IAU_TERM_J5,
  IAU_TERM_J6,
  IAU_TERM_J7,
  IAU_TERM_J8,
  IAU_TERM_JD,
  IAU_TERM_JA,
  IAU_TERM_JB,
  IAU_TERM_JC,
  IAU_TERM_JE,
  IAU_TERM_M1,
  IAU_TERM_M2,
  IAU_TERM_M3,
  IAU_TERM_ME1,
  IAU_TERM_ME2,
  IAU_TERM_ME3,
  IAU_TERM_ME4,
  IAU_TERM_ME5,
  IAU_TERM_N,
  IAU_TERM_N1,
  IAU_TERM_N2,
  IAU_TERM_N3,
  IAU_TERM_N4,
  IAU_TERM_N5,
  IAU_TERM_N6,
  IAU_TERM_N7,
  IAU_TERM_S1,
  IAU_TERM_S2,
  IAU_TERM_S3,
  IAU_TERM_S4,
  IAU_TERM_S5,
  IAU_TERM_S6,
  IAU_TERM_U1,
  IAU_TERM_U10,
  IAU_TERM_U11,
  IAU_TERM_U12,
  IAU_TERM_U13,
  IAU_TERM_U14,
  IAU_TERM_U15,
  IAU_TERM_U16,
  IAU_TERM_U2,
  IAU_TERM_U3,
  IAU_TERM_U4,
  IAU_TERM_U5,
  IAU_TERM_U6,
  IAU_TERM_U7,
  IAU_TERM_U8,
  IAU_TERM_U9,
  IAU_TERM_COUNT
} iau_term_id_t;

typedef struct {
  iau_term_id_t term;
  double a, b;
} iau_term_t;

iau_term_t d_terms[] = {
  {IAU_TERM_E11, 119.743, +0.0036096},
  {IAU_TERM_E10, 15.134, -0.1589763},
  {IAU_TERM_E13, 25.053, +12.9590088},
  {IAU_TERM_E12, 239.961, +0.1643573},
  {IAU_TERM_M1, 169.51, -0.4357640},
  {IAU_TERM_M3, 53.47, -0.0181510},
  {IAU_TERM_M2, 192.93, +1128.4096700},
  {IAU_TERM_E9, 34.226, +1.7484877},
  {IAU_TERM_E8, 276.617, +0.3287146},
  {IAU_TERM_E5, 357.529, +0.9856003},
  {IAU_TERM_E4, 176.625, +13.3407154},
  {IAU_TERM_E7, 134.963, +13.0649930},
  {IAU_TERM_E6, 311.589, +26.4057084},
  {IAU_TERM_E1, 125.045, -0.0529921},
  {IAU_TERM_E3, 260.008, +13.0120009},
  {IAU_TERM_E2, 250.089, -0.1059842},
  {IAU_TERM_ME5, 153.955429, +20.461675},
  {IAU_TERM_ME4, 339.164343, +16.369340},
  {IAU_TERM_ME1, 174.791086, +4.092335},
  {IAU_TERM_ME3, 164.373257, +12.277005},
  {IAU_TERM_ME2, 349.582171, +8.184670},
};

iau_term_t T_terms[] = {
  {IAU_TERM_U9, 101.81, +12872.63},
  {IAU_TERM_U8, 157.36, +16652.76},
  {IAU_TERM_U1, 115.75, +54991.87},
  {IAU_TERM_S6, 345.20, -1016.3},
  {IAU_TERM_S5, 316.45, +506.2},
  {IAU_TERM_S4, 300.00, -7225.9},
  {IAU_TERM_U11, 102.23, -2024.22},
  {IAU_TERM_JD, 114.012305, +6070.2476},
  {IAU_TERM_U10, 138.64, +8061.81},
  {IAU_TERM_U13, 304.01, -51.94},
  {IAU_TERM_U12, 316.41, +2863.96},
  {IAU_TERM_U15, 340.82, -75.32},
  {IAU_TERM_U14, 308.71, -93.17},
  {IAU_TERM_U16, 259.14, -504.81},
  {IAU_TERM_U4, 61.77, +25733.59},
  {IAU_TERM_U5, 249.32, +24471.46},
  {IAU_TERM_N3, 354.27, +46564.5},
  {IAU_TERM_U7, 77.66, +20289.42},
  {IAU_TERM_S1, 353.32, +75706.7},
  {IAU_TERM_S3, 177.40, -36505.5},
  {IAU_TERM_S2, 28.72, +75706.7},
  {IAU_TERM_JA, 99.360714, +4850.4046},
  {IAU_TERM_U6, 43.86, +22278.41},
  {IAU_TERM_N, 357.85, +52.316},
  {IAU_TERM_U3, 135.03, +29927.35},
  {IAU_TERM_JE, 49.511251, +64.3000},
  {IAU_TERM_N1, 323.92, +62606.6},
  {IAU_TERM_N2, 220.51, +55064.2},
  {IAU_TERM_U2, 141.69, +41887.66},
  {IAU_TERM_N4, 75.31, +26109.4},
  {IAU_TERM_N5, 35.36, +14325.4},
  {IAU_TERM_N6, 142.61, +2824.6},
  {IAU_TERM_N7, 177.85, +52.316},
  {IAU_TERM_J8, 113.35, +6070.0},
  {IAU_TERM_J4, 355.80, +1191.3},
  {IAU_TERM_J5, 119.90, +262.1},
  {IAU_TERM_J6, 229.80, +64.3},
  {IAU_TERM_J7, 352.25, +2382.6},
  {IAU_TERM_J1, 73.32, +91472.9},
  {IAU_TERM_J2, 24.62, +45137.2},
  {IAU_TERM_J3, 283.90, +4850.7},
  {IAU_TERM_JB, 175.895369, +1191.9605},
  {IAU_TERM_JC, 300.323162, +262.5475},
};

typedef struct {
  iau_term_id_t term;
  double a;
} iau_term2_t;

iau_term2_t T2_terms[] = {
  {IAU_TERM_M2, +8.864},
};

typedef enum {
  IAU_BODY_19P_BORRELLY,
  IAU_BODY_9P_TEMPEL_1,
  IAU_BODY_ADRASTEA,
  IAU_BODY_AMALTHEA,
  IAU_BODY_ARIEL,
  IAU_BODY_ATLAS,
  IAU_BODY_BELINDA,
  IAU_BODY_BIANCA,
  IAU_BODY_CALLISTO,
  IAU_BODY_CALYPSO,
  IAU_BODY_CERES,
  IAU_BODY_CHARON,
  IAU_BODY_CORDELIA,
  IAU_BODY_CRESSIDA,
  IAU_BODY_DAVIDA,
  IAU_BODY_DEIMOS,
  IAU_BODY_DESDEMONA,
  IAU_BODY_DESPINA,
  IAU_BODY_DIONE,
  IAU_BODY_EARTH,
  IAU_BODY_ENCELADUS,
  IAU_BODY_EPIMETHEUS,
  IAU_BODY_EROS,
  IAU_BODY_EUROPA,
  IAU_BODY_GALATEA,
  IAU_BODY_GANYMEDE,
  IAU_BODY_GASPRA,
  IAU_BODY_HELENE,
  IAU_BODY_IAPETUS,
  IAU_BODY_IDA,
  IAU_BODY_IO,
  IAU_BODY_ITOKAWA,
  IAU_BODY_JANUS,
  IAU_BODY_JULIET,
  IAU_BODY_JUPITER,
  IAU_BODY_LARISSA,
  IAU_BODY_LUTETIA,
  IAU_BODY_MARS,
  IAU_BODY_MERCURY,
  IAU_BODY_METIS,
  IAU_BODY_MIMAS,
  IAU_BODY_MIRANDA,
  IAU_BODY_MOON,
  IAU_BODY_NAIAD,
  IAU_BODY_NEPTUNE,
  IAU_BODY_OBERON,
  IAU_BODY_OPHELIA,
  IAU_BODY_PALLAS,
  IAU_BODY_PAN,
  IAU_BODY_PANDORA,
  IAU_BODY_PHOBOS,
  IAU_BODY_PHOEBE,
  IAU_BODY_PLUTO,
  IAU_BODY_PORTIA,
  IAU_BODY_PROMETHEUS,
  IAU_BODY_PROTEUS,
  IAU_BODY_PUCK,
  IAU_BODY_RHEA,
  IAU_BODY_ROSALIND,
  IAU_BODY_SATURN,
  IAU_BODY_STEINS,
  IAU_BODY_SUN,
  IAU_BODY_TELESTO,
  IAU_BODY_TETHYS,
  IAU_BODY_THALASSA,
  IAU_BODY_THEBE,
  IAU_BODY_TITAN,
  IAU_BODY_TITANIA,
  IAU_BODY_TRITON,
  IAU_BODY_UMBRIEL,
  IAU_BODY_URANUS,
  IAU_BODY_VENUS,
  IAU_BODY_VESTA,
  IAU_BODY_COUNT
} iau_body_id_t;

typedef enum {
  IAU_SIN_U9,
  IAU_SIN_U8,
  IAU_SIN_8N7,
  IAU_SIN_E11,
  IAU_SIN_U4,
  IAU_SIN_U7,
  IAU_SIN_E12,
  IAU_SIN_U1,
  IAU_SIN_S6,
  IAU_SIN_S5,
  IAU_SIN_S4,
  IAU_SIN_JD,
  IAU_SIN_M1,
  IAU_SIN_JB,
  IAU_SIN_M3,
  IAU_SIN_M2,
  IAU_SIN_U11,
  IAU_SIN_U10,
  IAU_SIN_U13,
  IAU_SIN_U12,
  IAU_SIN_U15,
  IAU_SIN_2U11,
  IAU_SIN_2U12,
  IAU_SIN_U16,
  IAU_SIN_6N7,
  IAU_SIN_U5,
  IAU_SIN_2N7,
  IAU_SIN_2J2,
  IAU_SIN_2J1,
  IAU_SIN_N3,
  IAU_SIN_S1,
  IAU_SIN_U14,
  IAU_SIN_3N7,
  IAU_SIN_7N7,
  IAU_SIN_E9,
  IAU_SIN_E8,
  IAU_SIN_S2,
  IAU_SIN_E10,
  IAU_SIN_E5,
  IAU_SIN_E4,
  IAU_SIN_E7,
  IAU_SIN_E6,
  IAU_SIN_E1,
  IAU_SIN_E13,
  IAU_SIN_E3,
  IAU_SIN_E2,
  IAU_SIN_4N7,
  IAU_SIN_U6,
  IAU_SIN_5N7,
  IAU_SIN_J3,
  IAU_SIN_N,
  IAU_SIN_U3,
  IAU_SIN_JE,
  IAU_SIN_N1,
  IAU_SIN_N2,
  IAU_SIN_U2,
  IAU_SIN_N4,
  IAU_SIN_N5,
  IAU_SIN_N6,
  IAU_SIN_N7,
  IAU_SIN_J8,
  IAU_SIN_9N7,
  IAU_SIN_J4,
  IAU_SIN_J5,
  IAU_SIN_J6,
  IAU_SIN_J7,
  IAU_SIN_J1,
  IAU_SIN_J2,
  IAU_SIN_JA,
  IAU_SIN_ME5,
  IAU_SIN_ME4,
  IAU_SIN_2N1,
  IAU_SIN_ME1,
  IAU_SIN_2S2,
  IAU_SIN_ME3,
  IAU_SIN_ME2,
  IAU_SIN_2S1,
  IAU_SIN_JC,
  IAU_SIN_S3,
  IAU_SIN_COUNT
} iau_sine_id_t;

typedef enum {
  IAU_COS_U9,
  IAU_COS_U8,
  IAU_COS_2S1,
  IAU_COS_S3,
  IAU_COS_U4,
  IAU_COS_U7,
  IAU_COS_U6,
  IAU_COS_U1,
  IAU_COS_S6,
  IAU_COS_U3,
  IAU_COS_U2,
  IAU_COS_JD,
  IAU_COS_M1,
  IAU_COS_JB,
  IAU_COS_M3,
  IAU_COS_U11,
  IAU_COS_U10,
  IAU_COS_U13,
  IAU_COS_7N7,
  IAU_COS_U15,
  IAU_COS_2U11,
  IAU_COS_U16,
  IAU_COS_U5,
  IAU_COS_2N7,
  IAU_COS_2J2,
  IAU_COS_N3,
  IAU_COS_S1,
  IAU_COS_U14,
  IAU_COS_3N7,
  IAU_COS_E10,
  IAU_COS_E4,
  IAU_COS_E7,
  IAU_COS_E6,
  IAU_COS_E1,
  IAU_COS_E13,
  IAU_COS_E3,
  IAU_COS_E2,
  IAU_COS_4N7,
  IAU_COS_5N7,
  IAU_COS_N,
  IAU_COS_J3,
  IAU_COS_JE,
  IAU_COS_N1,
  IAU_COS_N2,
  IAU_COS_S4,
  IAU_COS_N4,
  IAU_COS_N5,
  IAU_COS_N6,
  IAU_COS_N7,
  IAU_COS_J8,
  IAU_COS_J4,
  IAU_COS_J5,
  IAU_COS_J6,
  IAU_COS_J7,
  IAU_COS_J1,
  IAU_COS_J2,
  IAU_COS_JA,
  IAU_COS_6N7,
  IAU_COS_2S2,
  IAU_COS_S2,
  IAU_COS_2N1,
  IAU_COS_JC,
  IAU_COS_COUNT
} iau_cosine_id_t;

typedef struct {
  double m;
  iau_term_id_t term;
} iau_arg_rule_t;

iau_arg_rule_t sine_rules[] = {
  [IAU_SIN_U9] = {1, IAU_TERM_U9},
  [IAU_SIN_U8] = {1, IAU_TERM_U8},
  [IAU_SIN_8N7] = {8, IAU_TERM_N7},
  [IAU_SIN_E11] = {1, IAU_TERM_E11},
  [IAU_SIN_U4] = {1, IAU_TERM_U4},
  [IAU_SIN_U7] = {1, IAU_TERM_U7},
  [IAU_SIN_E12] = {1, IAU_TERM_E12},
  [IAU_SIN_U1] = {1, IAU_TERM_U1},
  [IAU_SIN_S6] = {1, IAU_TERM_S6},
  [IAU_SIN_S5] = {1, IAU_TERM_S5},
  [IAU_SIN_S4] = {1, IAU_TERM_S4},
  [IAU_SIN_JD] = {1, IAU_TERM_JD},
  [IAU_SIN_M1] = {1, IAU_TERM_M1},
  [IAU_SIN_JB] = {1, IAU_TERM_JB},
  [IAU_SIN_M3] = {1, IAU_TERM_M3},
  [IAU_SIN_M2] = {1, IAU_TERM_M2},
  [IAU_SIN_U11] = {1, IAU_TERM_U11},
  [IAU_SIN_U10] = {1, IAU_TERM_U10},
  [IAU_SIN_U13] = {1, IAU_TERM_U13},
  [IAU_SIN_U12] = {1, IAU_TERM_U12},
  [IAU_SIN_U15] = {1, IAU_TERM_U15},
  [IAU_SIN_2U11] = {2, IAU_TERM_U11},
  [IAU_SIN_2U12] = {2, IAU_TERM_U12},
  [IAU_SIN_U16] = {1, IAU_TERM_U16},
  [IAU_SIN_6N7] = {6, IAU_TERM_N7},
  [IAU_SIN_U5] = {1, IAU_TERM_U5},
  [IAU_SIN_2N7] = {2, IAU_TERM_N7},
  [IAU_SIN_2J2] = {2, IAU_TERM_J2},
  [IAU_SIN_2J1] = {2, IAU_TERM_J1},
  [IAU_SIN_N3] = {1, IAU_TERM_N3},
  [IAU_SIN_S1] = {1, IAU_TERM_S1},
  [IAU_SIN_U14] = {1, IAU_TERM_U14},
  [IAU_SIN_3N7] = {3, IAU_TERM_N7},
  [IAU_SIN_7N7] = {7, IAU_TERM_N7},
  [IAU_SIN_E9] = {1, IAU_TERM_E9},
  [IAU_SIN_E8] = {1, IAU_TERM_E8},
  [IAU_SIN_S2] = {1, IAU_TERM_S2},
  [IAU_SIN_E10] = {1, IAU_TERM_E10},
  [IAU_SIN_E5] = {1, IAU_TERM_E5},
  [IAU_SIN_E4] = {1, IAU_TERM_E4},
  [IAU_SIN_E7] = {1, IAU_TERM_E7},
  [IAU_SIN_E6] = {1, IAU_TERM_E6},
  [IAU_SIN_E1] = {1, IAU_TERM_E1},
  [IAU_SIN_E13] = {1, IAU_TERM_E13},
  [IAU_SIN_E3] = {1, IAU_TERM_E3},
  [IAU_SIN_E2] = {1, IAU_TERM_E2},
  [IAU_SIN_4N7] = {4, IAU_TERM_N7},
  [IAU_SIN_U6] = {1, IAU_TERM_U6},
  [IAU_SIN_5N7] = {5, IAU_TERM_N7},
  [IAU_SIN_J3] = {1, IAU_TERM_J3},
  [IAU_SIN_N] = {1, IAU_TERM_N},
  [IAU_SIN_U3] = {1, IAU_TERM_U3},
  [IAU_SIN_JE] = {1, IAU_TERM_JE},
  [IAU_SIN_N1] = {1, IAU_TERM_N1},
  [IAU_SIN_N2] = {1, IAU_TERM_N2},
  [IAU_SIN_U2] = {1, IAU_TERM_U2},
  [IAU_SIN_N4] = {1, IAU_TERM_N4},
  [IAU_SIN_N5] = {1, IAU_TERM_N5},
  [IAU_SIN_N6] = {1, IAU_TERM_N6},
  [IAU_SIN_N7] = {1, IAU_TERM_N7},
  [IAU_SIN_J8] = {1, IAU_TERM_J8},
  [IAU_SIN_9N7] = {9, IAU_TERM_N7},
  [IAU_SIN_J4] = {1, IAU_TERM_J4},
  [IAU_SIN_J5] = {1, IAU_TERM_J5},
  [IAU_SIN_J6] = {1, IAU_TERM_J6},
  [IAU_SIN_J7] = {1, IAU_TERM_J7},
  [IAU_SIN_J1] = {1, IAU_TERM_J1},
  [IAU_SIN_J2] = {1, IAU_TERM_J2},
  [IAU_SIN_JA] = {1, IAU_TERM_JA},
  [IAU_SIN_ME5] = {1, IAU_TERM_ME5},
  [IAU_SIN_ME4] = {1, IAU_TERM_ME4},
  [IAU_SIN_2N1] = {2, IAU_TERM_N1},
  [IAU_SIN_ME1] = {1, IAU_TERM_ME1},
  [IAU_SIN_2S2] = {2, IAU_TERM_S2},
  [IAU_SIN_ME3] = {1, IAU_TERM_ME3},
  [IAU_SIN_ME2] = {1, IAU_TERM_ME2},
  [IAU_SIN_2S1] = {2, IAU_TERM_S1},
  [IAU_SIN_JC] = {1, IAU_TERM_JC},
  [IAU_SIN_S3] = {1, IAU_TERM_S3},
};

iau_arg_rule_t cosine_rules[] = {
  [IAU_COS_U9] = {1, IAU_TERM_U9},
  [IAU_COS_U8] = {1, IAU_TERM_U8},
  [IAU_COS_2S1] = {2, IAU_TERM_S1},
  [IAU_COS_S3] = {1, IAU_TERM_S3},
  [IAU_COS_U4] = {1, IAU_TERM_U4},
  [IAU_COS_U7] = {1, IAU_TERM_U7},
  [IAU_COS_U6] = {1, IAU_TERM_U6},
  [IAU_COS_U1] = {1, IAU_TERM_U1},
  [IAU_COS_S6] = {1, IAU_TERM_S6},
  [IAU_COS_U3] = {1, IAU_TERM_U3},
  [IAU_COS_U2] = {1, IAU_TERM_U2},
  [IAU_COS_JD] = {1, IAU_TERM_JD},
  [IAU_COS_M1] = {1, IAU_TERM_M1},
  [IAU_COS_JB] = {1, IAU_TERM_JB},
  [IAU_COS_M3] = {1, IAU_TERM_M3},
  [IAU_COS_U11] = {1, IAU_TERM_U11},
  [IAU_COS_U10] = {1, IAU_TERM_U10},
  [IAU_COS_U13] = {1, IAU_TERM_U13},
  [IAU_COS_7N7] = {7, IAU_TERM_N7},
  [IAU_COS_U15] = {1, IAU_TERM_U15},
  [IAU_COS_2U11] = {2, IAU_TERM_U11},
  [IAU_COS_U16] = {1, IAU_TERM_U16},
  [IAU_COS_U5] = {1, IAU_TERM_U5},
  [IAU_COS_2N7] = {2, IAU_TERM_N7},
  [IAU_COS_2J2] = {2, IAU_TERM_J2},
  [IAU_COS_N3] = {1, IAU_TERM_N3},
  [IAU_COS_S1] = {1, IAU_TERM_S1},
  [IAU_COS_U14] = {1, IAU_TERM_U14},
  [IAU_COS_3N7] = {3, IAU_TERM_N7},
  [IAU_COS_E10] = {1, IAU_TERM_E10},
  [IAU_COS_E4] = {1, IAU_TERM_E4},
  [IAU_COS_E7] = {1, IAU_TERM_E7},
  [IAU_COS_E6] = {1, IAU_TERM_E6},
  [IAU_COS_E1] = {1, IAU_TERM_E1},
  [IAU_COS_E13] = {1, IAU_TERM_E13},
  [IAU_COS_E3] = {1, IAU_TERM_E3},
  [IAU_COS_E2] = {1, IAU_TERM_E2},
  [IAU_COS_4N7] = {4, IAU_TERM_N7},
  [IAU_COS_5N7] = {5, IAU_TERM_N7},
  [IAU_COS_N] = {1, IAU_TERM_N},
  [IAU_COS_J3] = {1, IAU_TERM_J3},
  [IAU_COS_JE] = {1, IAU_TERM_JE},
  [IAU_COS_N1] = {1, IAU_TERM_N1},
  [IAU_COS_N2] = {1, IAU_TERM_N2},
  [IAU_COS_S4] = {1, IAU_TERM_S4},
  [IAU_COS_N4] = {1, IAU_TERM_N4},
  [IAU_COS_N5] = {1, IAU_TERM_N5},
  [IAU_COS_N6] = {1, IAU_TERM_N6},
  [IAU_COS_N7] = {1, IAU_TERM_N7},
  [IAU_COS_J8] = {1, IAU_TERM_J8},
  [IAU_COS_J4] = {1, IAU_TERM_J4},
  [IAU_COS_J5] = {1, IAU_TERM_J5},
  [IAU_COS_J6] = {1, IAU_TERM_J6},
  [IAU_COS_J7] = {1, IAU_TERM_J7},
  [IAU_COS_J1] = {1, IAU_TERM_J1},
  [IAU_COS_J2] = {1, IAU_TERM_J2},
  [IAU_COS_JA] = {1, IAU_TERM_JA},
  [IAU_COS_6N7] = {6, IAU_TERM_N7},
  [IAU_COS_2S2] = {2, IAU_TERM_S2},
  [IAU_COS_S2] = {1, IAU_TERM_S2},
  [IAU_COS_2N1] = {2, IAU_TERM_N1},
  [IAU_COS_JC] = {1, IAU_TERM_JC},
};

typedef struct {
  double a;
  iau_sine_id_t sine;
} iau_sine_term_t;

typedef struct {
  double a;
  iau_cosine_id_t cosine;
} iau_cosine_term_t;

static iau_sine_term_t mercury_w_sines[] = {
  {+0.00993822, IAU_SIN_ME1},
  {-0.00104581, IAU_SIN_ME2},
  {-0.00010280, IAU_SIN_ME3},
  {-0.00002364, IAU_SIN_ME4},
  {-0.00000532, IAU_SIN_ME5},
};

static iau_sine_term_t galatea_alpha_sines[] = {
  {+0.70, IAU_SIN_N},
  {-0.07, IAU_SIN_N4},
};

static iau_cosine_term_t galatea_delta_cosines[] = {
  {-0.51, IAU_COS_N},
  {-0.05, IAU_COS_N4},
};

static iau_sine_term_t galatea_w_sines[] = {
  {-0.48, IAU_SIN_N},
  {+0.05, IAU_SIN_N},
};

static iau_sine_term_t thalassa_alpha_sines[] = {
  {+0.70, IAU_SIN_N},
  {-0.28, IAU_SIN_N2},
};

static iau_cosine_term_t thalassa_delta_cosines[] = {
  {-0.51, IAU_COS_N},
  {-0.21, IAU_COS_N2},
};

static iau_sine_term_t thalassa_w_sines[] = {
  {-0.48, IAU_SIN_N},
  {+0.19, IAU_SIN_N2},
};

static iau_sine_term_t ariel_alpha_sines[] = {
  {+0.29, IAU_SIN_U13},
};

static iau_cosine_term_t ariel_delta_cosines[] = {
  {+0.28, IAU_COS_U13},
};

static iau_sine_term_t ariel_w_sines[] = {
  {+0.05, IAU_SIN_U12},
  {+0.08, IAU_SIN_U13},
};

static iau_sine_term_t puck_alpha_sines[] = {
  {-0.33, IAU_SIN_U10},
};

static iau_cosine_term_t puck_delta_cosines[] = {
  {+0.31, IAU_COS_U10},
};

static iau_sine_term_t puck_w_sines[] = {
  {-0.09, IAU_SIN_U10},
};

static iau_sine_term_t phobos_alpha_sines[] = {
  {+1.79, IAU_SIN_M1},
};

static iau_cosine_term_t phobos_delta_cosines[] = {
  {-1.08, IAU_COS_M1},
};

static iau_sine_term_t phobos_w_sines[] = {
  {-1.42, IAU_SIN_M1},
  {-0.78, IAU_SIN_M2},
};

static iau_sine_term_t deimos_alpha_sines[] = {
  {+2.98, IAU_SIN_M3},
};

static iau_cosine_term_t deimos_delta_cosines[] = {
  {-1.78, IAU_COS_M3},
};

static iau_sine_term_t deimos_w_sines[] = {
  {-2.58, IAU_SIN_M3},
};

static iau_cosine_term_t deimos_w_cosines[] = {
  {+0.19, IAU_COS_M3},
};

static iau_sine_term_t rhea_alpha_sines[] = {
  {+3.10, IAU_SIN_S6},
};

static iau_cosine_term_t rhea_delta_cosines[] = {
  {-0.35, IAU_COS_S6},
};

static iau_sine_term_t rhea_w_sines[] = {
  {-3.08, IAU_SIN_S6},
};

static iau_sine_term_t oberon_alpha_sines[] = {
  {+0.16, IAU_SIN_U16},
};

static iau_cosine_term_t oberon_delta_cosines[] = {
  {+0.16, IAU_COS_U16},
};

static iau_sine_term_t oberon_w_sines[] = {
  {+0.04, IAU_SIN_U16},
};

static iau_sine_term_t portia_alpha_sines[] = {
  {-0.09, IAU_SIN_U7},
};

static iau_cosine_term_t portia_delta_cosines[] = {
  {+0.09, IAU_COS_U7},
};

static iau_sine_term_t portia_w_sines[] = {
  {-0.02, IAU_SIN_U7},
};

static iau_sine_term_t europa_alpha_sines[] = {
  {+1.086, IAU_SIN_J4},
  {+0.060, IAU_SIN_J5},
  {+0.015, IAU_SIN_J6},
  {+0.009, IAU_SIN_J7},
};

static iau_cosine_term_t europa_delta_cosines[] = {
  {+0.468, IAU_COS_J4},
  {+0.026, IAU_COS_J5},
  {+0.007, IAU_COS_J6},
  {+0.002, IAU_COS_J7},
};

static iau_sine_term_t europa_w_sines[] = {
  {-0.980, IAU_SIN_J4},
  {-0.054, IAU_SIN_J5},
  {-0.014, IAU_SIN_J6},
  {-0.008, IAU_SIN_J7},
};

static iau_sine_term_t tethys_alpha_sines[] = {
  {+9.66, IAU_SIN_S4},
};

static iau_cosine_term_t tethys_delta_cosines[] = {
  {-1.09, IAU_COS_S4},
};

static iau_sine_term_t tethys_w_sines[] = {
  {-9.60, IAU_SIN_S4},
  {+2.23, IAU_SIN_S5},
};

static iau_sine_term_t ophelia_alpha_sines[] = {
  {-0.09, IAU_SIN_U2},
};

static iau_cosine_term_t ophelia_delta_cosines[] = {
  {+0.09, IAU_COS_U2},
};

static iau_sine_term_t ophelia_w_sines[] = {
  {-0.03, IAU_SIN_U2},
};

static iau_sine_term_t rosalind_alpha_sines[] = {
  {-0.29, IAU_SIN_U8},
};

static iau_cosine_term_t rosalind_delta_cosines[] = {
  {+0.28, IAU_COS_U8},
};

static iau_sine_term_t rosalind_w_sines[] = {
  {-0.08, IAU_SIN_U8},
};

static iau_sine_term_t bianca_alpha_sines[] = {
  {-0.16, IAU_SIN_U3},
};

static iau_cosine_term_t bianca_delta_cosines[] = {
  {+0.16, IAU_COS_U3},
};

static iau_sine_term_t bianca_w_sines[] = {
  {-0.04, IAU_SIN_U3},
};

static iau_sine_term_t proteus_alpha_sines[] = {
  {+0.70, IAU_SIN_N},
  {-0.05, IAU_SIN_N6},
};

static iau_cosine_term_t proteus_delta_cosines[] = {
  {-0.51, IAU_COS_N},
  {-0.04, IAU_COS_N6},
};

static iau_sine_term_t proteus_w_sines[] = {
  {-0.48, IAU_SIN_N},
  {+0.04, IAU_SIN_N6},
};

static iau_sine_term_t cressida_alpha_sines[] = {
  {-0.04, IAU_SIN_U4},
};

static iau_cosine_term_t cressida_delta_cosines[] = {
  {+0.04, IAU_COS_U4},
};

static iau_sine_term_t cressida_w_sines[] = {
  {-0.01, IAU_SIN_U4},
};

static iau_sine_term_t belinda_alpha_sines[] = {
  {-0.03, IAU_SIN_U9},
};

static iau_cosine_term_t belinda_delta_cosines[] = {
  {+0.03, IAU_COS_U9},
};

static iau_sine_term_t belinda_w_sines[] = {
  {-0.01, IAU_SIN_U9},
};

static iau_sine_term_t miranda_alpha_sines[] = {
  {+4.41, IAU_SIN_U11},
  {-0.04, IAU_SIN_2U11},
};

static iau_cosine_term_t miranda_delta_cosines[] = {
  {+4.25, IAU_COS_U11},
  {-0.02, IAU_COS_2U11},
};

static iau_sine_term_t miranda_w_sines[] = {
  {-1.27, IAU_SIN_U12},
  {+0.15, IAU_SIN_2U12},
  {+1.15, IAU_SIN_U11},
  {-0.09, IAU_SIN_2U11},
};

static iau_sine_term_t larissa_alpha_sines[] = {
  {+0.70, IAU_SIN_N},
  {-0.27, IAU_SIN_N5},
};

static iau_cosine_term_t larissa_delta_cosines[] = {
  {-0.51, IAU_COS_N},
  {-0.20, IAU_COS_N5},
};

static iau_sine_term_t larissa_w_sines[] = {
  {-0.48, IAU_SIN_N},
  {+0.19, IAU_SIN_N},
};

static iau_sine_term_t amalthea_alpha_sines[] = {
  {-0.84, IAU_SIN_J1},
  {+0.01, IAU_SIN_2J1},
};

static iau_cosine_term_t amalthea_delta_cosines[] = {
  {-0.36, IAU_COS_J1},
};

static iau_sine_term_t amalthea_w_sines[] = {
  {+0.76, IAU_SIN_J1},
  {-0.01, IAU_SIN_2J1},
};

static iau_sine_term_t moon_alpha_sines[] = {
  {-3.8787, IAU_SIN_E1},
  {-0.1204, IAU_SIN_E2},
  {+0.0700, IAU_SIN_E3},
  {-0.0172, IAU_SIN_E4},
  {+0.0072, IAU_SIN_E6},
  {-0.0052, IAU_SIN_E10},
  {+0.0043, IAU_SIN_E13},
};

static iau_cosine_term_t moon_delta_cosines[] = {
  {+1.5419, IAU_COS_E1},
  {+0.0239, IAU_COS_E2},
  {-0.0278, IAU_COS_E3},
  {+0.0068, IAU_COS_E4},
  {-0.0029, IAU_COS_E6},
  {+0.0009, IAU_COS_E7},
  {+0.0008, IAU_COS_E10},
  {-0.0009, IAU_COS_E13},
};

static iau_sine_term_t moon_w_sines[] = {
  {+3.5610, IAU_SIN_E1},
  {+0.1208, IAU_SIN_E2},
  {-0.0642, IAU_SIN_E3},
  {+0.0158, IAU_SIN_E4},
  {+0.0252, IAU_SIN_E5},
  {-0.0066, IAU_SIN_E6},
  {-0.0047, IAU_SIN_E7},
  {-0.0046, IAU_SIN_E8},
  {+0.0028, IAU_SIN_E9},
  {+0.0052, IAU_SIN_E10},
  {+0.0040, IAU_SIN_E11},
  {+0.0019, IAU_SIN_E12},
  {-0.0044, IAU_SIN_E13},
};

static iau_sine_term_t naiad_alpha_sines[] = {
  {+0.70, IAU_SIN_N},
  {-6.49, IAU_SIN_N1},
  {+0.25, IAU_SIN_2N1},
};

static iau_cosine_term_t naiad_delta_cosines[] = {
  {-0.51, IAU_COS_N},
  {-4.75, IAU_COS_N1},
  {+0.09, IAU_COS_2N1},
};

static iau_sine_term_t naiad_w_sines[] = {
  {-0.48, IAU_SIN_N},
  {+4.40, IAU_SIN_N1},
  {-0.27, IAU_SIN_2N1},
};

static iau_sine_term_t neptune_alpha_sines[] = {
  {+0.70, IAU_SIN_N},
};

static iau_cosine_term_t neptune_delta_cosines[] = {
  {-0.51, IAU_COS_N},
};

static iau_sine_term_t neptune_w_sines[] = {
  {-0.48, IAU_SIN_N},
};

static iau_sine_term_t cordelia_alpha_sines[] = {
  {-0.15, IAU_SIN_U1},
};

static iau_cosine_term_t cordelia_delta_cosines[] = {
  {+0.14, IAU_COS_U1},
};

static iau_sine_term_t cordelia_w_sines[] = {
  {-0.04, IAU_SIN_U1},
};

static iau_sine_term_t titania_alpha_sines[] = {
  {+0.29, IAU_SIN_U15},
};

static iau_cosine_term_t titania_delta_cosines[] = {
  {+0.28, IAU_COS_U15},
};

static iau_sine_term_t titania_w_sines[] = {
  {+0.08, IAU_SIN_U15},
};

static iau_sine_term_t janus_alpha_sines[] = {
  {-1.623, IAU_SIN_S2},
  {+0.023, IAU_SIN_2S2},
};

static iau_cosine_term_t janus_delta_cosines[] = {
  {-0.183, IAU_COS_S2},
  {+0.001, IAU_COS_2S2},
};

static iau_sine_term_t janus_w_sines[] = {
  {+1.613, IAU_SIN_S2},
  {-0.023, IAU_SIN_2S2},
};

static iau_sine_term_t thebe_alpha_sines[] = {
  {-2.11, IAU_SIN_J2},
  {+0.04, IAU_SIN_2J2},
};

static iau_cosine_term_t thebe_delta_cosines[] = {
  {-0.91, IAU_COS_J2},
  {+0.01, IAU_COS_2J2},
};

static iau_sine_term_t thebe_w_sines[] = {
  {+1.91, IAU_SIN_J2},
  {-0.04, IAU_SIN_2J2},
};

static iau_sine_term_t triton_alpha_sines[] = {
  {-32.35, IAU_SIN_N7},
  {-6.28, IAU_SIN_2N7},
  {-2.08, IAU_SIN_3N7},
  {-0.74, IAU_SIN_4N7},
  {-0.28, IAU_SIN_5N7},
  {-0.11, IAU_SIN_6N7},
  {-0.07, IAU_SIN_7N7},
  {-0.02, IAU_SIN_8N7},
  {-0.01, IAU_SIN_9N7},
};

static iau_cosine_term_t triton_delta_cosines[] = {
  {+22.55, IAU_COS_N7},
  {+2.10, IAU_COS_2N7},
  {+0.55, IAU_COS_3N7},
  {+0.16, IAU_COS_4N7},
  {+0.05, IAU_COS_5N7},
  {+0.02, IAU_COS_6N7},
  {+0.01, IAU_COS_7N7},
};

static iau_sine_term_t triton_w_sines[] = {
  {+22.25, IAU_SIN_N7},
  {+6.73, IAU_SIN_2N7},
  {+2.05, IAU_SIN_3N7},
  {+0.74, IAU_SIN_4N7},
  {+0.28, IAU_SIN_5N7},
  {+0.11, IAU_SIN_6N7},
  {+0.05, IAU_SIN_7N7},
  {+0.02, IAU_SIN_8N7},
  {+0.01, IAU_SIN_9N7},
};

static iau_sine_term_t mimas_alpha_sines[] = {
  {+13.56, IAU_SIN_S3},
};

static iau_cosine_term_t mimas_delta_cosines[] = {
  {-1.53, IAU_COS_S3},
};

static iau_sine_term_t mimas_w_sines[] = {
  {-13.48, IAU_SIN_S3},
  {-44.85, IAU_SIN_S5},
};

static iau_sine_term_t ganymede_alpha_sines[] = {
  {-0.037, IAU_SIN_J4},
  {+0.431, IAU_SIN_J5},
  {+0.091, IAU_SIN_J6},
};

static iau_cosine_term_t ganymede_delta_cosines[] = {
  {-0.016, IAU_COS_J4},
  {+0.186, IAU_COS_J5},
  {+0.039, IAU_COS_J6},
};

static iau_sine_term_t ganymede_w_sines[] = {
  {+0.033, IAU_SIN_J4},
  {-0.389, IAU_SIN_J5},
  {-0.082, IAU_SIN_J6},
};

static iau_sine_term_t umbriel_alpha_sines[] = {
  {+0.21, IAU_SIN_U14},
};

static iau_cosine_term_t umbriel_delta_cosines[] = {
  {+0.20, IAU_COS_U14},
};

static iau_sine_term_t umbriel_w_sines[] = {
  {-0.09, IAU_SIN_U12},
  {+0.06, IAU_SIN_U14},
};

static iau_sine_term_t despina_alpha_sines[] = {
  {+0.70, IAU_SIN_N},
  {-0.09, IAU_SIN_N3},
};

static iau_cosine_term_t despina_delta_cosines[] = {
  {-0.51, IAU_COS_N},
  {-0.07, IAU_COS_N3},
};

static iau_sine_term_t despina_w_sines[] = {
  {-0.49, IAU_SIN_N},
  {+0.06, IAU_SIN_N},
};

static iau_sine_term_t callisto_alpha_sines[] = {
  {-0.068, IAU_SIN_J5},
  {+0.590, IAU_SIN_J6},
  {+0.010, IAU_SIN_J8},
};

static iau_cosine_term_t callisto_delta_cosines[] = {
  {-0.029, IAU_COS_J5},
  {+0.254, IAU_COS_J6},
  {-0.004, IAU_COS_J8},
};

static iau_sine_term_t callisto_w_sines[] = {
  {+0.061, IAU_SIN_J5},
  {-0.533, IAU_SIN_J6},
  {-0.009, IAU_SIN_J8},
};

static iau_sine_term_t jupiter_alpha_sines[] = {
  {+0.000117, IAU_SIN_JA},
  {+0.000938, IAU_SIN_JB},
  {+0.001432, IAU_SIN_JC},
  {+0.000030, IAU_SIN_JD},
  {+0.002150, IAU_SIN_JE},
};

static iau_cosine_term_t jupiter_delta_cosines[] = {
  {+0.000050, IAU_COS_JA},
  {+0.000404, IAU_COS_JB},
  {+0.000617, IAU_COS_JC},
  {-0.000013, IAU_COS_JD},
  {+0.000926, IAU_COS_JE},
};

static iau_sine_term_t epimetheus_alpha_sines[] = {
  {-3.153, IAU_SIN_S1},
  {+0.086, IAU_SIN_2S1},
};

static iau_cosine_term_t epimetheus_delta_cosines[] = {
  {-0.356, IAU_COS_S1},
  {+0.005, IAU_COS_2S1},
};

static iau_sine_term_t epimetheus_w_sines[] = {
  {+3.133, IAU_SIN_S1},
  {-0.086, IAU_SIN_2S1},
};

static iau_sine_term_t io_alpha_sines[] = {
  {+0.094, IAU_SIN_J3},
  {+0.024, IAU_SIN_J4},
};

static iau_cosine_term_t io_delta_cosines[] = {
  {+0.040, IAU_COS_J3},
  {+0.011, IAU_COS_J4},
};

static iau_sine_term_t io_w_sines[] = {
  {-0.085, IAU_SIN_J3},
  {-0.022, IAU_SIN_J4},
};

static iau_sine_term_t desdemona_alpha_sines[] = {
  {-0.17, IAU_SIN_U5},
};

static iau_cosine_term_t desdemona_delta_cosines[] = {
  {+0.16, IAU_COS_U5},
};

static iau_sine_term_t desdemona_w_sines[] = {
  {-0.04, IAU_SIN_U5},
};

static iau_sine_term_t juliet_alpha_sines[] = {
  {-0.06, IAU_SIN_U6},
};

static iau_cosine_term_t juliet_delta_cosines[] = {
  {+0.06, IAU_COS_U6},
};

static iau_sine_term_t juliet_w_sines[] = {
  {-0.02, IAU_SIN_U6},
};

typedef struct {
  const char *name;
  double a0;
  double aT;
  double d0;
  double dT;
  double w0;
  double wd;
  double wd_2;
  double wT_2;
  iau_sine_term_t *alpha_sines;
  iau_cosine_term_t *delta_cosines;
  iau_sine_term_t *w_sines;
  iau_cosine_term_t *w_cosines;
  size_t asin_count;
  size_t dcos_count;
  size_t wsin_count;
  size_t wcos_count;
} iau_body_t;

iau_body_t bodies[IAU_BODY_COUNT] = {
  {"19p_borrelly", 218.5, 0.0, -12.5, 0.0, 000, +390.0, 0.0, 0.0 , NULL, NULL, NULL, NULL, 0, 0, 0, 0},
  {"9p_tempel_1", 294, 0.0, 73, 0.0, 252.63, +212.064, 0.0, 0.0 , NULL, NULL, NULL, NULL, 0, 0, 0, 0},
  {"adrastea", 268.05, -0.009, 64.49, +0.003, 33.29, +1206.9986602, 0.0, 0.0 , NULL, NULL, NULL, NULL, 0, 0, 0, 0},
  {"amalthea", 268.05, -0.009, 64.49, +0.003, 231.67, +722.6314560, 0.0, 0.0 , amalthea_alpha_sines, amalthea_delta_cosines, amalthea_w_sines, NULL, sizeof(amalthea_alpha_sines)/sizeof(iau_sine_term_t), sizeof(amalthea_delta_cosines)/sizeof(iau_cosine_term_t), sizeof(amalthea_w_sines)/sizeof(iau_sine_term_t), 0},
  {"ariel", 257.43, 0.0, -15.10, 0.0, 156.22, -142.8356681, 0.0, 0.0 , ariel_alpha_sines, ariel_delta_cosines, ariel_w_sines, NULL, sizeof(ariel_alpha_sines)/sizeof(iau_sine_term_t), sizeof(ariel_delta_cosines)/sizeof(iau_cosine_term_t), sizeof(ariel_w_sines)/sizeof(iau_sine_term_t), 0},
  {"atlas", 40.58, -0.036, 83.53, -0.004, 137.88, +598.3060000, 0.0, 0.0 , NULL, NULL, NULL, NULL, 0, 0, 0, 0},
  {"belinda", 257.31, 0.0, -15.18, 0.0, 297.46, -577.3628170, 0.0, 0.0 , belinda_alpha_sines, belinda_delta_cosines, belinda_w_sines, NULL, sizeof(belinda_alpha_sines)/sizeof(iau_sine_term_t), sizeof(belinda_delta_cosines)/sizeof(iau_cosine_term_t), sizeof(belinda_w_sines)/sizeof(iau_sine_term_t), 0},
  {"bianca", 257.31, 0.0, -15.18, 0.0, 105.46, -828.3914760, 0.0, 0.0 , bianca_alpha_sines, bianca_delta_cosines, bianca_w_sines, NULL, sizeof(bianca_alpha_sines)/sizeof(iau_sine_term_t), sizeof(bianca_delta_cosines)/sizeof(iau_cosine_term_t), sizeof(bianca_w_sines)/sizeof(iau_sine_term_t), 0},
  {"callisto", 268.72, -0.009, 64.83, +0.003, 259.51, +21.5710715, 0.0, 0.0 , callisto_alpha_sines, callisto_delta_cosines, callisto_w_sines, NULL, sizeof(callisto_alpha_sines)/sizeof(iau_sine_term_t), sizeof(callisto_delta_cosines)/sizeof(iau_cosine_term_t), sizeof(callisto_w_sines)/sizeof(iau_sine_term_t), 0},
  {"calypso", 36.41, -0.036, 85.04, -0.004, 153.51, +190.6742373, 0.0, 0.0 , NULL, NULL, NULL, NULL, 0, 0, 0, 0},
  {"ceres", 291, 0.0, 59, 0.0, 170.90, +952.1532, 0.0, 0.0 , NULL, NULL, NULL, NULL, 0, 0, 0, 0},
  {"charon", 132.993, 0.0, -6.163, 0.0, 122.695, +56.3625225, 0.0, 0.0 , NULL, NULL, NULL, NULL, 0, 0, 0, 0},
  {"cordelia", 257.31, 0.0, -15.18, 0.0, 127.69, -1074.5205730, 0.0, 0.0 , cordelia_alpha_sines, cordelia_delta_cosines, cordelia_w_sines, NULL, sizeof(cordelia_alpha_sines)/sizeof(iau_sine_term_t), sizeof(cordelia_delta_cosines)/sizeof(iau_cosine_term_t), sizeof(cordelia_w_sines)/sizeof(iau_sine_term_t), 0},
  {"cressida", 257.31, 0.0, -15.18, 0.0, 59.16, -776.5816320, 0.0, 0.0 , cressida_alpha_sines, cressida_delta_cosines, cressida_w_sines, NULL, sizeof(cressida_alpha_sines)/sizeof(iau_sine_term_t), sizeof(cressida_delta_cosines)/sizeof(iau_cosine_term_t), sizeof(cressida_w_sines)/sizeof(iau_sine_term_t), 0},
  {"davida", 297, 0.0, 5, 0.0, 268.1, +1684.4193549, 0.0, 0.0 , NULL, NULL, NULL, NULL, 0, 0, 0, 0},
  {"deimos", 316.65, -0.108, 53.52, -0.061, 79.41, +285.1618970, 0.0, 0.0 , deimos_alpha_sines, deimos_delta_cosines, deimos_w_sines, deimos_w_cosines, sizeof(deimos_alpha_sines)/sizeof(iau_sine_term_t), sizeof(deimos_delta_cosines)/sizeof(iau_cosine_term_t), sizeof(deimos_w_sines)/sizeof(iau_sine_term_t), sizeof(deimos_w_cosines)/sizeof(iau_cosine_term_t)},
  {"desdemona", 257.31, 0.0, -15.18, 0.0, 95.08, -760.0531690, 0.0, 0.0 , desdemona_alpha_sines, desdemona_delta_cosines, desdemona_w_sines, NULL, sizeof(desdemona_alpha_sines)/sizeof(iau_sine_term_t), sizeof(desdemona_delta_cosines)/sizeof(iau_cosine_term_t), sizeof(desdemona_w_sines)/sizeof(iau_sine_term_t), 0},
  {"despina", 299.36, 0.0, 43.45, 0.0, 306.51, +1075.7341562, 0.0, 0.0 , despina_alpha_sines, despina_delta_cosines, despina_w_sines, NULL, sizeof(despina_alpha_sines)/sizeof(iau_sine_term_t), sizeof(despina_delta_cosines)/sizeof(iau_cosine_term_t), sizeof(despina_w_sines)/sizeof(iau_sine_term_t), 0},
  {"dione", 40.66, -0.036, 83.52, -0.004, 357.6, +131.5349316, 0.0, 0.0 , NULL, NULL, NULL, NULL, 0, 0, 0, 0},
  {"earth", 0.00, -0.641, 90.00, -0.557, 190.147, +360.9856235, 0.0, 0.0 , NULL, NULL, NULL, NULL, 0, 0, 0, 0},
  {"enceladus", 40.66, -0.036, 83.52, -0.004, 6.32, +262.7318996, 0.0, 0.0 , NULL, NULL, NULL, NULL, 0, 0, 0, 0},
  {"epimetheus", 40.58, -0.036, 83.52, -0.004, 293.87, +518.4907239, 0.0, 0.0 , epimetheus_alpha_sines, epimetheus_delta_cosines, epimetheus_w_sines, NULL, sizeof(epimetheus_alpha_sines)/sizeof(iau_sine_term_t), sizeof(epimetheus_delta_cosines)/sizeof(iau_cosine_term_t), sizeof(epimetheus_w_sines)/sizeof(iau_sine_term_t), 0},
  {"eros", 11.35, 0.0, 17.22, 0.0, 326.07, +1639.38864745, 0.0, 0.0 , NULL, NULL, NULL, NULL, 0, 0, 0, 0},
  {"europa", 268.08, -0.009, 64.51, +0.003, 36.022, +101.3747235, 0.0, 0.0 , europa_alpha_sines, europa_delta_cosines, europa_w_sines, NULL, sizeof(europa_alpha_sines)/sizeof(iau_sine_term_t), sizeof(europa_delta_cosines)/sizeof(iau_cosine_term_t), sizeof(europa_w_sines)/sizeof(iau_sine_term_t), 0},
  {"galatea", 299.36, 0.0, 43.43, 0.0, 258.09, +839.6597686, 0.0, 0.0 , galatea_alpha_sines, galatea_delta_cosines, galatea_w_sines, NULL, sizeof(galatea_alpha_sines)/sizeof(iau_sine_term_t), sizeof(galatea_delta_cosines)/sizeof(iau_cosine_term_t), sizeof(galatea_w_sines)/sizeof(iau_sine_term_t), 0},
  {"ganymede", 268.20, -0.009, 64.57, +0.003, 44.064, +50.3176081, 0.0, 0.0 , ganymede_alpha_sines, ganymede_delta_cosines, ganymede_w_sines, NULL, sizeof(ganymede_alpha_sines)/sizeof(iau_sine_term_t), sizeof(ganymede_delta_cosines)/sizeof(iau_cosine_term_t), sizeof(ganymede_w_sines)/sizeof(iau_sine_term_t), 0},
  {"gaspra", 9.47, 0.0, 26.70, 0.0, 83.67, +1226.9114850, 0.0, 0.0 , NULL, NULL, NULL, NULL, 0, 0, 0, 0},
  {"helene", 40.85, -0.036, 83.34, -0.004, 245.12, +131.6174056, 0.0, 0.0 , NULL, NULL, NULL, NULL, 0, 0, 0, 0},
  {"iapetus", 318.16, -3.949, 75.03, -1.143, 355.2, +4.5379572, 0.0, 0.0 , NULL, NULL, NULL, NULL, 0, 0, 0, 0},
  {"ida", 168.76, 0.0, -2.88, 0.0, 274.05, +1864.6280070, 0.0, 0.0 , NULL, NULL, NULL, NULL, 0, 0, 0, 0},
  {"io", 268.05, -0.009, 64.50, +0.003, 200.39, +203.4889538, 0.0, 0.0 , io_alpha_sines, io_delta_cosines, io_w_sines, NULL, sizeof(io_alpha_sines)/sizeof(iau_sine_term_t), sizeof(io_delta_cosines)/sizeof(iau_cosine_term_t), sizeof(io_w_sines)/sizeof(iau_sine_term_t), 0},
  {"itokawa", 90.53, 0.0, -66.30, 0.0, 000, +712.143, 0.0, 0.0 , NULL, NULL, NULL, NULL, 0, 0, 0, 0},
  {"janus", 40.58, -0.036, 83.52, -0.004, 58.83, +518.2359876, 0.0, 0.0 , janus_alpha_sines, janus_delta_cosines, janus_w_sines, NULL, sizeof(janus_alpha_sines)/sizeof(iau_sine_term_t), sizeof(janus_delta_cosines)/sizeof(iau_cosine_term_t), sizeof(janus_w_sines)/sizeof(iau_sine_term_t), 0},
  {"juliet", 257.31, 0.0, -15.18, 0.0, 302.56, -730.1253660, 0.0, 0.0 , juliet_alpha_sines, juliet_delta_cosines, juliet_w_sines, NULL, sizeof(juliet_alpha_sines)/sizeof(iau_sine_term_t), sizeof(juliet_delta_cosines)/sizeof(iau_cosine_term_t), sizeof(juliet_w_sines)/sizeof(iau_sine_term_t), 0},
  {"jupiter", 268.056595, -0.006499, 64.495303, +0.002413, 284.95, +870.5360000, 0.0, 0.0 , jupiter_alpha_sines, jupiter_delta_cosines, NULL, NULL, sizeof(jupiter_alpha_sines)/sizeof(iau_sine_term_t), sizeof(jupiter_delta_cosines)/sizeof(iau_cosine_term_t), 0, 0},
  {"larissa", 299.36, 0.0, 43.41, 0.0, 179.41, +649.0534470, 0.0, 0.0 , larissa_alpha_sines, larissa_delta_cosines, larissa_w_sines, NULL, sizeof(larissa_alpha_sines)/sizeof(iau_sine_term_t), sizeof(larissa_delta_cosines)/sizeof(iau_cosine_term_t), sizeof(larissa_w_sines)/sizeof(iau_sine_term_t), 0},
  {"lutetia", 52, 0.0, 12, 0.0, 94, +1057.7515, 0.0, 0.0 , NULL, NULL, NULL, NULL, 0, 0, 0, 0},
  {"mars", 317.68143, -0.1061, 52.88650, -0.0609, 176.630, +350.89198226, 0.0, 0.0 , NULL, NULL, NULL, NULL, 0, 0, 0, 0},
  {"mercury", 281.0097, -0.0328, 61.4143, -0.0049, 329.5469, +6.1385025, 0.0, 0.0 , NULL, NULL, mercury_w_sines, NULL, 0, 0, sizeof(mercury_w_sines)/sizeof(iau_sine_term_t), 0},
  {"metis", 268.05, -0.009, 64.49, +0.003, 346.09, +1221.2547301, 0.0, 0.0 , NULL, NULL, NULL, NULL, 0, 0, 0, 0},
  {"mimas", 40.66, -0.036, 83.52, -0.004, 333.46, +381.9945550, 0.0, 0.0 , mimas_alpha_sines, mimas_delta_cosines, mimas_w_sines, NULL, sizeof(mimas_alpha_sines)/sizeof(iau_sine_term_t), sizeof(mimas_delta_cosines)/sizeof(iau_cosine_term_t), sizeof(mimas_w_sines)/sizeof(iau_sine_term_t), 0},
  {"miranda", 257.43, 0.0, -15.08, 0.0, 30.70, -254.6906892, 0.0, 0.0 , miranda_alpha_sines, miranda_delta_cosines, miranda_w_sines, NULL, sizeof(miranda_alpha_sines)/sizeof(iau_sine_term_t), sizeof(miranda_delta_cosines)/sizeof(iau_cosine_term_t), sizeof(miranda_w_sines)/sizeof(iau_sine_term_t), 0},
  {"moon", 269.9949, +0.0031, 66.5392, +0.0130, 38.3213, +13.17635815, -1.4e-12, -1.4e-12 , moon_alpha_sines, moon_delta_cosines, moon_w_sines, NULL, sizeof(moon_alpha_sines)/sizeof(iau_sine_term_t), sizeof(moon_delta_cosines)/sizeof(iau_cosine_term_t), sizeof(moon_w_sines)/sizeof(iau_sine_term_t), 0},
  {"naiad", 299.36, 0.0, 43.36, 0.0, 254.06, +1222.8441209, 0.0, 0.0 , naiad_alpha_sines, naiad_delta_cosines, naiad_w_sines, NULL, sizeof(naiad_alpha_sines)/sizeof(iau_sine_term_t), sizeof(naiad_delta_cosines)/sizeof(iau_cosine_term_t), sizeof(naiad_w_sines)/sizeof(iau_sine_term_t), 0},
  {"neptune", 299.36, 0.0, 43.46, 0.0, 253.18, +536.3128492, 0.0, 0.0 , neptune_alpha_sines, neptune_delta_cosines, neptune_w_sines, NULL, sizeof(neptune_alpha_sines)/sizeof(iau_sine_term_t), sizeof(neptune_delta_cosines)/sizeof(iau_cosine_term_t), sizeof(neptune_w_sines)/sizeof(iau_sine_term_t), 0},
  {"oberon", 257.43, 0.0, -15.10, 0.0, 6.77, -26.7394932, 0.0, 0.0 , oberon_alpha_sines, oberon_delta_cosines, oberon_w_sines, NULL, sizeof(oberon_alpha_sines)/sizeof(iau_sine_term_t), sizeof(oberon_delta_cosines)/sizeof(iau_cosine_term_t), sizeof(oberon_w_sines)/sizeof(iau_sine_term_t), 0},
  {"ophelia", 257.31, 0.0, -15.18, 0.0, 130.35, -956.4068150, 0.0, 0.0 , ophelia_alpha_sines, ophelia_delta_cosines, ophelia_w_sines, NULL, sizeof(ophelia_alpha_sines)/sizeof(iau_sine_term_t), sizeof(ophelia_delta_cosines)/sizeof(iau_cosine_term_t), sizeof(ophelia_w_sines)/sizeof(iau_sine_term_t), 0},
  {"pallas", 33, 0.0, -3, 0.0, 38, +1105.8036, 0.0, 0.0 , NULL, NULL, NULL, NULL, 0, 0, 0, 0},
  {"pan", 40.6, -0.036, 83.5, -0.004, 48.8, +626.0440000, 0.0, 0.0 , NULL, NULL, NULL, NULL, 0, 0, 0, 0},
  {"pandora", 40.58, -0.036, 83.53, -0.004, 162.92, +572.7891000, 0.0, 0.0 , NULL, NULL, NULL, NULL, 0, 0, 0, 0},
  {"phobos", 317.68, -0.108, 52.90, -0.061, 35.06, +1128.8445850, 0.0, 0.0 , phobos_alpha_sines, phobos_delta_cosines, phobos_w_sines, NULL, sizeof(phobos_alpha_sines)/sizeof(iau_sine_term_t), sizeof(phobos_delta_cosines)/sizeof(iau_cosine_term_t), sizeof(phobos_w_sines)/sizeof(iau_sine_term_t), 0},
  {"phoebe", 356.90, 0.0, 77.80, 0.0, 178.58, +931.639, 0.0, 0.0 , NULL, NULL, NULL, NULL, 0, 0, 0, 0},
  {"pluto", 132.993, 0.0, -6.163, 0.0, 302.695, +56.3625225, 0.0, 0.0 , NULL, NULL, NULL, NULL, 0, 0, 0, 0},
  {"portia", 257.31, 0.0, -15.18, 0.0, 25.03, -701.4865870, 0.0, 0.0 , portia_alpha_sines, portia_delta_cosines, portia_w_sines, NULL, sizeof(portia_alpha_sines)/sizeof(iau_sine_term_t), sizeof(portia_delta_cosines)/sizeof(iau_cosine_term_t), sizeof(portia_w_sines)/sizeof(iau_sine_term_t), 0},
  {"prometheus", 40.58, -0.036, 83.53, -0.004, 296.14, +587.289000, 0.0, 0.0 , NULL, NULL, NULL, NULL, 0, 0, 0, 0},
  {"proteus", 299.27, 0.0, 42.91, 0.0, 93.38, +320.7654228, 0.0, 0.0 , proteus_alpha_sines, proteus_delta_cosines, proteus_w_sines, NULL, sizeof(proteus_alpha_sines)/sizeof(iau_sine_term_t), sizeof(proteus_delta_cosines)/sizeof(iau_cosine_term_t), sizeof(proteus_w_sines)/sizeof(iau_sine_term_t), 0},
  {"puck", 257.31, 0.0, -15.18, 0.0, 91.24, -472.5450690, 0.0, 0.0 , puck_alpha_sines, puck_delta_cosines, puck_w_sines, NULL, sizeof(puck_alpha_sines)/sizeof(iau_sine_term_t), sizeof(puck_delta_cosines)/sizeof(iau_cosine_term_t), sizeof(puck_w_sines)/sizeof(iau_sine_term_t), 0},
  {"rhea", 40.38, -0.036, 83.55, -0.004, 235.16, +79.6900478, 0.0, 0.0 , rhea_alpha_sines, rhea_delta_cosines, rhea_w_sines, NULL, sizeof(rhea_alpha_sines)/sizeof(iau_sine_term_t), sizeof(rhea_delta_cosines)/sizeof(iau_cosine_term_t), sizeof(rhea_w_sines)/sizeof(iau_sine_term_t), 0},
  {"rosalind", 257.31, 0.0, -15.18, 0.0, 314.90, -644.6311260, 0.0, 0.0 , rosalind_alpha_sines, rosalind_delta_cosines, rosalind_w_sines, NULL, sizeof(rosalind_alpha_sines)/sizeof(iau_sine_term_t), sizeof(rosalind_delta_cosines)/sizeof(iau_cosine_term_t), sizeof(rosalind_w_sines)/sizeof(iau_sine_term_t), 0},
  {"saturn", 40.589, -0.036, 83.537, -0.004, 38.90, +810.7939024, 0.0, 0.0 , NULL, NULL, NULL, NULL, 0, 0, 0, 0},
  {"steins", 90, 0.0, -62, 0.0, 93.94, +1428.852332, 0.0, 0.0 , NULL, NULL, NULL, NULL, 0, 0, 0, 0},
  {"sun", 286.13, 0.0, 63.87, 0.0, 84.176, +14.1844000, 0.0, 0.0 , NULL, NULL, NULL, NULL, 0, 0, 0, 0},
  {"telesto", 50.51, -0.036, 84.06, -0.004, 56.88, +190.6979332, 0.0, 0.0 , NULL, NULL, NULL, NULL, 0, 0, 0, 0},
  {"tethys", 40.66, -0.036, 83.52, -0.004, 8.95, +190.6979085, 0.0, 0.0 , tethys_alpha_sines, tethys_delta_cosines, tethys_w_sines, NULL, sizeof(tethys_alpha_sines)/sizeof(iau_sine_term_t), sizeof(tethys_delta_cosines)/sizeof(iau_cosine_term_t), sizeof(tethys_w_sines)/sizeof(iau_sine_term_t), 0},
  {"thalassa", 299.36, 0.0, 43.45, 0.0, 102.06, +1155.7555612, 0.0, 0.0 , thalassa_alpha_sines, thalassa_delta_cosines, thalassa_w_sines, NULL, sizeof(thalassa_alpha_sines)/sizeof(iau_sine_term_t), sizeof(thalassa_delta_cosines)/sizeof(iau_cosine_term_t), sizeof(thalassa_w_sines)/sizeof(iau_sine_term_t), 0},
  {"thebe", 268.05, -0.009, 64.49, +0.003, 8.56, +533.7004100, 0.0, 0.0 , thebe_alpha_sines, thebe_delta_cosines, thebe_w_sines, NULL, sizeof(thebe_alpha_sines)/sizeof(iau_sine_term_t), sizeof(thebe_delta_cosines)/sizeof(iau_cosine_term_t), sizeof(thebe_w_sines)/sizeof(iau_sine_term_t), 0},
  {"titan", 39.4827, 0.0, 83.4279, 0.0, 186.5855, +22.5769768, 0.0, 0.0 , NULL, NULL, NULL, NULL, 0, 0, 0, 0},
  {"titania", 257.43, 0.0, -15.10, 0.0, 77.74, -41.3514316, 0.0, 0.0 , titania_alpha_sines, titania_delta_cosines, titania_w_sines, NULL, sizeof(titania_alpha_sines)/sizeof(iau_sine_term_t), sizeof(titania_delta_cosines)/sizeof(iau_cosine_term_t), sizeof(titania_w_sines)/sizeof(iau_sine_term_t), 0},
  {"triton", 299.36, 0.0, 41.17, 0.0, 296.53, -61.2572637, 0.0, 0.0 , triton_alpha_sines, triton_delta_cosines, triton_w_sines, NULL, sizeof(triton_alpha_sines)/sizeof(iau_sine_term_t), sizeof(triton_delta_cosines)/sizeof(iau_cosine_term_t), sizeof(triton_w_sines)/sizeof(iau_sine_term_t), 0},
  {"umbriel", 257.43, 0.0, -15.10, 0.0, 108.05, -86.8688923, 0.0, 0.0 , umbriel_alpha_sines, umbriel_delta_cosines, umbriel_w_sines, NULL, sizeof(umbriel_alpha_sines)/sizeof(iau_sine_term_t), sizeof(umbriel_delta_cosines)/sizeof(iau_cosine_term_t), sizeof(umbriel_w_sines)/sizeof(iau_sine_term_t), 0},
  {"uranus", 257.311, 0.0, -15.175, 0.0, 203.81, -501.1600928, 0.0, 0.0 , NULL, NULL, NULL, NULL, 0, 0, 0, 0},
  {"venus", 272.76, 0.0, 67.16, 0.0, 160.20, -1.4813688, 0.0, 0.0 , NULL, NULL, NULL, NULL, 0, 0, 0, 0},
  {"vesta", 305.8, 0.0, 41.4, 0.0, 292, +1617.332776, 0.0, 0.0 , NULL, NULL, NULL, NULL, 0, 0, 0, 0},
};

