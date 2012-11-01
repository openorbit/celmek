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

#include "de4xx.h"


#define MAX_LINE_LENGTH 100
int
de_ascii_next_group(FILE *f)
{
  char buff[MAX_LINE_LENGTH+1];
  memset(buff, 0, sizeof(buff));
  int group;
  while (!feof(f)) {
    if (fgets(buff, sizeof(buff)-1, f)) {
      if (sscanf(buff, "GROUP   %d\n", &group) == 1) {
        return group;
      }
    }
  }
  return -1;
}

int
de_ascii_1030_header(FILE *f, double *s, double *e, double *i)
{
  double start, end, interval;

  if (fscanf(f, "%lf %lf %lf\n", &start, &end, &interval) == 3) {
    *s = start;
    *e = end;
    *i = interval;
    return 0;
  }

  return -1;
}

int
de_ascii_1050_header(FILE *f, int offs[13], int cooef[13], int sets[13])
{
  if (fscanf(f, "%d %d %d %d %d %d %d %d %d %d %d %d %d\n",
             offs, offs+1, offs+2, offs+3, offs+4, offs+5, offs+6, offs+7,
             offs+8, offs+9, offs+10, offs+11, offs+12) != 13) {
    return -1;
  }
  
  if (fscanf(f, "%d %d %d %d %d %d %d %d %d %d %d %d %d\n",
             cooef, cooef+1, cooef+2, cooef+3, cooef+4, cooef+5, cooef+6,
             cooef+7, cooef+8, cooef+9, cooef+10, cooef+11, cooef+12) != 13) {
    return -1;
  }
  
  if (fscanf(f, "%d %d %d %d %d %d %d %d %d %d %d %d %d\n",
             sets, sets+1, sets+2, sets+3, sets+4, sets+5, sets+6, sets+7,
             sets+8, sets+9, sets+10, sets+11, sets+12) != 13) {
    return -1;
  }

  return 0;
}

double
de_ascii_get_fortran_double(FILE *f)
{
  char reals[MAX_LINE_LENGTH+1];
  
  if (fscanf(f, "%100s", reals) == 1) {
    //printf("GFD: '%s'\n", reals);
    char *Dptr = strchr(reals, 'D');
    if (Dptr) {
      *Dptr = 'e';
    }

    double val;
    if (sscanf(reals, "%lg", &val) == 1) {
      return val;
    }
  }

  return NAN;
}

int
de_ascii_get_1070_sub_header(FILE *f, int *rec_id, int *count)
{
  char buff[MAX_LINE_LENGTH+1];
  
  while (!feof(f)) {
    if (fgets(buff, sizeof(buff)-1, f)) {
      //printf("BUFF '%s'\n", buff);
      
      int r, c;
      if (sscanf(buff, "%d %d\n", &r, &c) == 2) {
        *rec_id = r;
        *count = c;
        return 0;
      }
    }
  }
  return -1;
}

typedef struct {
  int record_id;
  int record_len;
  double start;
  double end;
  size_t coef_len;
  double *coefs;
} de_ascii_file_record_t;

short au_index = -1;
short emrat_index = -1;

int
de_ascii_1040_fields(FILE *f)
{
  char buff[101];
  
  short param_count = 0;
  if (fscanf(f, "%hd", &param_count) == 1) {
    for (short i = 0 ; i < param_count ; i ++) {
      if (fscanf(f, "%100s", buff) == 1) {
        if (!strcmp(buff, "EMRAT")) {
          emrat_index = i;
        } else if (!strcmp(buff, "AU")) {
          au_index = i;
        }
      }

      if (au_index != -1 && emrat_index != -1) {
        return 0;
      }
    }
  }
  return -1;
}

int
de_ascii_1041_variables(FILE *f, double *au, double *gmrat)
{
  short param_count = 0;
  
  if (fscanf(f, "%hd", &param_count) == 1) {
    for (short i = 0 ; i < param_count ; i ++) {
      double value = de_ascii_get_fortran_double(f);

      if (i == au_index) {
        *au = value;
      } else if (i == emrat_index) {
        *gmrat = value;
      }
    }
    return 0;
  }
  return -1;
}


int
de_ascii_1070_record(FILE *f, de_ascii_file_record_t *record)
{
  int record_id, count;
  
  if (de_ascii_get_1070_sub_header(f, &record_id, &count)) {
    return -1;
  }

  if (count > record->coef_len) {
    return -1; // Missformed record, it should not exceed the length computed
               // from the header
  }

  record->record_id = record_id;
  record->record_len = count - 2; // - for first two doubles

  record->start = de_ascii_get_fortran_double(f);
  record->end = de_ascii_get_fortran_double(f);
  
  if (isnan(record->start) || isnan(record->end)) {
    return -1;
  }
  
  // Subtract 2 for start and end time already read
  //printf("count before read: %d\n", count);
  for (int i = 0 ; i < count - 2 ; i ++) {
    record->coefs[i] = de_ascii_get_fortran_double(f);
    //printf("read: %3d %f\n", i, record->coefs[i]);
    if (isnan(record->coefs[i])) return -1;
    //printf("cooef: %f\n", cooef);
  }
  
  //printf("Record id = %d, size = %d\n", record_id, count);
  return 0;
}

