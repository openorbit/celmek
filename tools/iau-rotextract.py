#! /usr/bin/env python
# coding=utf-8

import sys

nameMap = {'δ0' : 'delta0', 'α0': 'alpha0', 'W' : "w"}
terms = {}

current = None
bodies = {}

def newBody(kind, name):
  body = {'kind': kind, 'name': name}
  body['α0_sin_amp'] = []
  body['α0_sin_arg'] = []
  body['δ0_sin_amp'] = []
  body['δ0_sin_arg'] = []
  body['W_sin_amp'] = []
  body['W_sin_arg'] = []
  body['α0_cos_amp'] = []
  body['α0_cos_arg'] = []
  body['δ0_cos_amp'] = []
  body['δ0_cos_arg'] = []
  body['W_cos_amp'] = []
  body['W_cos_arg'] = []
  body['α0_T'] = []
  body['α0_d'] = []
  body['α0_T^2'] = []
  body['α0_d^2'] = []
  body['δ0_T'] = []
  body['δ0_d'] = []
  body['δ0_T^2'] = []
  body['δ0_d^2'] = []
  body['W_T'] = []
  body['W_d'] = []
  body['W_T^2'] = []
  body['W_d^2'] = []
  body['W'] = []
  body['δ0'] = []
  body['α0'] = []
  body['δ0_const'] = None
  body['α0_const'] = None
  body['W_const'] = None
  return body


# Firs read the all the data from the file
for line in file(sys.argv[1]):
  # First skip comments and white space
  if line.startswith('#'):
    continue
  if line.replace(" ", "") == "\n":
    continue

  items = line.split()
  
  if line.startswith('STAR'):
    name = items[1]
    kind = items[0]
    current = newBody(kind, name)
    bodies[name] = current
  elif line.startswith('PLANET'):
    name = items[1]
    kind = items[0]
    current = newBody(kind, name)
    bodies[name] = current
  elif line.startswith('MOON'):
    name = items[1]
    kind = items[0]
    parent = items[2]
    current = newBody(kind, name)
    current['parent'] = parent
    bodies[name] = current
  elif line.startswith('DWARF'):
    name = items[1]
    kind = items[0]
    current = newBody(kind, name)
    bodies[name] = current
  elif line.startswith('TERM'):
    terms[items[1]] = items[3:]
  else:
    if items[0] not in nameMap:
      print "error reading file, symbol %s is not known" % (items[0])
    else:
      current[items[0]] = items[2:]
      current['%s_const' % (items[0])] = items[2]
      for i in range(0, len(items)):
        if items[i] == 'sin':
          current["%s_sin_amp" % (items[0])] += ["%s%s" % (items[i-2], items[i-1])]
          current["%s_sin_arg" % (items[0])] += [items[i+1]]
        elif items[i] == 'cos':
          current["%s_cos_amp" % (items[0])] += ["%s%s" % (items[i-2], items[i-1])]
          current["%s_cos_arg" % (items[0])] += [items[i+1]]
        elif items[i].endswith('T'):
          current["%s_T" % (items[0])] += ["%s%s" % (items[i-1],items[i][0:-1])]
        elif items[i].endswith('d'):
          current["%s_d" % (items[0])] += ["%s%s" % (items[i-1],items[i][0:-1])]
        elif items[i].endswith('T^2'):
          current["%s_T^2" % (items[0])] += ["%s%s" % (items[i-1],items[i][0:-3])]
        elif items[i].endswith('d^2'):
          current["%s_d^2" % (items[0])] += ["%s%s" % (items[i-1],items[i][0:-3])]
      
# Generate term enum
def termID(key):
  return "IAU_TERM_%s" % (key.upper())

def bodyID(key):
  return "IAU_BODY_%s" % (key.upper())

def sineID(key):
  return "IAU_SIN_%s" % (key.upper())

def cosineID(key):
  return "IAU_COS_%s" % (key.upper())

print "typedef enum {"
for key in sorted(terms.keys()):
  term = terms[key]
  print "  %s," % (termID(key))
print "  IAU_TERM_COUNT"
print "} iau_term_id_t;"
print ""
print "typedef struct {"
print "  iau_term_id_t term;"
print "  double a, b;"
print "} iau_term_t;"
print ""
print "iau_term_t d_terms[] = {"
for key in terms:
  term = terms[key]
  if term[2].endswith('d'):
    print "  {%s, %s, %s%s}," % (termID(key), term[0], term[1], term[2][0:-1])
print "};"

print ""

print "iau_term_t T_terms[] = {"
for key in terms:
  term = terms[key]
  if term[2].endswith('T'):
    print "  {%s, %s, %s%s}," % (termID(key), term[0], term[1], term[2][0:-1])
print "};"
print ""

print "typedef struct {"
print "  iau_term_id_t term;"
print "  double a;"
print "} iau_term2_t;"
print ""

