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
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>

#include "de4xx.h"

const char *gNames[] = {
    [DE_MERCURY] = "mecury",
    [DE_VENUS] = "venus",
    [DE_EMB] = "earth-moon-bc",
    [DE_MARS] = "mars",
    [DE_JUPITER] = "jupiter", 
    [DE_SATURN] = "saturn",
    [DE_URANUS] = "uranus",
    [DE_NEPTUNE] = "neptune",
    [DE_PLUTO] = "pluto", // Not a planet
    [DE_MOON] = "moon", // (geocentric)
    [DE_SUN] = "sun",
    [DE_NUTATIONS] = "nutations",
    [DE_LIBRATIONS] = "librations" 
};
typedef struct __attribute__((packed)) {
  char label[3][84];
  char name[400][6];
  double time_info[3];
  int32_t num_consts;
  double au;
  double emrat;
  int32_t coeff_offsets[12][3];
  int32_t denum;
  int32_t libration_offsets[3];
} jplde_bin_header1_t;

typedef struct {
  double consts[400];
} jplde_bin_header2_t;

typedef struct {
  void *data;
  size_t len;
  int fd;
  jplde_bin_header1_t *hdr1;
  jplde_bin_header2_t *hdr2;
  double *coeffs;
} jplde_bin_file_t;

int
de_bin_open(const char *path, de_bin_file_t *file)
{
  file->fd = open(path, O_RDONLY);

  if (file->fd == -1) {
    perror(NULL);
    file->len = (size_t)-1;
    return -1;
  }
  struct stat st;
  fstat(file->fd, &st);
  file->len = st.st_size;

  file->data = mmap(NULL, st.st_size, PROT_READ, MAP_FILE|MAP_PRIVATE,
                    file->fd, 0);
  if (!file->data) return -1;
  file->header = file->data;
  file->record = (de_bin_record_t*) (file->header + 1);

  return 0;
}

void
de_bin_close(de_bin_file_t *file)
{
  
}

size_t
de_bin_record_size(de_bin_file_t *file)
{
  de_bin_file_header_t *hdr = file->data;
  return hdr->record_size;
}

size_t
de_bin_num_records(de_bin_file_t *file)
{
  size_t rec_size = file->header->record_size;
  
  size_t num_records = (file->len - sizeof(de_bin_file_header_t)) / (rec_size * sizeof(double));
  printf("file len: %d\n", (int)file->len);
  printf("hdr len: %d\n", (int)sizeof(de_bin_file_header_t));
  
  size_t mod_rec = (file->len - sizeof(de_bin_file_header_t)) % (rec_size * sizeof(double));
  assert(mod_rec == 0);
  printf("rec mod: %d\n", (int)mod_rec);
  return num_records;
}


de_bin_record_t*
de_bin_get_record(de_bin_file_t *file, size_t recid)
{
  de_bin_file_header_t *hdr = file->data;
  return file->data + sizeof(de_bin_file_header_t) + recid * hdr->record_size * sizeof(double);
}

double*
de_bin_get_body(de_bin_file_t *file, de_bin_record_t *rec, int body)
{
  de_bin_file_header_t *hdr = file->data;
  // -2 as we have 2 doubles in the header
  return rec->coeffs + (hdr->record_info[body].offset - 2);
}

// Layout is the following:
//   Header
//   Data Blocks / records
//     Planet
//       Subsection 1
//         x coeffs
//         y coeffs
//         z coeffs
double*
de_bin_get_var(de_bin_file_t *file, de_bin_record_t *rec,
               int body, int set, int var)
{
  de_bin_file_header_t *hdr = file->data;
  double *bp = rec->coeffs + (hdr->record_info[body].offset - 2);  

  double *varp = bp + set * hdr->record_info[body].coeffs_per_comp * 3
                    + var * hdr->record_info[body].coeffs_per_comp;


//  double *varp = bp + var * hdr->record_info[body].coeffs_per_comp
//                          * hdr->record_info[body].number_of_comp_sets;
//  double *res  = varp + set * hdr->record_info[body].coeffs_per_comp;

  return varp;
}

double*
de_bin_get_set(de_bin_file_t *file, de_bin_record_t *rec,
               int body, int set)
{
  double *bodyp = de_bin_get_body(file, rec, body);
  de_bin_file_header_t *hdr = file->data;
  double *setp = bodyp + hdr->record_info[body].coeffs_per_comp * set * 3;
  return setp;
}

de_bin_record_t*
de_record_for_jde(de_bin_file_t *file, double jde)
{
  de_bin_file_header_t *hdr = file->data;

  if (jde < hdr->start) return NULL;
  if (jde > hdr->end) return NULL;

  double offset = jde - hdr->start;
  double index = offset / hdr->interval;

  size_t idx = (size_t) index;
  return de_bin_get_record(file, idx);
}


