#include <stdio.h>
#include <stdlib.h>
#include "timer.h"

extern void sim(unsigned, unsigned );

int main(int argc, char **argv) {

  unsigned n = argc > 1 ? atoi(argv[1]) : 100;
  unsigned i = argc > 2 ? atoi(argv[2]) : 1;

  printf("n=%u, i=%i\n", n, i);
  timeit(sim(n, i));
  printf("total mflops: %.3f\n", 18 * (double)(n * n * i) / (1024.0 * 1024.0));
  return 0;

}