print "iau_term2_t T2_terms[] = {"
for key in terms:
  term = terms[key]
  for i in range(0, len(term)):
    if term[i].endswith('T^2'):
      print "  {%s, %s%s}," % (termID(key), term[i-1], term[i][0:-3])
print "};"
print ""

print "typedef enum {"
for key in sorted(bodies.keys()):
  print "  %s," % (bodyID(key))
print "  IAU_BODY_COUNT"
print "} iau_body_id_t;"
print ""

# Get all sine and cosine argument and unique them
sine_args = set()
cosine_args = set()

for key in bodies:
  body = bodies[key]
  
  for item in body['α0_sin_arg']:
    sine_args.add(item)
  for item in body['δ0_sin_arg']:
    sine_args.add(item)
  for item in body['W_sin_arg']:
    sine_args.add(item)
    
  for item in body['α0_cos_arg']:
    cosine_args.add(item)
  for item in body['δ0_cos_arg']:
    cosine_args.add(item)
  for item in body['W_cos_arg']:
    cosine_args.add(item)
  
print "typedef enum {"
for arg in sine_args:
  print "  %s," % (sineID(arg))
print "  IAU_SIN_COUNT"
print "} iau_sine_id_t;"
print ""

print "typedef enum {"
for arg in cosine_args:
  print "  %s," % (cosineID(arg))
print "  IAU_COS_COUNT"
print "} iau_cosine_id_t;"
print ""


print "typedef struct {"
print "  double m;"
print "  iau_term_id_t term;"
print "} iau_arg_rule_t;"
print ""

# The Sine and cosine args are written as [A]B, where A is the multiplier and
# B the term name, this loop splits the arg into multiplier and term name
print "iau_arg_rule_t sine_rules[] = {"
for arg in sine_args:
  mult = 1
  term = termID(arg)
  if arg[0].isdigit():
    mult = int(arg[0])
    term = termID(arg[1:])
  
  print "  [%s] = {%d, %s}," % (sineID(arg), mult, term)
print "};"
print ""

print "iau_arg_rule_t cosine_rules[] = {"
for arg in cosine_args:
  mult = 1
  term = termID(arg)
  if arg[0].isdigit():
    mult = int(arg[0])
    term = termID(arg[1:])
  
  print "  [%s] = {%d, %s}," % (cosineID(arg), mult, term)
print "};"
print ""


print "typedef struct {"
print "  double a;"
print "  iau_sine_id_t sine;"
print "} iau_sine_term_t;"
print ""

print "typedef struct {"
print "  double a;"
print "  iau_cosine_id_t cosine;"
print "} iau_cosine_term_t;"
print ""

def hasAlphaSines(body):
  return len(body['α0_sin_amp']) > 0

def hasAlphaCosines(body):
  return len(body['α0_cos_amp']) > 0

def hasDeltaSines(body):
  return len(body['δ0_sin_amp']) > 0

def hasDeltaCosines(body):
  return len(body['δ0_cos_amp']) > 0

def hasWSines(body):
  return len(body['W_sin_amp']) > 0
  
def hasWCosines(body):
  return len(body['W_cos_amp']) > 0


for key in bodies:
  body = bodies[key]      
  
  if hasAlphaSines(body):
    print "static iau_sine_term_t %s_alpha_sines[] = {" % (body['name'].lower())
    for i in range(0, len(body['α0_sin_amp'])):
      amp = body['α0_sin_amp'][i]
      arg = body['α0_sin_arg'][i]
      print "  {%s, %s}," % (amp,sineID(arg))
    print "};"
    print ""
  
  if hasDeltaCosines(body):
    print "static iau_cosine_term_t %s_delta_cosines[] = {" % (body['name'].lower())
    for i in range(0, len(body['δ0_cos_amp'])):
      amp = body['δ0_cos_amp'][i]
      arg = body['δ0_cos_arg'][i]
      print "  {%s, %s}," % (amp,cosineID(arg))
    print "};"
    print ""
    
  if hasWSines(body):
    print "static iau_sine_term_t %s_w_sines[] = {" % (body['name'].lower())
    for i in range(0, len(body['W_sin_amp'])):
      amp = body['W_sin_amp'][i]
      arg = body['W_sin_arg'][i]
      print "  {%s, %s}," % (amp,sineID(arg))
    print "};"
    print ""
    
  if hasWCosines(body):
    print "static iau_cosine_term_t %s_w_cosines[] = {" % (body['name'].lower())
    for i in range(0, len(body['W_sin_amp'])):
      amp = body['W_cos_amp'][i]
      arg = body['W_cos_arg'][i]
      print "  {%s, %s}," % (amp,cosineID(arg))
    print "};"
    print ""

  if hasAlphaCosines(body):
    print "#warning \"alpha cosine exists for %s\"" % (body['name'])
  if hasDeltaSines(body):
    print "#warning \"delta sine exists for %s\"" % (body['name'])
  


