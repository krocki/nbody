CC=gcc
TARGETS = nbody nbody_gl bh bh_gl
INCLUDES=-I/usr/local/include/ -I.
OPT_LEVEL=
#-Ofast -mavx -fomit-frame-pointer -fPIC -mtune=native
CFLAGS = -g -std=c11 -Wfatal-errors $(OPT_LEVEL) -pedantic -Wextra
LFLAGS = -g -L/usr/local/lib $(OPT_LEVEL) -flto -lm
HEADERS:=$(wildcard *.h) Makefile

OS:=$(shell uname)
ifeq ($(OS),Darwin) #OSX
  GL_FLAGS=-lglfw -framework OpenGL -lpthread
  CFLAGS:=$(CFLAGS) -DAPPLE -DGL_SILENCE_DEPRECATION
else # Linux or other
  GL_FLAGS=-lglfw -lGL -lpthread
endif

.SUFFIXES:

all: $(TARGETS)

nbody_gl: main_gl.o nbody.o randf.o rsqrt.o randnf.o
	$(CC) -o $@ $^ $(LFLAGS) $(GL_FLAGS)

nbody: main.o nbody.o randf.o rsqrt.o
	$(CC) -o $@ $^ $(LFLAGS)

bh_gl: bh.o main-bh-gl.o randf.o randnf.o oct.o particle.o
	$(CC) -o $@ $^ $(LFLAGS) $(GL_FLAGS)

bh: bh.o main-bh.o randf.o oct.o particle.o
	$(CC) -o $@ $^ $(LFLAGS)

%.o: %.c $(HEADERS)
	$(CC) -I$(INCLUDES) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(TARGETS) *.o
