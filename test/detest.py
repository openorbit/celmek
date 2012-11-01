#! /usr/bin/env python

import sys
import subprocess


decomp = sys.argv[1] # Test program
defile = sys.argv[2] # Digital Ephemeris File (given as argument to test
                     # program)
detestfile = sys.argv[3] # The test file (from JPL)

// Target names
targetNames = ['mercury', 'venus', 'earth', 'mars',
               'jupiter', 'saturn', 'uranus', 'neptune',
               'pluto', 'moon', 'sun',
               'ssb', 'emb', 'nutations', 'librations']
coordNames = ['x', 'y', 'z', 'xdot', 'ydot', 'zdot']

precision = 1e-13

# We do this the ugly way
headerRead = False
for line in file(detestfile):
  if headerRead:
    (eph, caldate, jed, target, center, coord, value) = line.split()
    output = subprocess.check_output([decomp, defile, jed])
    # Parse output
    results = {}
    for o in output:
      so = o.split()
      results[so[0]] = so[1:]
    
    # Compare results (NOTE: subtract one as fortran indices starts with 1)
    tgt = results[targetNames[int(target)-1]]
    cent = results[targetNames[int(center)-1]]
  else:
    if line.startswith("EOT"):
      headerRead = True
