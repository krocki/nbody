#include <stdio.h>
#include <stdlib.h>
#include "oct.h"
#include "particle.h"
#include "timer.h"

extern float randf();
extern void
bh(unsigned n, unsigned steps,
   particle *p, float dt, float eps,
   oct_node *O);

int main(int argc, char **argv) {

  unsigned n = argc > 1 ? atoi(argv[1]) : 100;
  unsigned i = argc > 2 ? atoi(argv[2]) : 1;

  float dt  = 0.01f; /* time step */
  float eps = 1e-9f; /* softening */

  printf("num bodies=%u, iters=%i\n", n, i);

  particle *p = calloc(n, sizeof(particle));

  /* init */
  for (unsigned i=0; i<n; i++) {
    p[i].pos.x = 2.0f * randf() - 1.0f;
    p[i].pos.y = 2.0f * randf() - 1.0f;
    p[i].pos.z = 2.0f * randf() - 1.0f;
    p[i].mass = 1.0f; p[i].idx = i;
  }

  oct_node *root = NULL;
  root = oct_alloc();
  oct_reset(root);

  oct_insert(root, &p[0]);
  oct_insert(root, &p[1]);
  timeit(bh(n, i, p, dt, eps, root));

  oct_release(root);

  free(p);

  return 0;

}
