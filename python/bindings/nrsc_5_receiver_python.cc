/*
 * Copyright 2023 Free Software Foundation, Inc.
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
/* BINDTOOL_HEADER_FILE(nrsc_5_receiver.h)                                        */
/* BINDTOOL_HEADER_FILE_HASH(d8cd9ba38e63aedf50ea88a09ef0bc88)                     */
/***********************************************************************************/

#include <pybind11/complex.h>
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>

namespace py = pybind11;

#include <nrsc5_rx/nrsc_5_receiver.h>
// pydoc.h is automatically generated in the build directory
#include <nrsc_5_receiver_pydoc.h>

void bind_nrsc_5_receiver(py::module& m)
{

    using nrsc_5_receiver = ::gr::nrsc5_rx::nrsc_5_receiver;


    py::class_<nrsc_5_receiver, gr::hier_block2, std::shared_ptr<nrsc_5_receiver>>(
        m, "nrsc_5_receiver", D(nrsc_5_receiver))

        .def(py::init(&nrsc_5_receiver::make),
             py::arg("program") = 0,
             py::arg("samp_rate") = 1200000,
             py::arg("audio_rate") = 44100,
             D(nrsc_5_receiver, make))


        .def("set_program",
             &nrsc_5_receiver::set_program,
             py::arg("program"),
             D(nrsc_5_receiver, set_program))

        ;
}
