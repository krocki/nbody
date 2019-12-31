#include <stdio.h>
#include "particle.h"

int particle_in_range(particle *p,
  float *a, float *b) {

  return (
    (p->pos.x > a[0] &&
       p->pos.x <= b[0]) &&
    (p->pos.y > a[1] &&
       p->pos.y <= b[1]) &&
    (p->pos.z > a[2] &&
       p->pos.z <= b[2])
  );
}

void particle_print(particle *p) {
  printf("particle %p: "
           "idx=%05d, "
           "pos=[%9.6f, %9.6f, %9.6f]"
           "\n",
           (void*)p, p->idx, p->pos.x, p->pos.y, p->pos.z);
           //"\tvel=[%9.6f, %9.6f, %9.6f]\n"
           //"\tacc=[%9.6f, %9.6f, %9.6f]\n"
           //p->vel.x, p->vel.y, p->vel.z,
           //p->acc.x, p->acc.y, p->acc.z
}
