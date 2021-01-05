#  rpi-rgb-led-matrix for Pd

> Let there be light!

This is a simple wrapper of the excellent [rpi-rgb-led-matrix](https://github.com/hzeller/rpi-rgb-led-matrix) library for Pure Data. The provided Pd external makes it easy to draw on a LED matrix with a Raspberry Pi. Just convert your images or animations to lists with RGB or RGBA values for each LED pixel and send it to the object.

# Build

Clone the repo and build the object.

    git clone
    cd ltbl
    git submodule update --init --recursive
    make

# Usage

See the provided help patch. You can run it with

    make test

The `Makefile` assumes purr-data, which you have to run as root for `rpi-rgb-led-matrix` to work!
