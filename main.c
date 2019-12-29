#include <stdio.h>
#include <stdlib.h>
#include "timer.h"

#define STATIC_MEM 1

extern float randf();
extern void
sim(unsigned n, unsigned steps,
    float *x,  float *y,  float *z,
    float *vx, float *vy, float *vz,
    float *m,  float dt,  float eps);

int main(int argc, char **argv) {

  unsigned n = argc > 1 ? atoi(argv[1]) : 10000;
  unsigned i = argc > 2 ? atoi(argv[2]) : 10;

  float dt  = 0.01f; /* time step */
  float eps = 1e-9f; /* softening */

  printf("num bodies=%u, iters=%i\n", n, i);

#if STATIC_MEM
  float  x[n]; float  y[n]; float  z[n];
  float vx[n]; float vy[n]; float vz[n];
  float  m[n];
#else
  float *x =  calloc(n, sizeof(float));
  float *y =  calloc(n, sizeof(float));
  float *z =  calloc(n, sizeof(float));
  float *vx = calloc(n, sizeof(float));
  float *vy = calloc(n, sizeof(float));
  float *vz = calloc(n, sizeof(float));
  float *m =  calloc(n, sizeof(float));
#endif

  /* init */
  for (unsigned i=0; i<n; i++) {
    x[i] = 2.0f * randf() - 1.0f;
    y[i] = 2.0f * randf() - 1.0f;
    z[i] = 2.0f * randf() - 1.0f;
    vx[i] = 0.0f; vy[i] = 0.0f; vz[i] = 0.0f;
    m[i] = 1.0f;
  }

  double t;
  timeit2(sim(n, i, x,  y,  z,
          vx, vy, vz, m, dt, eps), &t);

  double mflops = 18 * (double)(n * n * i) / (1 << 20);

  printf("mflops: %.3f, time: %8.4f s\n"
         "time/iteration: %10.6f s\n%5.2f mflop/s, "
         "%5.3f M interactions/s\n",
         mflops, t, t/i, mflops/t,
         (n * n)/(t * (1 << 20)));

#if STATIC_MEM
#else
  free(x), free(y), free(z);
  free(vx), free(vy), free(vz);
  free(m);
#endif

  return 0;

}
