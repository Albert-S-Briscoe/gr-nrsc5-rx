/* -*- c++ -*- */
/*
 * Copyright 2022 Albert Briscoe.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "nrsc5_rx_impl.h"

#include <gnuradio/io_signature.h>
#include <stdexcept>
#include <string>

namespace gr {
	namespace nrsc5_rx {

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
				gr::io_signature::make(2 /* min outputs */, 2 /*max outputs */, sizeof(float)))
		{
			message_port_register_out(pmt::mp("out"));

			_test = test;
			nrsc5_sync = 0;
			new_sis_message = false;
			new_id3_message = false;

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
				ninput_items_required[i] = (noutput_items * 135) / 8;
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

			auto out0 = static_cast<float*>(output_items[0]);
			auto out1 = static_cast<float*>(output_items[1]);

			// Do <+signal processing+>
			// Tell runtime system how many input items we consumed on
			// each input stream.
			nrsc5_pipe_samples_cs16(nrsc5, in, 2 * ninput_items[0]);

			n = 0;
			for (int i = 0; i < noutput_items * 2; i++, n++) {
				if (left_audio_queue.empty() || right_audio_queue.empty())
					break;
				out0[i] = (float)left_audio_queue.front() / 32767;
				out1[i] = (float)right_audio_queue.front() / 32767;
				left_audio_queue.pop();
				right_audio_queue.pop();
			}

			consume_each (ninput_items[0]);

			// output SIS information
			if (new_sis_message) {
				message_port_pub(pmt::mp("out"), sis_message);
				new_sis_message = false;
			}
			if (new_id3_message) {
				message_port_pub(pmt::mp("out"), id3_message);
				new_id3_message = false;
			}

//			if (n > 0)
//				std::cerr << "produced=" << n << " input: " << ninput_items[0] << " wanted: " << noutput_items << " size: " << left_audio_queue.size() << "\n";

			// Tell runtime system how many output items we produced.
			return n;
		}

		void nrsc5_rx_impl::set_program(int program) {
			std::cerr << "program: " << program << "\n";
			_program = program;

			// Clear program specific id3 information
			message_port_pub(pmt::mp("out"), pmt::mp(pmt::from_long(1), pmt::mp(""), pmt::mp(""), pmt::mp(""), pmt::mp("")));
			new_id3_message = false;
		}

		int nrsc5_rx_impl::get_sync() {
			return nrsc5_sync;
		}

	} /* namespace nrsc5_rx */
} /* namespace gr */

