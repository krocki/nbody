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
    printf("oct_node: %p, "
    "bodies=%lu\n"
    "id %d (%s), parent %p\n"
    "\t[x %6.3f:%6.3f]\n"
    "\t[y %6.3f:%6.3f]\n"
    "\t[z %6.3f:%6.3f]\n",
    (void*)n, n->bodies,
    n->id,
    inverse_id[n->id],
    (void*)n->parent,
    n->a[X], n->b[X],
    n->a[Y], n->b[Y],
    n->a[Z], n->b[Z]);

    if (0 != n->p) particle_print(n->p);

    for (int i=0; i<CAP; i++)
      oct_print(n->c[i]);

  }
}

void oct_reset(oct_node *n) {

  if (0 != n) {
    n->a[X] = n->a[Y] = n->a[Z] = -1.0f;
    n->b[X] = n->b[Y] = n->b[Z] = 1.0f;
    n->bodies = 0; n->p = 0;
    for (int i = 0; i<CAP; i++)
      oct_release(n->c[i]);
  } else
    puts("oct_reset: n is NUL");

}

void oct_insert(oct_node *n, particle *p) {

  n->bodies++;

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
    q->p = p; q->bodies = 1;
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
