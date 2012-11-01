#! /usr/bin/env python

import sys

f = open(sys.argv[1], 'r')

first = True
variable = 0
power = 0
terms = 0
body = ""
variable_powers = [0,0,0]

def varToStr(v):
  if v == 1:
    return "x"
  elif v == 2:
    return "y"
  elif v == 3:
    return "z"

for line in f:
  if line.startswith(" VSOP87"):
    comps = line.split()
    tn = comps[7]
    power = int(tn[-1])
    variable = int(comps[5])
    variable_powers[variable-1] = int(power)
    terms = 0
    body = comps[3].lower()
    if not first:
      print "};"
    first = False
    print "static vsop87_term_t %s_%s_%d[] = {" % (body,varToStr(variable), power)
  else:
    comps = line.split()
    a = comps[-3]
    b = comps[-2]
    c = comps[-1]
    if float(a) > 0.0:
      terms = terms + 1
      print "  {%s, %s, %s}," % (a, b, c)

print "};"

# Assuming one body per file
print "static vsop87_body_t %s = {" % (body)
print "  .size_x = {sizeof(%s_x_0)/sizeof(vsop87_term_t)," % (body)
for i in range(1, variable_powers[0]+1):
  print "             sizeof(%s_x_%d)/sizeof(vsop87_term_t)," % (body, i)
print "  },"
print "  .size_y = {sizeof(%s_y_0)/sizeof(vsop87_term_t)," % (body)
for i in range(1, variable_powers[1]+1):
  print "             sizeof(%s_y_%d)/sizeof(vsop87_term_t)," % (body, i)
print "  },"
print "  .size_z = {sizeof(%s_z_0)/sizeof(vsop87_term_t)," % (body)
for i in range(1, variable_powers[2]+1):
  print "             sizeof(%s_z_%d)/sizeof(vsop87_term_t)," % (body, i)
print "  },"
terms_x = "  .terms_x = {"
for i in range(0, variable_powers[0]):
  terms_x += "%s_x_%d, " % (body, i)
terms_x += "%s_x_%d}," % (body, variable_powers[0])

terms_y = "  .terms_y = {"
for i in range(0, variable_powers[1]):
  terms_y += "%s_y_%d, " % (body, i)
terms_y += "%s_y_%d}," % (body, variable_powers[1])

terms_z = "  .terms_z = {"
for i in range(0, variable_powers[2]):
  terms_z += "%s_z_%d, " % (body, i)
terms_z += "%s_z_%d}," % (body, variable_powers[2])

print terms_x
print terms_y
print terms_z
print "};"
