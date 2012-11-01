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

#include <stddef.h>
#include <stdio.h>

#include "de4xx.h"

#define ITER 10000

int
main(int argc, char const *argv[])
{
  if (argc != 2) {
    printf("no file given\n");
    return 0;
  }

  de_bin_file_t file;
  if (!de_bin_open(argv[1], &file)) {
    de_bin_file_header_t *hdr = file.header;
    printf("range: %f %f\n", hdr->start, hdr->end);
    printf("interval: %f\n", hdr->interval);
    printf("au: %f\n", hdr->au);
    printf("emratio: %f\n", hdr->emratio);
    printf("record size: %d\n", (int)hdr->record_size);
    printf("nutations and liberations: %d\n", (int)hdr->has_nutations_and_librations);

    for (int i = 0 ; i < 13 ; i++) {
      printf("%2d: %3d %3d %3d\n", i,
             (int)hdr->record_info[i].offset,
             (int)hdr->record_info[i].coeffs_per_comp,
             (int)hdr->record_info[i].number_of_comp_sets);
    }

    size_t rec_count = de_bin_num_records(&file);
    printf("number of records: %d\n", (int)rec_count);
#if 0
    size_t rec_size = de_bin_record_size(&file);
    printf("record size: %d\n", (int)rec_size);

    struct timeval start, stop;

    gettimeofday(&start, NULL);
    //for (int i = 0 ; i < ITER ; i ++) {
    de_compute_state_vectors(&file, 625398.5 /*+ i*/);
    //}
    gettimeofday(&stop, NULL);

    double startf = ((double)start.tv_sec) + ((double)start.tv_usec) * 1e-6;
    double stopf = ((double)stop.tv_sec) + ((double)stop.tv_usec) * 1e-6;

    printf("total time: %f s\n", stopf-startf);
    printf("time per iter: %f ms\n", (stopf-startf)/ITER * 1e3);
#endif
    de_bin_close(&file);
  }
#if 0
  jplde_bin_file_t jfile;
  if (!jplde_bin_open(argv[1], &jfile)) {
    //for (int i = 0 ; i < 3 ; i++) {
    //  printf("label: ");
    //  for (int j = 0 ; j < 84 ; j++) {
    //    putchar(jfile.hdr1->label[i][j]);
    //  }
    //}
    printf("\nlabel: %d\n", (int)offsetof(jplde_bin_header1_t, label));
    printf("size: %d\n", (int)sizeof(jplde_bin_header1_t));
#endif
#if 0
    char name[400][6];
    double time_info[3];
    int32_t num_consts;
    double au;
    double emrat;
    int32_t coeff_offsets[12][3];
    int32_t denum;
    int32_t libration_offsets[3];
#endif
    //for (int i = 0 ; i < 400 ; i++) {
    //    printf("name: ");
    //    for (int j = 0 ; j < 6 ; j++) {
    //        putchar(jfile.hdr1->name[i][j]);
    //      }
    //      printf("\n");
    //    }
#if 0
    printf("time info: %f %f %f\n", jfile.hdr1->time_info[0], jfile.hdr1->time_info[1], jfile.hdr1->time_info[2]);
    printf("num consts: %d\n", jfile.hdr1->num_consts);
    printf("au: %f\n", jfile.hdr1->au);
    printf("emrat: %f\n", jfile.hdr1->emrat);
    for (int i = 0 ; i < 12 ; i ++) {
      printf("body %d: %d %d %d\n", i, jfile.hdr1->coeff_offsets[i][0],
             jfile.hdr1->coeff_offsets[i][1],
             jfile.hdr1->coeff_offsets[i][2]);
    }
    printf("libration offsets: %d %d %d\n", jfile.hdr1->libration_offsets[0],
           jfile.hdr1->libration_offsets[1],
           jfile.hdr1->libration_offsets[2]);

    printf("first hdr2 value: %f\n", jfile.hdr2->consts[0]);
    printf("interval of first granule: %f %f\n", jfile.coeffs[0], jfile.coeffs[1]);


    double target_low = 0.406000000000000000e+03 - 0.0005e+3;
    double target_high = 0.406000000000000000e+03 + 0.0005e+3;
    double *target = (double*)(jfile.hdr1 + 1);
    for (int i = 0 ; i < 2000 ; i ++) {
      if (target[i] >= target_low && target[i] <= target_high) {
        printf("406 found parameter at offset: %d (%d)\n", i, (int)(i*sizeof(double)));
        printf("\t%p\n", (void*)&target[i]);
      }
    }
    target_low = 0.422000000000000000e+03 - 0.0005e+3;
    target_high = 0.422000000000000000e+03 + 0.0005e+3;
    for (int i = 0 ; i < 2000 ; i ++) {
      if (target[i] >= target_low && target[i] <= target_high) {
        printf("422 found parameter at offset: %d (%d)\n", i, (int)(i*sizeof(double)));
        printf("\t%p\n", (void*)&target[i]);
      }
    }

    printf("hdr1: %p (%d)\n", jfile.hdr1, (int)sizeof(*jfile.hdr1));
    printf("hdr2: %p (%d)\n", jfile.hdr2, (int)sizeof(*jfile.hdr1));
    uintptr_t p = ((uintptr_t)jfile.hdr2 - ((uintptr_t)jfile.hdr1 + sizeof(jplde_bin_header1_t)));
    printf("hdr2-hdr1 end: %d %d\n", (int)p, (int)(p/sizeof(double)));
  }
#endif


  return 0;
}