int
de_ascii_1070_data(FILE *f, FILE *out, size_t coeff_count)
{
  de_ascii_file_record_t record;
  memset(&record, 0, sizeof(record));
  
  record.coef_len = coeff_count;
  record.coefs = calloc(coeff_count, sizeof(double));
  
  printf("reading 1070 data\n");

  while (!de_ascii_1070_record(f, &record)) {
    // So we have a record with coefficients, we now have to split these up
    // based on the layout of the coefs
    //printf("ascii record %d: %f %f [%d]\n", record.record_id,
    //       record.start, record.end, record.record_len);
    
    de_bin_record_t bin_record;
    bin_record.start = record.start; 
    bin_record.end = record.end;
    
    fwrite(&bin_record, sizeof(bin_record), 1, out);
    fwrite(record.coefs, sizeof(double), record.record_len, out);
  }
  
  free(record.coefs);
  return 0;
}

int
de_ascii_read_first_line(FILE *f, int *record_size)
{
  char buff[MAX_LINE_LENGTH+1];
  if (fgets(buff, sizeof(buff)-1, f)) {
    int kz, nc;
    if (sscanf(buff, "KSIZE=  %d    NCOEFF=  %d\n", &kz, &nc) == 2) {
      *record_size = nc;
      return 0;
    }
  }
  return -1;
}

int
de_ascii_convert(const char *path, const char *output)
{
  FILE *f = fopen(path, "r");
  if (!f) {
    return -1; 
  }
  FILE *out = fopen(output, "wb");
  if (!out) {
    fclose(f);
    return -1;
  }


  double start = NAN, end = NAN, interval = NAN, au = NAN, emrat = NAN;
  int offsets[13], coeffs[13], sets[13];
  int record_size = 0;
  de_bin_file_header_t hdr;
  memset(&hdr, 0, sizeof(hdr));

  if (de_ascii_read_first_line(f, &record_size)) {
    printf("error: no record count on first line\n");
    return -1;
  }

  while (!feof(f)) {
    int group = de_ascii_next_group(f);
  
    switch (group) {
    case 1030:
      if (de_ascii_1030_header(f, &start, &end, &interval)) {
        printf("error: reading 1030 header\n");
        fclose(f);
        fclose(out);
        return -1;
      } else {
        hdr.start = start;
        hdr.end = end;
        hdr.interval = interval;
      }
      break;
    case 1040:
      if (de_ascii_1040_fields(f)) {
        printf("error: reading 1040 header\n");
        fclose(f);
        fclose(out);
      }
      break;
    case 1041:
      if (de_ascii_1041_variables(f, &au, &emrat)) {
        printf("error: reading 1041 header\n");
        fclose(f);
        fclose(out);        
      } else {
        hdr.au = au;
        hdr.emratio = emrat;
      }
      break;
    case 1050:
      if (de_ascii_1050_header(f, offsets, coeffs, sets)) {
      // Error
        printf("error: reading 1050 header\n");
        fclose(f);
        fclose(out);
        return -1;
      } else {
        for (int i = 0 ; i < 13 ; i ++) {
          // Fortran data starts with 1, subtract 1 to make the offset normal
          hdr.record_info[i].offset = offsets[i] - 1;
          hdr.record_info[i].coeffs_per_comp = coeffs[i];
          hdr.record_info[i].number_of_comp_sets = sets[i];
        }
        if (coeffs[11] != 0 && coeffs[12] != 0) {
          hdr.has_nutations_and_librations = 1;
        } else if (coeffs[11] == 0 && coeffs[12] == 0) {
          hdr.has_nutations_and_librations = 0;
        } else {
          printf("there are either nutations or librations, but not both in the file\n");
          return -1;
        }
        // Subtract 1, these indices start with 1
        hdr.record_size = record_size;
      }
      break;
    case 1070:
      // Header must be complete
      if (isnan(start)) { printf("start not in header\n"); return -1; }
      if (isnan(end)) { printf("end not in header\n"); return -1; }
      if (isnan(interval)) { printf("interval not in header\n"); return -1; }
      if (isnan(au)) { printf("au not in header\n"); return -1; }
      if (isnan(emrat)) { printf("emrat not in header\n"); return -1; }

      printf("Read a valid header:\n");
      printf("\tstart = %f\n", start);
      printf("\tend = %f\n", end);
      printf("\tinterval = %f\n", interval);
      printf("\temrat = %f\n", emrat);
      printf("\tau = %f\n", au);
      printf("\trecord size = %d\n", record_size);
      printf("Will proceed with the coefficients, this will take some time\n");
      fwrite(&hdr, sizeof(hdr), 1, out);
      de_ascii_1070_data(f, out, record_size);
      break;
    default:
      break;
    }
  }

  printf("Range %f - %f\n", start, end);
  for (int i = 0 ; i < 13 ; i++) {
    printf("%2d: %3d %3d %3d\n", i, offsets[i], coeffs[i], sets[i]);
  }
  
  fclose(f);
  fclose(out);
  return 0;
}


int
main(int argc, char const *argv[])
{
  if (argc != 3) {
    printf("no file given\n");
    return 0;
  }

  de_ascii_convert(argv[1], argv[2]);
  
  return 0;
}