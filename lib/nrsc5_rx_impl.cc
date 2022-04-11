/* -*- c++ -*- */
/*
 * Copyright 2022 Albert Briscoe.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include <gnuradio/io_signature.h>
#include "nrsc5_rx_impl.h"

namespace gr {
  namespace nrsc5_rx {

    using input_type = gr_complex;
    using output_type = float;
    nrsc5_rx::sptr
    nrsc5_rx::make(bool test)
    {
      return gnuradio::make_block_sptr<nrsc5_rx_impl>(
        test);
    }


    /*
     * The private constructor
     */
    nrsc5_rx_impl::nrsc5_rx_impl(bool test)
      : gr::block("nrsc5_rx",
              gr::io_signature::make(1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
              gr::io_signature::make(2 /* min outputs */, 2 /*max outputs */, sizeof(output_type)))
    {
      _test = test;
    }

    /*
     * Our virtual destructor.
     */
    nrsc5_rx_impl::~nrsc5_rx_impl()
    {
    }

    void
    nrsc5_rx_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
    {
    #pragma message("implement a forecast that fills in how many items on each input you need to produce noutput_items and remove this warning")
      /* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
    }

    int
    nrsc5_rx_impl::general_work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      auto in = static_cast<const input_type*>(input_items[0]);
      auto out0 = static_cast<output_type*>(output_items[0]);
      auto out1 = static_cast<output_type*>(output_items[1]);

      #pragma message("Implement the signal processing in your block and remove this warning")
      // Do <+signal processing+>
      // Tell runtime system how many input items we consumed on
      // each input stream.
      consume_each (noutput_items);

      // Tell runtime system how many output items we produced.
      return noutput_items;
    }

  } /* namespace nrsc5_rx */
} /* namespace gr */
