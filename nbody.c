#include <stdlib.h>
#include <math.h>
//extern float __rsqrt(float);

void sim(const unsigned n,
         const unsigned steps,
         float *x,  float *y,  float *z,
         float *vx, float *vy, float *vz,
         float *m,  float dt,  float eps) {

  unsigned iters = 0;

  for (; iters<steps; iters++) {

    /* outer */
    for (unsigned i=0; i<n; i++) {

      float Fx = 0.0f;
      float Fy = 0.0f;
      float Fz = 0.0f;
      float xi = x[i];
      float yi = y[i];
      float zi = z[i];

      /* inner */
      /* compute forces Fx, Fy, Fz */
      for (unsigned j=0; j<n; j++) {

        /* 3 */
        float dx = x[j] - xi;
        float dy = y[j] - yi;
        float dz = z[j] - zi;
        /* 6 */
        float dist = eps + dx*dx + dy*dy + dz*dz;
        /* 2 */
        float inv = 1.0f / sqrtf(dist);
        /* 2 */
        float inv3 = inv*inv*inv;
        /* 1 */
        float s = m[j] * inv3;
        /* 6 */
        Fx += dx * s;
        Fy += dy * s;
        Fz += dz * s;
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
    }
  }
}
