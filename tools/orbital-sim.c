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

#include <celmek/celmek.h>

#include <stdlib.h>
#include <stdio.h>
#include <sys/time.h>

#define ITER 10000

int
main(int argc, const char *argv[argc])
{
  cm_init();
  
  struct timeval start, stop;

  gettimeofday(&start, NULL);
  
  for (int i = 0 ; i < ITER ; i ++) {
    cm_orbit_compute(2448724.5);
  }
  gettimeofday(&stop, NULL);

  double startf = ((double)start.tv_sec) + ((double)start.tv_usec) * 1e-6;
  double stopf = ((double)stop.tv_sec) + ((double)stop.tv_usec) * 1e-6;
  
  printf("total time: %f s\n", stopf-startf);
  printf("time per iter: %f ms\n", (stopf-startf)/ITER * 1e3);
  return 0;
}