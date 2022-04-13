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
		nrsc5_rx::make(int program, bool test)
		{
			return gnuradio::make_block_sptr<nrsc5_rx_impl>(program, test);
		}


		/*
		 * The private constructor
		 */
		nrsc5_rx_impl::nrsc5_rx_impl(int program, bool test)
			: gr::block("nrsc5_rx",
				gr::io_signature::make(1 /* min inputs */, 1 /* max inputs */, 2 * sizeof(int16_t)),
				gr::io_signature::make(1 /* min outputs */, 1 /*max outputs */, sizeof(int16_t)))
				// above is temporary
		{
			_test = test;

			if ((program >= 0) && (program < 4))
				_program = program;

			if (nrsc5_open_pipe(&nrsc5) != 0)
				throw std::runtime_error("nrsc5_rx_impl: nrsc5_open_pipe failed");

			// other vars?
			nrsc5_set_auto_gain(nrsc5, 1);
			nrsc5_set_callback(nrsc5, nrsc5_rx_callback, NULL);

			nrsc5_start(nrsc5);
			std::cerr << "started nrsc5\n";
		}

		/*
		 * Our virtual destructor.
		 */
		nrsc5_rx_impl::~nrsc5_rx_impl() {
			nrsc5_stop(nrsc5);
			nrsc5_close(nrsc5);
			std::cerr << "stopped nrsc5\n";
		}


		void
		nrsc5_rx_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required) {
			/* <+forecast+> e.g. ninput_items_required[0] = noutput_items */
			unsigned ninputs = ninput_items_required.size ();
			for(unsigned i = 0; i < ninputs; i++) {
//				ninput_items_required[i] = noutput_items;
//				std::cerr << "input" << i << " needs: " << (noutput_items * 135) / 8 << "\n";
				ninput_items_required[i] = (noutput_items * 135) / 16;
			}

			// add a case here for when nrsc5 doesn't have sync
			// maybe for when nrsc5 has a reasonably sized buffer of data but hasn't output anything yet?
		}

		int
		nrsc5_rx_impl::general_work (int noutput_items,
		                   gr_vector_int &ninput_items,
		                   gr_vector_const_void_star &input_items,
		                   gr_vector_void_star &output_items)
		{
			int n = 0;
			//auto in = static_cast<const input_type*>(input_items[0]);
			const int16_t* in = static_cast<const int16_t*>(input_items[0]);

			// disabled until I have a callback function
			auto out0 = static_cast<int16_t*>(output_items[0]);
//			auto out0 = static_cast<output_type*>(output_items[0]);
//			auto out1 = static_cast<output_type*>(output_items[1]);

			// Do <+signal processing+>
			// Tell runtime system how many input items we consumed on
			// each input stream.
			nrsc5_pipe_samples_cs16(nrsc5, in, 2 * ninput_items[0]);

			for (int i = 0; i < ninput_items[0]; i++) {
				if (audio_queue.empty())
					break;
//				out0[i] = in[i];
				out0[i] = audio_queue.front();
				audio_queue.pop();
				n = i;
			}

			consume_each (ninput_items[0]);

			if (n > 0)
				std::cerr << "n=" << n << " ninputitems: " << ninput_items[0] << "\n";

			// Tell runtime system how many output items we produced.
			return n;
		}

	} /* namespace nrsc5_rx */
} /* namespace gr */

void nrsc5_rx_callback(const nrsc5_event_t *event, void *opaque) {
	switch (event->event) {
		case NRSC5_EVENT_LOST_DEVICE: // rtl_sdr disconnected. I doubt I'll need this
			std::cerr << "NRSC5_EVENT_LOST_DEVICE\n";
			break;
		case NRSC5_EVENT_BER: // Bit Error Rate
			std::cerr << "NRSC5_EVENT_BER\n";
			break;
		case NRSC5_EVENT_MER: // Modulation Error Ratio?
			std::cerr << "NRSC5_EVENT_MER\n";
			break;
		case NRSC5_EVENT_IQ: // IQ data
			std::cerr << "NRSC5_EVENT_IQ\n";
			break;
		case NRSC5_EVENT_HDC: // HDC audio packet?
//			std::cerr << "NRSC5_EVENT_HDC\n";
			break;
		case NRSC5_EVENT_AUDIO: // Audio
//			std::cerr << "NRSC5_EVENT_AUDIO\n";
			if (event->audio.program == _program) {
				std::cerr << "    Program: " << event->audio.program << " count: " << event->audio.count << "\n";
				for (int i = 0; i < 4096; i++) {
//					std::cerr << " " << (event->audio.data)[i];
					audio_queue.push((event->audio.data)[i]);
				}
//				audio_queue.push(event->audio.data);
			}
			break;
		case NRSC5_EVENT_SYNC: // Got sync
			std::cerr << "NRSC5_EVENT_SYNC\n";
			break;
		case NRSC5_EVENT_LOST_SYNC: // Sync lost
			std::cerr << "NRSC5_EVENT_LOST_SYNC\n";
			break;
		case NRSC5_EVENT_ID3: // ID3 information?
			std::cerr << "NRSC5_EVENT_ID3\n";
			break;
		case NRSC5_EVENT_SIG: // Service information?
			std::cerr << "NRSC5_EVENT_SIG\n";
			break;
		case NRSC5_EVENT_LOT: // LOT file data available?
			std::cerr << "NRSC5_EVENT_LOT\n";
			break;
		case NRSC5_EVENT_SIS: // Station information
			std::cerr << "NRSC5_EVENT_SIS\n";
			break;
		default:
			std::cerr << "Unknown event\n";
	}

	return;
}
