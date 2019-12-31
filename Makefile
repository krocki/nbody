CC=gcc
TARGETS = nbody nbody_gl bh
INCLUDES=.
OPT_LEVEL=
#-Ofast -mavx -fomit-frame-pointer -fPIC -mtune=native
CFLAGS = -g -std=c99 -Wfatal-errors $(OPT_LEVEL)
LFLAGS = -g $(OPT_LEVEL) -flto -lm
HEADERS:=$(wildcard *.h) Makefile
OS:=$(shell uname)

ifeq ($(OS),Darwin) #OSX
  GL_FLAGS=-lglfw -framework OpenGL -lpthread
else # Linux or other
  GL_FLAGS=-lglfw -lGL -lpthread
endif

.SUFFIXES:

all: $(TARGETS)

nbody_gl: main_gl.o nbody.o randf.o rsqrt.o randnf.o
	$(CC) -o $@ $^ $(LFLAGS) $(GL_FLAGS)

nbody: main.o nbody.o randf.o rsqrt.o
	$(CC) -o $@ $^ $(LFLAGS)

bh: bh.o nbody-bh.o randf.o oct.o
	$(CC) -o $@ $^ $(LFLAGS)

%.o: %.c $(HEADERS)
	$(CC) -I$(INCLUDES) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(TARGETS) *.o