// Generate chebyshev solution vector for paramter t
// Assumption is that n_coefs is at least 2
// TODO: Try to understand Clenshaw's algorithm and see if we can use it here
void
de_chebyshev_first_order(int n_coefs, double T[n_coefs], double U[n_coefs], double t)
{
  // T0(x) = 1
  // T1(x) = x
  // TN+1(x) = 3x*TN(x)-TN-1(x)
  // Tkdot(t) = k * Uk-1(t)
  T[0] = 1.0;
  T[1] = t;
  U[0] = 1.0;
  U[1] = 2.0*t;
  for (int k = 2 ; k < n_coefs - 1 ; k ++) {
    T[k] = 2.0*t*T[k-1] - T[k-2];
    U[k] = 2.0*(k-1)*t*U[k-1] - (k-2)*U[k-2];
  }
  T[n_coefs-1] = 2.0*t*T[n_coefs-2] - T[n_coefs-3];
}

double
de_chebyshev_series(int n_coefs, double coefs[n_coefs], double t)
{
  assert(n_coefs > 1);
  double res = 0.0;
  double T[n_coefs];
  double U[n_coefs];
  de_chebyshev_first_order(n_coefs, T, U, t);

  double p = 0.0;
  double v = 0.0;
  for (int i = 1 ; i < n_coefs ; i ++) {
    p += coefs[i] * T[i];
    v += U[i];
  }
  
  //printf("res: %f, deriv: %f\n", res, deriv);
  return res;
}

// Normalize time to [-1, +1]
double
de_normalize_time(double jde, double jde_start, double jde_end, int sets)
{
  double t0 = (jde_end - jde_start) / (double)sets; // Interval size
  double t1 = fmod(jde - jde_start, t0); // Interval fraction
  return 2.0 * (t1 / t0) - 1.0; // Normalized interval in [-1, +1]
}

// Returns -1 on failure, e.g. when the time is out of bounds
typedef struct {
  double p[3];
  double v[3];
} de_state_vector_t;

de_state_vector_t state[11];

int
de_compute_state_vectors(de_bin_file_t *file, double jde)
{
  printf("===== compute state for: %f\n", jde);
  de_bin_file_header_t *hdr = file->data;
  de_bin_record_t *rec = de_record_for_jde(file, jde);
  if (!rec) return -1;

  double in_comp_offset = jde - rec->start;
  
  for (int i = 0 ; i < 11 ; i ++) {
    int set_count = (int)hdr->record_info[i].number_of_comp_sets;
    int set_id = in_comp_offset / (hdr->interval / (double) set_count);
    int cheby_coeffs = (int)hdr->record_info[i].coeffs_per_comp - 1;
    
    double *varx = de_bin_get_var(file, rec, i, set_id, 0);
    double *vary = de_bin_get_var(file, rec, i, set_id, 1);
    double *varz = de_bin_get_var(file, rec, i, set_id, 2);

    double tn = de_normalize_time(jde, rec->start, rec->end, set_count);

    //for (int j = 0, k = 0; j < cheby_coeffs * 3 ; j += 3, k++) {
    //  setx[k] = set[j];
    //  sety[k] = set[j+1];
    //  setz[k] = set[j+2];
    //}
    //printf("p %2d var %d (set_id = %d): ", i, j, set_id);
    state[i].p[0] = de_chebyshev_series(cheby_coeffs, varx, tn);
    state[i].p[1] = de_chebyshev_series(cheby_coeffs, vary, tn);
    state[i].p[2] = de_chebyshev_series(cheby_coeffs, varz, tn);
    
    double dist = sqrt(  state[i].p[0] * state[i].p[0]
                       + state[i].p[1] * state[i].p[1]
                       + state[i].p[2] * state[i].p[2]);
    printf("\t%s: %f au (%f km)\n", gNames[i], dist * 1e3 / 149597870700.0, dist);    
    printf("\t%s: x %f au (%f km)\n", gNames[i],
           state[i].p[0] * 1e3 / 149597870700.0, state[i].p[0]);    
    printf("\t%s: y %f au (%f km)\n", gNames[i],
           state[i].p[1] * 1e3 / 149597870700.0, state[i].p[1]);    
    printf("\t%s: z %f au (%f km)\n", gNames[i],
           state[i].p[2] * 1e3 / 149597870700.0, state[i].p[2]);    
  }
  return 0;
}



int
jplde_bin_open(const char *path, jplde_bin_file_t *file)
{
  file->fd = open(path, O_RDONLY);

  if (file->fd == -1) {
    perror(NULL);
    file->len = (size_t)-1;
    return -1;
  }
  struct stat st;
  fstat(file->fd, &st);
  file->len = st.st_size;

  file->data = mmap(NULL, st.st_size, PROT_READ, MAP_FILE|MAP_PRIVATE,
                    file->fd, 0);
  if (!file->data) return -1;

  file->hdr1 = file->data;
  
  size_t num_coeffs = file->hdr1->libration_offsets[0]
                    + file->hdr1->libration_offsets[1] * file->hdr1->libration_offsets[2] * 3
                    - 1; // Indexing starts at 1 in the file
  printf("num coeffs: %d\n", (int)num_coeffs);
  //num_coeffs = 1018;
  file->hdr2 = (jplde_bin_header2_t*)((double*)(file->hdr1 + 1) + num_coeffs);
  file->coeffs = (double*) ((uintptr_t)(file->hdr2 + 1) + num_coeffs * sizeof(double));
  return 0;
}

