GCC=gcc
TARGETS = nbody nbody_gl
INCLUDES=.
CFLAGS = -g -O1 -fPIC -std=c99 -Wfatal-errors
LFLAGS = -lm
HEADERS:=$(wildcard *.h)

.SUFFIXES:

all: $(TARGETS)

nbody_gl: main_gl.o nbody.o randf.o
	$(GCC) -o $@ $^ $(LFLAGS)

nbody: main.o nbody.o randf.o
	$(GCC) -o $@ $^ $(LFLAGS)

%.o: %.c $(HEADERS)
	$(GCC) -I$(INCLUDES) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(TARGETS) *.o