void nrsc5_rx_callback(const nrsc5_event_t *event, void *opaque) {
	switch (event->event) {
		case NRSC5_EVENT_LOST_DEVICE: // rtl_sdr disconnected. I doubt I'll need this (because I'm piping data)
//			std::cerr << "NRSC5_EVENT_LOST_DEVICE\n";
			nrsc5_sync = 0;
			break;

		// use these for signal level?
		case NRSC5_EVENT_BER: // Bit Error Rate
//			std::cerr << "NRSC5_EVENT_BER\n";
			break;
		case NRSC5_EVENT_MER: // Modulation Error Ratio?
//			std::cerr << "NRSC5_EVENT_MER\n";
			break;

		case NRSC5_EVENT_IQ: // IQ data
//			std::cerr << "NRSC5_EVENT_IQ\n";
			break;
		case NRSC5_EVENT_HDC: // HDC audio packet?
//			std::cerr << "NRSC5_EVENT_HDC\n";
			break;
		case NRSC5_EVENT_AUDIO: // Audio
//			std::cerr << "NRSC5_EVENT_AUDIO\n";
			if (event->audio.program == _program) {
//				std::cerr << "    Program: " << event->audio.program << " count: " << event->audio.count << "\n";
				for (int i = 0; i < 2048; i++) {
					left_audio_queue.push((event->audio.data)[2*i]);
					right_audio_queue.push((event->audio.data)[2*i+1]);
				}
//				audio_queue.push(event->audio.data);
			}
			break;
		case NRSC5_EVENT_SYNC: // Got sync
//			std::cerr << "NRSC5_EVENT_SYNC\n";
			nrsc5_sync = 1;
			break;
		case NRSC5_EVENT_LOST_SYNC: // Sync lost
//			std::cerr << "NRSC5_EVENT_LOST_SYNC\n";
			nrsc5_sync = 0;
			break;
		case NRSC5_EVENT_ID3: // ID3 information?
//			std::cerr << "NRSC5_EVENT_ID3\n";

			if (event->id3.program == _program)
				parse_id3(event);

			break;
		case NRSC5_EVENT_SIG: // Service information?
//			std::cerr << "NRSC5_EVENT_SIG\n";
			break;
		case NRSC5_EVENT_LOT: // LOT file data available?
//			std::cerr << "NRSC5_EVENT_LOT\n";
			break;
		case NRSC5_EVENT_SIS: // Station information
//			std::cerr << "NRSC5_EVENT_SIS\n";

/*			fprintf(stderr, "name            \"%s\"\n", event->sis.name);
			fprintf(stderr, "slogan          \"%s\"\n", event->sis.slogan);
			fprintf(stderr, "message         \"%s\"\n", event->sis.message);
			fprintf(stderr, "alert           \"%s\"\n", event->sis.alert);
			fprintf(stderr, "country_code    \"%s\"\n", event->sis.country_code);
			fprintf(stderr, "fcc_facility_id \"%d\"\n", event->sis.fcc_facility_id);*/

			// null values can cause errors in pmt::string_to_symbol
			nrsc5_tmp_pmt[0] = pmt::string_to_symbol(event->sis.name ? event->sis.name : "");
			nrsc5_tmp_pmt[1] = pmt::string_to_symbol(event->sis.slogan ? event->sis.slogan : "");
			nrsc5_tmp_pmt[2] = pmt::string_to_symbol(event->sis.message ? event->sis.message : "");
			nrsc5_tmp_pmt[3] = pmt::string_to_symbol(event->sis.alert ? event->sis.alert : "");
			nrsc5_tmp_pmt[4] = pmt::string_to_symbol(event->sis.country_code ? event->sis.country_code : "");

			sprintf(nrsc5_facility_id, "%d", event->sis.fcc_facility_id);
			nrsc5_tmp_pmt[5] = pmt::string_to_symbol(nrsc5_facility_id);

			// pmt::from_long(0) indicates that this is SIS data
			sis_message = pmt::mp(pmt::from_long(0), nrsc5_tmp_pmt[0], nrsc5_tmp_pmt[1], nrsc5_tmp_pmt[2], nrsc5_tmp_pmt[3], nrsc5_tmp_pmt[4], nrsc5_tmp_pmt[5]);
			new_sis_message = true;
			break;
		default:
			std::cerr << "Unknown event\n";
	}

	return;
}

// could this be in the nrsc5_rx_impl class?
void parse_id3(const nrsc5_event_t *event) {
//	fprintf(stderr, "program    \"%d\"\n", event->id3.program);
/*	fprintf(stderr, "title      \"%s\"\n", event->id3.title);
	fprintf(stderr, "artist     \"%s\"\n", event->id3.artist);
	fprintf(stderr, "album      \"%s\"\n", event->id3.album);
	fprintf(stderr, "genre      \"%s\"\n", event->id3.genre);

	fprintf(stderr, "ufid owner \"%s\"\n", event->id3.ufid.owner);
	fprintf(stderr, "ufid id    \"%s\"\n", event->id3.ufid.id);

	fprintf(stderr, "xhdr mime  \"%x\"\n", event->id3.xhdr.mime);
	fprintf(stderr, "xhdr param \"%d\"\n", event->id3.xhdr.param);
	fprintf(stderr, "xhdr lot   \"%d\"\n", event->id3.xhdr.lot);*/

	nrsc5_tmp_pmt[0] = pmt::string_to_symbol(event->id3.title ? event->id3.title : "");
	nrsc5_tmp_pmt[1] = pmt::string_to_symbol(event->id3.artist ? event->id3.artist : "");
	nrsc5_tmp_pmt[2] = pmt::string_to_symbol(event->id3.album ? event->id3.album : "");
	nrsc5_tmp_pmt[3] = pmt::string_to_symbol(event->id3.genre ? event->id3.genre : "");

	// pmt::from_long(1) indicates that this is ID3 data
	id3_message = pmt::mp(pmt::from_long(1), nrsc5_tmp_pmt[0], nrsc5_tmp_pmt[1], nrsc5_tmp_pmt[2], nrsc5_tmp_pmt[3]);
	new_id3_message = true;
}
