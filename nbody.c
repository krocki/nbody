#include <math.h>
#include <stdlib.h>

extern float randf();

void sim(unsigned n, unsigned steps) {

  float *x = calloc(n, sizeof(float));
  float *y = calloc(n, sizeof(float));
  float *z = calloc(n, sizeof(float));
  float *vx = calloc(n, sizeof(float));
  float *vy = calloc(n, sizeof(float));
  float *vz = calloc(n, sizeof(float));
  float *m = calloc(n, sizeof(float));

  float dt = 0.001f;
  float eps = 1e-6f;

  unsigned iters = 0;
  /* init */
  for (unsigned i=0; i<n; i++) {
    x[i] = 2.0f * randf() - 1.0f;
    y[i] = 2.0f * randf() - 1.0f;
    z[i] = 2.0f * randf() - 1.0f;
    vx[i] = 0.0f; vy[i] = 0.0f; vz[i] = 0.0f;
    m[i] = 1.0f;
    //printf("%3zu %3zu: p={%10.6f,%10.6f,%10.6f}, v={%10.6f,%10.6f,%10.6f}\n", iters, i, x[i], y[i], z[i], vx[i], vy[i], vz[i]);
  }
  for (; iters<steps; iters++) {
    //puts("---");
    for (unsigned i=0; i<n; i++) {
      float Fx = 0.0f; float Fy = 0.0f; float Fz = 0.0f;
      for (unsigned j=0; j<n; j++) {
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
    for (unsigned i=0; i<n; i++) {
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
