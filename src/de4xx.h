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

#ifndef ORBIT_DE4XX_H
#define ORBIT_DE4XX_H

#include <stdint.h>
// Digital Empheris Reader
typedef enum {
  DE_MERCURY,
  DE_VENUS,
  DE_EMB,
  DE_MARS,
  DE_JUPITER, 
  DE_SATURN,
  DE_URANUS,
  DE_NEPTUNE,
  DE_PLUTO, // Not a planet
  DE_MOON, // (geocentric)
  DE_SUN,
  DE_NUTATIONS, // Not supported for now
  DE_LIBRATIONS // Not supported for now 
} de_series_id_t;

// The ASCII format must be preprocessed into binary
// Our binary format is made for direct memory mapping. The format is general
// but only supports the cartesian components for now.
// In the file, first there are two doubles containing the range of the whole
// series. After this, the header data in the form of the offset, cooef count
// and number of component sets.

typedef struct {
  uint64_t offset; // 64 bit to ensure alignment of doubles
  uint64_t coeffs_per_comp;
  uint64_t number_of_comp_sets;
} de_record_info_t;

typedef struct {
  double start;
  double end;
  double interval;
  double au;
  double emratio;
  uint64_t record_size;
  uint64_t has_nutations_and_librations;
  de_record_info_t record_info[13];
} de_bin_file_header_t;

typedef struct {
  double start;
  double end;
  double coeffs[];
} de_bin_record_t;


typedef struct {
  void *data;
  size_t len;
  int fd;
  
  de_bin_file_header_t *header;
  de_bin_record_t *record;
} de_bin_file_t;

int de_bin_open(const char *path, de_bin_file_t *file);
void de_bin_close(de_bin_file_t *file);
int de_compute_state_vectors(de_bin_file_t *file, double jde);

// TODO: Should not be public
size_t de_bin_num_records(de_bin_file_t *file);


#endif /* ! ORBIT_DE4XX_H */
