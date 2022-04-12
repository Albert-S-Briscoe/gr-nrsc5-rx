/* -*- c++ -*- */
/*
 * Copyright 2022 Albert Briscoe.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

extern "C" {
#include "nrsc5.h"
}

#include "nrsc5_rx_impl.h"

#include <gnuradio/io_signature.h>
#include <stdexcept>

namespace gr {
  namespace nrsc5_rx {

//    using input_type = int16_t;
//    using output_type = float;
    nrsc5_rx::sptr
    nrsc5_rx::make(bool test)
    {
      return gnuradio::make_block_sptr<nrsc5_rx_impl>(test);
    }


    /*
     * The private constructor
     */
    nrsc5_rx_impl::nrsc5_rx_impl(bool test)
      : gr::block("nrsc5_rx",
              gr::io_signature::make(1 /* min inputs */, 1 /* max inputs */, sizeof(int16_t)),
              gr::io_signature::make(0 /* min outputs */, 0 /*max outputs */, sizeof(float)))
    {
      _test = test;

      // check nrsc5 version?

      if (nrsc5_open_pipe(&nrsc5) != 0)
        throw std::runtime_error("nrsc5_rx_impl: nrsc5_open_pipe failed");

      // other vars?
    }

    /*
     * Our virtual destructor.
     */
    nrsc5_rx_impl::~nrsc5_rx_impl() {
      nrsc5_stop(nrsc5);
      nrsc5_close(nrsc5);
    }


//    void
//    nrsc5_rx_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required) {
//      #pragma message("implement a forecast that fills in how many items on each input you need to produce noutput_items and remove this warning")
//      /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
//    }

    int
    nrsc5_rx_impl::general_work (int noutput_items,
                       int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      //auto in = static_cast<const input_type*>(input_items[0]);
      const int16_t* in = static_cast<const int16_t*>(input_items[0]);

      // disabled until I have a callback function
//      auto out0 = static_cast<output_type*>(output_items[0]);
//      auto out1 = static_cast<output_type*>(output_items[1]);

      //#pragma message("Implement the signal processing in your block and remove this warning")
      // Do <+signal processing+>
      // Tell runtime system how many input items we consumed on
      // each input stream.
      nrsc5_pipe_samples_cs16(nrsc5, in, ninput_items);

      //consume_each (noutput_items);

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace nrsc5_rx */
} /* namespace gr */
