/* -*- c++ -*- */
/*
 * Copyright 2022 Albert Briscoe.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "nrsc5_rx_impl.h"

#include <gnuradio/io_signature.h>
#include <stdexcept>

namespace gr {
	namespace nrsc5_rx {

//		using input_type = int16_t;
//		using output_type = float;
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
				gr::io_signature::make(1 /* min inputs */, 1 /* max inputs */, 2 * sizeof(int16_t)),
				gr::io_signature::make(1 /* min outputs */, 1 /*max outputs */, 2 * sizeof(int16_t)))
				// above is temporary
		{
			_test = test;

			if (nrsc5_open_pipe(&nrsc5) != 0)
				throw std::runtime_error("nrsc5_rx_impl: nrsc5_open_pipe failed");

			// other vars?
			nrsc5_set_auto_gain(nrsc5, 1);
			nrsc5_set_callback(nrsc5, nrsc5_rx_callback, NULL);

			nrsc5_start(nrsc5);
		}

		/*
		 * Our virtual destructor.
		 */
		nrsc5_rx_impl::~nrsc5_rx_impl() {
			nrsc5_stop(nrsc5);
			nrsc5_close(nrsc5);
		}


		void
		nrsc5_rx_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required) {
			/* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
			unsigned ninputs = ninput_items_required.size ();
			for(unsigned i = 0; i < ninputs; i++)
				ninput_items_required[i] = noutput_items;
		}

		int
		nrsc5_rx_impl::general_work (int noutput_items,
		                   gr_vector_int &ninput_items,
		                   gr_vector_const_void_star &input_items,
		                   gr_vector_void_star &output_items)
		{
			//auto in = static_cast<const input_type*>(input_items[0]);
			const int16_t* in = static_cast<const int16_t*>(input_items[0]);

			// disabled until I have a callback function
			auto out0 = static_cast<int16_t*>(output_items[0]);
//			auto out0 = static_cast<output_type*>(output_items[0]);
//			auto out1 = static_cast<output_type*>(output_items[1]);

			// Do <+signal processing+>
			// Tell runtime system how many input items we consumed on
			// each input stream.
			nrsc5_pipe_samples_cs16(nrsc5, in, 2 * noutput_items);

			for (int i = 0; i < noutput_items; i++) {
				out0[i] = in[i];
			}

			consume_each (noutput_items);

			// Tell runtime system how many output items we produced.
			return noutput_items;
		}

	} /* namespace nrsc5_rx */
} /* namespace gr */

void nrsc5_rx_callback(const nrsc5_event_t *evt, void *opaque) {
	std::cerr << "callback\n";
	return;
}
