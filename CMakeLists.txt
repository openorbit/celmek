cmake_minimum_required(VERSION 2.8)
project(celmek)
set(CELMEK_SRC  "src/coordinates.c"
                "src/de4xx.c"
                "src/elp2000-82b.c"
                "src/goffin-j2000.c"
                "src/iau-rotation.c"
                "src/kepler.c"
                "src/lieske-e2x3.c"
                "src/mean-orbits.c"
                "src/models.c"
                "src/mpc-orbit-reader.c"
                "src/orbital-elements.c"
                "src/time-formats.c"
                "src/vsop87.c")

set(CELMEK_HDR "include/celmek/celmek.h"
               "include/celmek/cm-math.h"
               "include/celmek/cm-time.h"
               "include/celmek/cm-model.h"
               "include/celmek/cm-coordinates.h"
               "include/celmek/cm-astro.h")

# We are building a library, we make the include here a sys include dir
# in order to let tools and code build with '<>' includes.
include_directories(BEFORE "src")
include_directories(BEFORE SYSTEM "include")

add_library(celmek STATIC ${CELMEK_SRC})

add_executable(deconvert "tools/de4xx-convert.c")
target_link_libraries(deconvert celmek)

add_executable(desim "tools/de4xx-sim.c")
target_link_libraries(desim celmek)

add_executable(cmsim "tools/orbital-sim.c")
target_link_libraries(cmsim celmek)

install(TARGETS celmek
        ARCHIVE DESTINATION "lib/")

install(FILES ${CELMEK_HDR}
        DESTINATION "include/celmek/")




# Optionally add documentation target if we have Doxygen available
find_package(Doxygen)

if (DOXYGEN_FOUND)
  # Try to locate dot, if available we use it to generate call graphs
  find_program(DOT_EXECUTABLE dot)
  if (DOT_EXECUTABLE)
    set(DOT_FOUND "YES")
    set(DOT_PATH "${DOT_EXECUTABLE}")
  else (DOT_EXECUTABLE)
    set(DOT_FOUND "NO")
    set(DOT_PATH "")
  endif (DOT_EXECUTABLE)

  # Configure the doxygen file
  set(DOXYGEN_CONFIG "${CMAKE_BINARY_DIR}/Doxyfile")

  configure_file( "${CMAKE_SOURCE_DIR}/Doxyfile.in"
    "${DOXYGEN_CONFIG}"
    @ONLY)
  add_custom_target(docs ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CONFIG}
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR})
endif (DOXYGEN_FOUND)
