RGB_LIBDIR=rpi-rgb-led-matrix/lib

all: rgbmatrix ltbl

rgbmatrix:
	$(MAKE) -C rpi-rgb-led-matrix/lib

ltbl: rgbmatrix
	$(MAKE) -f Makefile.ltbl


clean:
	$(MAKE) clean -C rpi-rgb-led-matrix/lib
	$(MAKE) clean -f Makefile.ltbl
	
test: all
	pd-l2ork ltbl-help.pd &

.PHONY: clean rgbmatrix ltbl all
