/*
 * Copyright 2022 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 *
 */

/***********************************************************************************/
/* This file is automatically generated using bindtool and can be manually edited  */
/* The following lines can be configured to regenerate this file during cmake      */
/* If manual edits are made, the following tags should be modified accordingly.    */
/* BINDTOOL_GEN_AUTOMATIC(0)                                                       */
/* BINDTOOL_USE_PYGCCXML(0)                                                        */
/* BINDTOOL_HEADER_FILE(nrsc5_rx.h)                                        */
/* BINDTOOL_HEADER_FILE_HASH(9d650b54f5964724e860e7f59bff8d3e)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <nrsc5_rx/nrsc5_rx.h>
// pydoc.h is automatically generated in the build directory
#include <nrsc5_rx_pydoc.h>

void bind_nrsc5_rx(py::module& m)
{

    using nrsc5_rx    = ::gr::nrsc5_rx::nrsc5_rx;


    py::class_<nrsc5_rx, gr::block, gr::basic_block,
        std::shared_ptr<nrsc5_rx>>(m, "nrsc5_rx", D(nrsc5_rx))

        .def(py::init(&nrsc5_rx::make),
           py::arg("program") = 0,
           py::arg("test") = true,
           D(nrsc5_rx,make)
        )
        



        ;




}








