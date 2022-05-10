### GNU Radio module for receiving NRSC-5 (HD Radio)

Lets you use [nrsc5](https://github.com/theori-io/nrsc5) as a block in [GNU Radio](https://www.gnuradio.org/)

Outputs audio and SIS/ID3 data.

### Basic build instructions:
```
cd gr-nrsc5-rx
mkdir build
cd build
cmake -DENABLE_DOXYGEN=OFF ..
make
sudo make install
```
