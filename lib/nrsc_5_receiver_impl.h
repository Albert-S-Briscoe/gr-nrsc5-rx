/* -*- c++ -*- */
/*
 * Copyright 2022 Albert Briscoe.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_NRSC5_RX_NRSC_5_RECEIVER_IMPL_H
#define INCLUDED_NRSC5_RX_NRSC_5_RECEIVER_IMPL_H

#include <nrsc5_rx/nrsc_5_receiver.h>

#include <gnuradio/filter/pfb_arb_resampler_ccf.h>
#include <gnuradio/blocks/complex_to_interleaved_short.h>
#include <nrsc5_rx/nrsc5_rx.h>
#include <gnuradio/filter/pfb_arb_resampler_fff.h>

namespace gr {
namespace nrsc5_rx {

class nrsc_5_receiver_impl : public nrsc_5_receiver
{
private:
    std::vector<float> taps;
    std::vector<float> audio_taps;

    gr::filter::pfb_arb_resampler_ccf::sptr resampler;
    gr::blocks::complex_to_interleaved_short::sptr type_converter;
    gr::nrsc5_rx::nrsc5_rx::sptr nrsc5_rx;
    gr::filter::pfb_arb_resampler_fff::sptr audio_resampler0;
    gr::filter::pfb_arb_resampler_fff::sptr audio_resampler1;

	void set_program(int program) override;

public:
    nrsc_5_receiver_impl(int program, float samp_rate, float audio_rate);
    ~nrsc_5_receiver_impl();

    // Where all the action really happens
};

} // namespace nrsc5_rx
} // namespace gr

#endif /* INCLUDED_NRSC5_RX_NRSC_5_RECEIVER_IMPL_H */
