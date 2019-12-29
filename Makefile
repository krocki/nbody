CC=gcc
TARGETS = nbody nbody_gl
INCLUDES=.
CFLAGS = -g -Ofast -fPIC -fomit-frame-pointer -std=c99 -Wfatal-errors
LFLAGS = -g -Ofast -flto -lm
HEADERS:=$(wildcard *.h)

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

%.o: %.c $(HEADERS)
	$(CC) -I$(INCLUDES) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(TARGETS) *.o
