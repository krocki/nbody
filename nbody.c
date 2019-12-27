#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "timer.h"

float randf() {
  return 2.0f * (rand() / (float)RAND_MAX) - 1.0f;
}

void sim(size_t n, size_t steps) {

  float *x = calloc(n, sizeof(float));
  float *y = calloc(n, sizeof(float));
  float *z = calloc(n, sizeof(float));
  float *vx = calloc(n, sizeof(float));
  float *vy = calloc(n, sizeof(float));
  float *vz = calloc(n, sizeof(float));
  float *m = calloc(n, sizeof(float));

  float dt = 0.001f;
  float eps = 1e-6f;

  size_t iters = 0;
  /* init */
  for (size_t i=0; i<n; i++) {
    x[i] = randf(); y[i] = randf(); z[i] = randf();
    vx[i] = 0.0f; vy[i] = 0.0f; vz[i] = 0.0f;
    m[i] = 1.0f;
    //printf("%3zu %3zu: p={%10.6f,%10.6f,%10.6f}, v={%10.6f,%10.6f,%10.6f}\n", iters, i, x[i], y[i], z[i], vx[i], vy[i], vz[i]);
  }
  for (; iters<steps; iters++) {
    //puts("---");
    for (size_t i=0; i<n; i++) {
      float Fx = 0.0f; float Fy = 0.0f; float Fz = 0.0f;
      for (size_t j=0; j<n; j++) {
        float dx = x[j] - x[i];
        float dy = y[j] - y[i];
        float dz = z[j] - z[i];
        float dist = eps + dx*dx + dy*dy + dz*dz;
        float inv = 1.0f / sqrtf(dist);
        float inv3 = inv*inv*inv;
        Fx += dx * inv3;
        Fy += dy * inv3;
        Fz += dz * inv3;
      }
      vx[i] += dt*Fx;
      vy[i] += dt*Fy;
      vz[i] += dt*Fz;
    }

    /* integrate */
    for (size_t i=0; i<n; i++) {
      x[i] += vx[i]*dt;
      y[i] += vy[i]*dt;
      z[i] += vz[i]*dt;
      //printf("%3zu %3zu: p={%10.6f,%10.6f,%10.6f}, v={%10.6f,%10.6f,%10.6f}\n", iters, i, x[i], y[i], z[i], vx[i], vy[i], vz[i]);
    }
  }

  free(x), free(y), free(z);
  free(vx), free(vy), free(vz);
  free(m);
}

int main(int argc, char **argv) {

  size_t n = argc > 1 ? atoi(argv[1]) : 100;
  size_t i = argc > 2 ? atoi(argv[2]) : 1;

  timeit(sim(n, i));
  printf("total mflops: %.3f\n", 18 * (double)(n * n * i) / (1024.0 * 1024.0));
  return 0;

}
