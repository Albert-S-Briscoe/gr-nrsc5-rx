/* -*- c++ -*- */
/*
 * Copyright 2022 Albert Briscoe.
 *
 * SPDX-License-Identifier: GPL-3.0-or-later
 */

#ifndef INCLUDED_NRSC5_RX_NRSC_5_RECEIVER_H
#define INCLUDED_NRSC5_RX_NRSC_5_RECEIVER_H

#include <gnuradio/hier_block2.h>
#include <nrsc5_rx/api.h>

namespace gr {
namespace nrsc5_rx {

/*!
 * \brief <+description of block+>
 * \ingroup nrsc5_rx
 *
 */
class NRSC5_RX_API nrsc_5_receiver : virtual public gr::hier_block2
{
public:
    typedef std::shared_ptr<nrsc_5_receiver> sptr;

    /*!
     * \brief Return a shared_ptr to a new instance of nrsc5_rx::nrsc_5_receiver.
     *
     * To avoid accidental use of raw pointers, nrsc5_rx::nrsc_5_receiver's
     * constructor is in a private implementation
     * class. nrsc5_rx::nrsc_5_receiver::make is the public interface for
     * creating new instances.
     */
    static sptr make(int program = 0, float samp_rate = 1200000, float audio_rate = 44100);

    virtual void set_program(int program) = 0;
};

} // namespace nrsc5_rx
} // namespace gr

#endif /* INCLUDED_NRSC5_RX_NRSC_5_RECEIVER_H */
