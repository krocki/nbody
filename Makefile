GCC=gcc
TARGETS = nbody
INCLUDES=../clib/
CFLAGS = -g3 -O0 -fPIC -std=c11 -Wfatal-errors
LFLAGS = -lm

.SUFFIXES:

all: $(TARGETS)

%: %.o
	$(GCC) $(LFLAGS) -o $@ $^

%.o: %.c
	$(GCC) -I$(INCLUDES) $(CFLAGS) -c $< -o $@

%.o: %.c %.h
	$(GCC) -I$(INCLUDES) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(TARGETS) *.o
