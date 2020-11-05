RGB_LIB_DISTRIBUTION=rpi-rgb-led-matrix
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib

all: rgbmatrix ltbl

rgbmatrix:
	$(MAKE) -C $(RGB_LIBDIR)

ltbl: rgbmatrix
	$(MAKE) -f Makefile.ltbl

debug: rgbmatrix
	CFLAGS="-O0 -ggdb" $(MAKE) -e -f Makefile.ltbl

clean:
	$(MAKE) clean -C $(RGB_LIBDIR)
	$(MAKE) clean -f Makefile.ltbl

test: all
	sudo pd-l2ork ltbl-help.pd

.PHONY: clean rgbmatrix ltbl all
