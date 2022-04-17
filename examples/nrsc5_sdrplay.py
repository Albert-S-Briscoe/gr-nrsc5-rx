#!/usr/bin/env python3
# -*- coding: utf-8 -*-

#
# SPDX-License-Identifier: GPL-3.0
#
# GNU Radio Python Flow Graph
# Title: SDRPlay NRSC-5 Receiver
# Author: Albert Briscoe
# GNU Radio version: 3.9.5.0

from distutils.version import StrictVersion

if __name__ == '__main__':
    import ctypes
    import sys
    if sys.platform.startswith('linux'):
        try:
            x11 = ctypes.cdll.LoadLibrary('libX11.so')
            x11.XInitThreads()
        except:
            print("Warning: failed to XInitThreads()")

from PyQt5 import Qt
from PyQt5.QtCore import QObject, pyqtSlot
from gnuradio import qtgui
from gnuradio.filter import firdes
import sip
from gnuradio import audio
from gnuradio import gr
from gnuradio.fft import window
import sys
import signal
from argparse import ArgumentParser
from gnuradio.eng_arg import eng_float, intx
from gnuradio import eng_notation
from gnuradio import soapy
from gnuradio.qtgui import Range, RangeWidget
from PyQt5 import QtCore
import nrsc5_rx



from gnuradio import qtgui

