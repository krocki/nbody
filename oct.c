#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "oct.h"

const char* inverse_id[8] =
{
  "FSW", "FSE", "FNW", "FNE",
  "BSW", "BSE", "BNW", "BNE"
};

oct_node *oct_alloc() {
  oct_node *n = calloc(1, sizeof(oct_node));
  return n;
}

void oct_release(oct_node *n) {

  if (0 != n) {
    for (int i=0; i<CAP; i++)
      oct_release(n->c[i]);

    free(n), n = 0;
  }

}

void oct_print(oct_node *n) {

  if (0 != n) {
    for (unsigned i=0; i<n->level; i++)
      printf("  ");
    printf("[%d %s] # %lu %s "
    "\t[x %6.3f:%6.3f] "
    "[y %6.3f:%6.3f] "
    "[z %6.3f:%6.3f] (m=%3.1f, c=%2.1f, %2.1f, %2.1f), %3d in=%d\n",
    n->id, inverse_id[n->id],
    n->bodies, n->bodies == 1 ? "*" :
    n->bodies == 0 ?  "!" : "  ",
    n->a[X], n->b[X],
    n->a[Y], n->b[Y],
    n->a[Z], n->b[Z],
    n->mass_total, n->mass_center[0],
    n->mass_center[1], n->mass_center[2],
    (n->p != 0) ? (int)n->p->idx : -1,
    (n->p != 0) ?
    particle_in_range(n->p, n->a, n->b) : -1);

    for (int i=0; i<CAP; i++) {
      oct_print(n->c[i]);
    }

  }
}

void oct_reset(oct_node *n) {

  if (0 != n) {
    n->a[X] = n->a[Y] = n->a[Z] = -1.0f;
    n->b[X] = n->b[Y] = n->b[Z] = 1.0f;
    n->bodies = 0; n->p = 0;
    n->mass_total = 0;
    for (int i=0; i<3; i++) n->mass_center[i] = 0.0f;
    for (int i = 0; i<CAP; i++)
      oct_release(n->c[i]);
  } else
    puts("oct_reset: n is NUL");

}

void oct_insert(oct_node *n, particle *p) {

  n->bodies++;
  n->mass_total += p->mass;
  for (int i=0; i<3; i++) {
    n->mass_center[i] =
    ((n->mass_center[i] * (n->bodies-1)) +
    (p->pos.v[i]*p->mass)) / (float)(n->bodies);
  }

  float m[3];
  unsigned idx[3];
  unsigned idx_final = 0;
  for (int i=0; i<3; i++) {
    m[i] = n->a[i] + (n->b[i] - n->a[i]) / 2.0f;
    idx[i] = (p->pos.v[i] > m[i]);
    idx_final |= (idx[i]) << i;
  }

  if (NULL != n->c[idx_final])
    oct_insert(n->c[idx_final], p);
  else {
    /* n->c[idx] is null */
    oct_node *q = n->c[idx_final] = oct_alloc();
    q->parent = n; q->id = idx_final;
    q->p = p; q->level = n->level+1;
    q->bodies = 1;

    q->mass_total = p->mass;
    for (int i=0; i<3; i++)
      q->mass_center[i] = p->mass * p->pos.v[i];

    for (int i=0; i<3; i++) {
      if (idx[i] < 1) {
        q->a[i] = n->a[i]; q->b[i] = m[i];
      } else {
        q->b[i] = n->b[i]; q->a[i] = m[i];
      }
    }
    /* if parent node had a particle */
    /* move it to another sub oct tree */
    if (NULL != n->p) {
      particle *t = n->p;
      n->p = NULL;
      n->bodies--;
      oct_insert(n, t);
    }
  }
}

void forces(float3 *F, unsigned long *ops, oct_node *o, particle *p, float theta, float eps) {

  if (NULL == o) return;

  float dx = o->mass_center[0] - p->pos.x;
  float dy = o->mass_center[1] - p->pos.y;
  float dz = o->mass_center[2] - p->pos.z;

  float s = o->b[0] - o->a[0];
  float d = sqrtf(eps + dx*dx + dy*dy + dz*dz);
  float ratio = s/d;

  if (ratio <= theta || o->bodies==1) {
    if (o->p != p) { /* skip self */
      float inv = 1.0f / d;
      float inv3 = o->mass_total * inv*inv*inv;
      F->x += dx * inv3;
      F->y += dy * inv3;
      F->z += dz * inv3;
      (*ops)++;
    }
  }
  else
  {
    for (int i = 0; i<CAP; i++) {
      forces(F, ops, o->c[i], p, theta, eps);
    }
  }
}
