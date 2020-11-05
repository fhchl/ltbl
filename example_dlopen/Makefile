app = dlopen_sample
lib = libcat.so libdog.so

CFLAGS = -Wall -pedantic
LDFLAGS = -ldl

all: $(app) $(lib)


lib%.so: %.c
	$(CC) -shared -fPIC -o $@ $<

clean:
	$(RM) $(app) $(lib)

run: all
	./$(app) cat
	./$(app) dog
	-./$(app) bear

$(lib): animal.h
