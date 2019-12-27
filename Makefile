GCC=gcc
TARGETS = nbody
INCLUDES=
CFLAGS = -g3 -O0 -fPIC -std=c11 -Wfatal-errors
LFLAGS = -lm

.SUFFIXES:

all: $(TARGETS)

%: %.o
	$(GCC) -o $@ $^ $(LFLAGS)

%.o: %.c
	$(GCC) -I$(INCLUDES) $(CFLAGS) -c $< -o $@

%.o: %.c %.h
	$(GCC) -I$(INCLUDES) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(TARGETS) *.o