class nrsc5_sdrplay(gr.top_block, Qt.QWidget):

    def __init__(self):
        gr.top_block.__init__(self, "SDRPlay NRSC-5 Receiver", catch_exceptions=True)
        Qt.QWidget.__init__(self)
        self.setWindowTitle("SDRPlay NRSC-5 Receiver")
        qtgui.util.check_set_qss()
        try:
            self.setWindowIcon(Qt.QIcon.fromTheme('gnuradio-grc'))
        except:
            pass
        self.top_scroll_layout = Qt.QVBoxLayout()
        self.setLayout(self.top_scroll_layout)
        self.top_scroll = Qt.QScrollArea()
        self.top_scroll.setFrameStyle(Qt.QFrame.NoFrame)
        self.top_scroll_layout.addWidget(self.top_scroll)
        self.top_scroll.setWidgetResizable(True)
        self.top_widget = Qt.QWidget()
        self.top_scroll.setWidget(self.top_widget)
        self.top_layout = Qt.QVBoxLayout(self.top_widget)
        self.top_grid_layout = Qt.QGridLayout()
        self.top_layout.addLayout(self.top_grid_layout)

        self.settings = Qt.QSettings("GNU Radio", "nrsc5_sdrplay")

        try:
            if StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
                self.restoreGeometry(self.settings.value("geometry").toByteArray())
            else:
                self.restoreGeometry(self.settings.value("geometry"))
        except:
            pass

        ##################################################
        # Variables
        ##################################################
        self.samp_rate = samp_rate = 3000000
        self.rsp_gain = rsp_gain = -45
        self.program = program = 0
        self.freq = freq = 915e5
        self.audio_rate = audio_rate = 44100

        ##################################################
        # Blocks
        ##################################################
        self._rsp_gain_range = Range(-47, 0, 1, -45, 200)
        self._rsp_gain_win = RangeWidget(self._rsp_gain_range, self.set_rsp_gain, "SDR Gain", "counter_slider", float, QtCore.Qt.Horizontal)
        self.top_grid_layout.addWidget(self._rsp_gain_win, 1, 0, 1, 1)
        for r in range(1, 2):
            self.top_grid_layout.setRowStretch(r, 1)
        for c in range(0, 1):
            self.top_grid_layout.setColumnStretch(c, 1)
        # Create the options list
        self._program_options = [0, 1, 2, 3]
        # Create the labels list
        self._program_labels = ['HD-1', 'HD-2', 'HD-3', 'HD-4']
        # Create the combo box
        # Create the radio buttons
        self._program_group_box = Qt.QGroupBox("Program" + ": ")
        self._program_box = Qt.QHBoxLayout()
        class variable_chooser_button_group(Qt.QButtonGroup):
            def __init__(self, parent=None):
                Qt.QButtonGroup.__init__(self, parent)
            @pyqtSlot(int)
            def updateButtonChecked(self, button_id):
                self.button(button_id).setChecked(True)
        self._program_button_group = variable_chooser_button_group()
        self._program_group_box.setLayout(self._program_box)
        for i, _label in enumerate(self._program_labels):
            radio_button = Qt.QRadioButton(_label)
            self._program_box.addWidget(radio_button)
            self._program_button_group.addButton(radio_button, i)
        self._program_callback = lambda i: Qt.QMetaObject.invokeMethod(self._program_button_group, "updateButtonChecked", Qt.Q_ARG("int", self._program_options.index(i)))
        self._program_callback(self.program)
        self._program_button_group.buttonClicked[int].connect(
            lambda i: self.set_program(self._program_options[i]))
        self.top_grid_layout.addWidget(self._program_group_box, 2, 0, 1, 1)
        for r in range(2, 3):
            self.top_grid_layout.setRowStretch(r, 1)
        for c in range(0, 1):
            self.top_grid_layout.setColumnStretch(c, 1)
        self._freq_range = Range(881e5, 1079e5, 2e5, 915e5, 200)
        self._freq_win = RangeWidget(self._freq_range, self.set_freq, "Frequency", "counter_slider", float, QtCore.Qt.Horizontal)
        self.top_grid_layout.addWidget(self._freq_win, 0, 0, 1, 1)
        for r in range(0, 1):
            self.top_grid_layout.setRowStretch(r, 1)
        for c in range(0, 1):
            self.top_grid_layout.setColumnStretch(c, 1)
        self.soapy_sdrplay_source_0 = None
        dev = 'driver=sdrplay'
        stream_args = ''
        tune_args = ['']
        settings = ['']

        self.soapy_sdrplay_source_0 = soapy.source(dev, "fc32", 1, '',
                                  stream_args, tune_args, settings)
        self.soapy_sdrplay_source_0.set_sample_rate(0, samp_rate)
        self.soapy_sdrplay_source_0.set_bandwidth(0, 5000000.0)
        self.soapy_sdrplay_source_0.set_antenna(0, 'RX')
        self.soapy_sdrplay_source_0.set_gain_mode(0, False)
        self.soapy_sdrplay_source_0.set_frequency(0, freq)
        self.soapy_sdrplay_source_0.set_frequency_correction(0, 0)
        self.soapy_sdrplay_source_0.set_gain(0, min(max(-rsp_gain, 0.0), 47.0))
        self.qtgui_sink_x_0 = qtgui.sink_c(
            1024, #fftsize
            window.WIN_BLACKMAN_hARRIS, #wintype
            0, #fc
            samp_rate, #bw
            "", #name
            True, #plotfreq
            True, #plotwaterfall
            True, #plottime
            False, #plotconst
            None # parent
        )
        self.qtgui_sink_x_0.set_update_time(1.0/10)
        self._qtgui_sink_x_0_win = sip.wrapinstance(self.qtgui_sink_x_0.qwidget(), Qt.QWidget)

        self.qtgui_sink_x_0.enable_rf_freq(False)

        self.top_grid_layout.addWidget(self._qtgui_sink_x_0_win, 3, 0, 10, 1)
        for r in range(3, 13):
            self.top_grid_layout.setRowStretch(r, 1)
        for c in range(0, 1):
            self.top_grid_layout.setColumnStretch(c, 1)
        self.nrsc5_rx_nrsc_5_receiver_0 = nrsc5_rx.nrsc_5_receiver(program, samp_rate, audio_rate)
        self.audio_sink_0 = audio.sink(audio_rate, '', True)


        ##################################################
        # Connections
        ##################################################
        self.connect((self.nrsc5_rx_nrsc_5_receiver_0, 1), (self.audio_sink_0, 1))
        self.connect((self.nrsc5_rx_nrsc_5_receiver_0, 0), (self.audio_sink_0, 0))
        self.connect((self.soapy_sdrplay_source_0, 0), (self.nrsc5_rx_nrsc_5_receiver_0, 0))
        self.connect((self.soapy_sdrplay_source_0, 0), (self.qtgui_sink_x_0, 0))


    def closeEvent(self, event):
        self.settings = Qt.QSettings("GNU Radio", "nrsc5_sdrplay")
        self.settings.setValue("geometry", self.saveGeometry())
        self.stop()
        self.wait()

        event.accept()

    def get_samp_rate(self):
        return self.samp_rate

    def set_samp_rate(self, samp_rate):
        self.samp_rate = samp_rate
        self.qtgui_sink_x_0.set_frequency_range(0, self.samp_rate)
        self.soapy_sdrplay_source_0.set_sample_rate(0, self.samp_rate)

    def get_rsp_gain(self):
        return self.rsp_gain

    def set_rsp_gain(self, rsp_gain):
        self.rsp_gain = rsp_gain
        self.soapy_sdrplay_source_0.set_gain(0, min(max(-self.rsp_gain, 0.0), 47.0))

    def get_program(self):
        return self.program

    def set_program(self, program):
        self.program = program
        self._program_callback(self.program)

    def get_freq(self):
        return self.freq

    def set_freq(self, freq):
        self.freq = freq
        self.soapy_sdrplay_source_0.set_frequency(0, self.freq)

    def get_audio_rate(self):
        return self.audio_rate

    def set_audio_rate(self, audio_rate):
        self.audio_rate = audio_rate




def main(top_block_cls=nrsc5_sdrplay, options=None):

    if StrictVersion("4.5.0") <= StrictVersion(Qt.qVersion()) < StrictVersion("5.0.0"):
        style = gr.prefs().get_string('qtgui', 'style', 'raster')
        Qt.QApplication.setGraphicsSystem(style)
    qapp = Qt.QApplication(sys.argv)

    tb = top_block_cls()

    tb.start()

    tb.show()

    def sig_handler(sig=None, frame=None):
        tb.stop()
        tb.wait()

        Qt.QApplication.quit()

    signal.signal(signal.SIGINT, sig_handler)
    signal.signal(signal.SIGTERM, sig_handler)

    timer = Qt.QTimer()
    timer.start(500)
    timer.timeout.connect(lambda: None)

    qapp.exec_()

if __name__ == '__main__':
    main()
