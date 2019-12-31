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

        float dx = x[j] - xi;
        float dy = y[j] - yi;
        float dz = z[j] - zi;
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
    }
  }
}
