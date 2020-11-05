RGB_LIB_DISTRIBUTION=../rpi-rgb-led-matrix
RGB_INCDIR=$(RGB_LIB_DISTRIBUTION)/include
RGB_LIBDIR=$(RGB_LIB_DISTRIBUTION)/lib
RGB_LIBRARY_NAME=rgbmatrix
RGB_LIBRARY=$(RGB_LIBDIR)/lib$(RGB_LIBRARY_NAME).a
RGB_LDFLAGS+=-L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lrt -lm -lpthread -lstdc++

all: prog

rgbmatrix:
	$(MAKE) -C $(RGB_LIBDIR)

example.o: example.c
	cc -c -Wall -Wpedantic -fpic -I$(RGB_INCDIR) -o $@ $<

libexample.so: example.o rgbmatrix
	# important that parts that refer to symbols come before their definition
	cc -Wall -Wpedantic -shared -o $@ $< -L$(RGB_LIBDIR) -l$(RGB_LIBRARY_NAME) -lstdc++

main.o: main.c
	cc -c -Wall -Wpedantic -o $@ $<

prog: main.o | libexample.so
	cc -o $@ $< -ldl

test: prog
	sudo ./$<

clean:
	rm *.o *.so prog

.PHONY: clean main