# GENERATE BODY DATA HERE
print "typedef struct {"
print "  const char *name;"
print "  double a0;"
print "  double aT;"
print "  double d0;"
print "  double dT;"
print "  double w0;"
print "  double wd;"
print "  double wd_2;"
print "  double wT_2;"

print "  iau_sine_term_t *alpha_sines;"
print "  iau_cosine_term_t *delta_cosines;"
print "  iau_sine_term_t *w_sines;"
print "  iau_cosine_term_t *w_cosines;"
print "  size_t asin_count;"
print "  size_t dcos_count;"
print "  size_t wsin_count;"
print "  size_t wcos_count;"
print "} iau_body_t;"
print ""

def getA0(body):
  return body['α0_const']
def getAT(body):
  terms = body['α0_T']
  if len(terms) == 0:
    return "0.0"
  if len(terms) > 1:
    print "#warning \"too many aT\""
  return terms[0]

def getD0(body):
  return body['δ0_const']
def getDT(body):
  terms = body['δ0_T']
  if len(terms) == 0:
    return "0.0"
  if len(terms) > 1:
    print "#warning \"too many dT\""
  return terms[0]

def getW0(body):
  return body['W_const']
def getWd(body):
  terms = body['W_d']
  if len(terms) == 0:
    return "0.0"
  if len(terms) > 1:
    print "#warning \"too many Wd\""
  return terms[0]
def getWd2(body):
  terms = body['W_d^2']
  if len(terms) == 0:
    return "0.0"
  if len(terms) > 1:
    print "#warning \"too many Wd^2\""
  return terms[0]
def getWT2(body):
  terms = body['W_d^2']
  if len(terms) == 0:
    return "0.0"
  if len(terms) > 1:
    print "#warning \"too many WT^2\""
  return terms[0]

  
# Note, important that this is sorted
print "iau_body_t bodies[IAU_BODY_COUNT] = {"
for key in sorted(bodies.keys()):
  body = bodies[key]
  asines = "NULL"
  dcosines = "NULL"
  wsines = "NULL"
  wcosines = "NULL"
  asine_len = "0"
  dcosine_len = "0"
  wsine_len = "0"
  wcosine_len = "0"
  
  if hasAlphaSines(body):
    asines = "%s_alpha_sines" % (body['name'].lower())
    asine_len = "sizeof(%s_alpha_sines)/sizeof(iau_sine_term_t)" % (body['name'].lower())
  if hasDeltaCosines(body):
    dcosines = "%s_delta_cosines" % (body['name'].lower())
    dcosine_len = "sizeof(%s_delta_cosines)/sizeof(iau_cosine_term_t)" % (body['name'].lower())
  if hasWSines(body):
    wsines = "%s_w_sines" % (body['name'].lower())
    wsine_len = "sizeof(%s_w_sines)/sizeof(iau_sine_term_t)" % (body['name'].lower())
  if hasWCosines(body):
    wcosines = "%s_w_cosines" % (body['name'].lower())
    wcosine_len = "sizeof(%s_w_cosines)/sizeof(iau_cosine_term_t)" % (body['name'].lower())
  
  print "  {\"%s\", %s, %s, %s, %s, %s, %s, %s, %s" % (body['name'].lower(), getA0(body), getAT(body), getD0(body), getDT(body), getW0(body), getWd(body), getWd2(body), getWT2(body)),
  print ", %s, %s, %s, %s, %s, %s, %s, %s}," % (asines, dcosines, wsines, wcosines, asine_len, dcosine_len, wsine_len, wcosine_len)
print "};"
print ""

for key in bodies:
  body = bodies[key]
  if len(body['δ0_sin_amp']) > 0 or len(body['δ0_sin_arg']) > 0:
    print '#warning "delta0 sin %s"' % (body['name'])
  if len(body['α0_cos_amp']) > 0 or len(body['α0_cos_arg']) > 0:
    print '#warning "alpha0 cos %s"' % (body['name'])
  if len(body['α0_d']) > 0:
    print '#warning "alpha0 d %s"' % (body['name'])
  if len(body['α0_T^2']) > 0:
    print '#warning "alpha0 T^2 %s"' % (body['name'])
  if len(body['α0_d^2']) > 0:
    print '#warning "alpha0 d^2 %s"' % (body['name'])
  if len(body['δ0_d']) > 0:
    print '#warning "delta0 d %s"' % (body['name'])
  if len(body['δ0_T^2']) > 0:
    print '#warning "delta0 T^2 %s"' % (body['name'])
  if len(body['δ0_d^2']) > 0:
    print '#warning "delta0 d^2 %s"' % (body['name'])
  if len(body['W_T']) > 0:
    print '#warning "WT %s"' % (body['name'])
