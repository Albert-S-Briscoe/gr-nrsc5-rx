### GNU Radio OOT module for NRSC-5

Connects [nrsc5](https://github.com/theori-io/nrsc5) to [gnuradio](https://www.gnuradio.org/)

Audio works reasonably well. There's also some example GRC flies. Currently requires [my branch of nrsc5](https://github.com/Albert-S-Briscoe/nrsc5).

Basic build instructions:
```
cd gr-nrsc5-rx
mkdir build
cd build
cmake -DENABLE_DOXYGEN=OFF ..
make
sudo make install
```
