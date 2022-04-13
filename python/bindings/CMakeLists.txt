# Copyright 2020 Free Software Foundation, Inc.
#
# This file is part of GNU Radio
#
# SPDX-License-Identifier: GPL-3.0-or-later
#

########################################################################
# Check if there is C++ code at all
########################################################################
if(NOT nrsc5_rx_sources)
    MESSAGE(STATUS "No C++ sources... skipping python bindings")
    return()
endif(NOT nrsc5_rx_sources)

########################################################################
# Check for pygccxml
########################################################################
GR_PYTHON_CHECK_MODULE_RAW(
    "pygccxml"
    "import pygccxml"
    PYGCCXML_FOUND
    )

include(GrPybind)

########################################################################
# Python Bindings
########################################################################
list(APPEND nrsc5_rx_python_files
    nrsc5_rx_python.cc
    python_bindings.cc
)

GR_PYBIND_MAKE_OOT(nrsc5_rx
   ../..
   gr::nrsc5_rx
   "${nrsc5_rx_python_files}")

# copy in bindings .so file for use in QA test module
add_custom_target(
  copy_bindings_for_tests ALL
  COMMAND
    ${CMAKE_COMMAND} -E copy "${CMAKE_CURRENT_BINARY_DIR}/*.so"
    ${CMAKE_BINARY_DIR}/test_modules/nrsc5_rx/
  DEPENDS nrsc5_rx_python)

install(TARGETS nrsc5_rx_python DESTINATION ${GR_PYTHON_DIR}/nrsc5_rx COMPONENT pythonapi)
