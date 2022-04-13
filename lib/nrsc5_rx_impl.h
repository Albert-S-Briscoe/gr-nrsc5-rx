/* -*- c++ -*- */
/*
 * Copyright 2022 Albert Briscoe.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_NRSC5_RX_NRSC5_RX_IMPL_H
#define INCLUDED_NRSC5_RX_NRSC5_RX_IMPL_H

#include <nrsc5_rx/nrsc5_rx.h>
#include <queue>

namespace gr {
namespace nrsc5_rx {

	class nrsc5_rx_impl : public nrsc5_rx {
	private:
		bool _test;

		nrsc5_t* nrsc5;

	public:
		nrsc5_rx_impl(int program, bool test);
		~nrsc5_rx_impl();

		// Where all the action really happens
		void forecast (int noutput_items, gr_vector_int &ninput_items_required);

		int general_work(int noutput_items,
		gr_vector_int &ninput_items,
		gr_vector_const_void_star &input_items,
		gr_vector_void_star &output_items);

    };

  } // namespace nrsc5_rx
} // namespace gr

unsigned int _program;
std::queue<uint16_t> left_audio_queue;
std::queue<uint16_t> right_audio_queue;

void nrsc5_rx_callback(const nrsc5_event_t *evt, void *opaque);

#endif /* INCLUDED_NRSC5_RX_NRSC5_RX_IMPL_H */
