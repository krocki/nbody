#include <stdio.h>
#include "particle.h"

void particle_print(particle *p) {
  printf("particle %p: {\n"
           "\tidx=%06u, mass=%9.6f\n"
           "\tpos=[%9.6f, %9.6f, %9.6f]\n"
           "\tvel=[%9.6f, %9.6f, %9.6f]\n"
           "\tacc=[%9.6f, %9.6f, %9.6f]\n"
           "}\n",
           p, p->idx, p->mass,
           p->pos.x, p->pos.y, p->pos.z,
           p->vel.x, p->vel.y, p->vel.z,
           p->acc.x, p->acc.y, p->acc.z);
};
