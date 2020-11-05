RGB_LIBDIR=rpi-rgb-led-matrix/lib

RGB_LIB_DISTRIBUTION=rpi-rgb-led-matrix
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a
RGB_LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread

all: rgbmatrix ltbl

rgbmatrix:
	$(MAKE) -C rpi-rgb-led-matrix/lib

ltbl: rgbmatrix
	$(MAKE) -f Makefile.ltbl
	
	
libexample.so: rgbmatrix
	cc -shared -fPIC -I$(RGB_INCDIR) -L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread  -o libexample.so libexample.c 

main: libexample.so
	cc -rdynamic -ldl -c main.c

debug: rgbmatrix
	CFLAGS="-O0 -ggdb" $(MAKE) -e -f Makefile.ltbl
	
clean:
	$(MAKE) clean -C rpi-rgb-led-matrix/lib
	$(MAKE) clean -f Makefile.ltbl
	
test: all
	sudo pd-l2ork ltbl-help.pd

.PHONY: clean rgbmatrix ltbl all
