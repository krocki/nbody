#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "oct.h"

void bh(const unsigned n,
        const unsigned steps,
        particle *p,
        float dt,
        float eps,
        oct_node *o) {

  float theta = 0.5f;

  unsigned iters = 0;
  for (; iters<steps; iters++) {

    for (unsigned i=0; i<n; i++) {

      float3 F = {0, 0, 0};
      unsigned long ops = 0;
      forces(&F, &ops, o, &p[i], theta, eps);

      //printf("p[%u], F = %f %f %f, ops=%lu\n",
      //i, F.x, F.y, F.z, ops);

      p[i].vel.x += dt*F.x;
      p[i].vel.y += dt*F.y;
      p[i].vel.z += dt*F.z;

    }

    for (unsigned i=0; i<n; i++) {

      p[i].pos.x += dt*p[i].vel.x;
      p[i].pos.y += dt*p[i].vel.y;
      p[i].pos.z += dt*p[i].vel.z;

    }

  }
}
