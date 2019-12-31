#include <sys/time.h>
#include <time.h>
#include "rdtsc.h"

double get_time() {
  struct timeval tv; gettimeofday(&tv, NULL);
  return (tv.tv_sec + tv.tv_usec * 1e-6);
}

void delay(int ms) {
  long pause;
  clock_t now,then;

  pause = ms*(CLOCKS_PER_SEC/1000);
  now = then = clock();
  while( (now-then) < pause )
      now = clock();
}

#define timeit(expr)  \
  do { double ts=get_time(); \
       unsigned long long cs=rdtsc(); \
       (expr); \
       unsigned long long ce=rdtsc(); \
       double te=get_time(); \
       printf("`" #expr "` took %f s, %llu cycles\n",\
           te-ts, ce-cs); \
  } while (0)

#define timeit2(expr, val)  \
  do { double ts=get_time(); \
       unsigned long long cs=rdtsc(); \
       (expr); \
       unsigned long long ce=rdtsc(); \
       double te=get_time(); \
       *(val) = te-ts; \
  } while (0)

