/* -*- c++ -*- */
/*
 * Copyright 2022 Albert Briscoe.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#include "nrsc_5_receiver_impl.h"
#include <gnuradio/io_signature.h>

#include <gnuradio/filter/firdes.h>

namespace gr {
namespace nrsc5_rx {

//#pragma message("set the following appropriately and remove this warning")
using input_type = gr_complex;
//#pragma message("set the following appropriately and remove this warning")
using output_type = float;
nrsc_5_receiver::sptr
nrsc_5_receiver::make(int program, float samp_rate, float audio_rate)
{
    return gnuradio::make_block_sptr<nrsc_5_receiver_impl>(
        program, samp_rate, audio_rate);
}


/*
 * The private constructor
 */
nrsc_5_receiver_impl::nrsc_5_receiver_impl(int program, float samp_rate, float audio_rate)
    : gr::hier_block2("nrsc_5_receiver",
                      gr::io_signature::make(1 /* min inputs */, 1 /* max inputs */, sizeof(input_type)),
                      gr::io_signature::make(2 /* min outputs */, 2 /*max outputs */, sizeof(output_type)))
{
    double resampler_rate = 744187.5/samp_rate;
	double cutoff = resampler_rate > 1.0 ? 0.4 : 0.4*resampler_rate;
    double trans_width = resampler_rate > 1.0 ? 0.2 : 0.2*resampler_rate;
    taps = gr::filter::firdes::low_pass(32, 32, cutoff, trans_width);

	double audio_resampler_rate = audio_rate/44100;
	double audio_cutoff = audio_resampler_rate > 1.0 ? 0.4 : 0.4*audio_resampler_rate;
    double audio_trans_width = audio_resampler_rate > 1.0 ? 0.2 : 0.2*audio_resampler_rate;
    audio_taps = gr::filter::firdes::low_pass(32, 32, audio_cutoff, audio_trans_width);


    resampler = gr::filter::pfb_arb_resampler_ccf::make(resampler_rate, taps, 32);
	type_converter = gr::blocks::complex_to_interleaved_short::make(true, 32767);
    nrsc5_rx = gr::nrsc5_rx::nrsc5_rx::make(program);
    audio_resampler0 = gr::filter::pfb_arb_resampler_fff::make(audio_resampler_rate, audio_taps, 32);
    audio_resampler1 = gr::filter::pfb_arb_resampler_fff::make(audio_resampler_rate, audio_taps, 32);

    connect(self(), 0, resampler, 0);
    connect(resampler, 0, type_converter, 0);
    connect(type_converter, 0, nrsc5_rx, 0);
    connect(nrsc5_rx, 0, audio_resampler0, 0);
    connect(nrsc5_rx, 1, audio_resampler1, 0);
    connect(audio_resampler0, 0, self(), 0);
    connect(audio_resampler1, 0, self(), 1);
}

/*
 * Our virtual destructor.
 */
nrsc_5_receiver_impl::~nrsc_5_receiver_impl() {}

void nrsc_5_receiver_impl::set_program(int program) {
	// use callback to set program
}

} /* namespace nrsc5_rx */
} /* namespace gr */
