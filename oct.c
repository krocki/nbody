#include <stdio.h>
#include <stdlib.h>
#include "oct.h"

const char* inverse_id[8] =
{
  "FSW", "FSE", "FNW", "FSW",
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
    "[x %1.3f:%1.3f] "
    "[y %1.3f:%1.3f] "
    "[z %1.3f:%1.3f]\n",
    n,
    n->a[X], n->b[X],
    n->a[Y], n->b[Y],
    n->a[Z], n->b[Z]);
    if (n->p) particle_print(n->p);

    for (int i=0; i<CAP; i++)
      oct_print(n->c[i]);

  }
}

void oct_reset(oct_node *n) {

  if (0 != n) {
    n->a[X] = n->a[Y] = n->a[Z] = -1.0f;
    n->b[X] = n->b[Y] = n->b[Z] = 1.0f;
  } else
    puts("oct_reset: n is NUL");

}

void oct_insert(oct_node *n, particle *p) {

  puts("oct_insert");
  particle_print(p);

  oct_print(n);
  float m[3];
  unsigned idx[3];
  unsigned idx_final = 0;
  for (int i=0; i<3; i++) {
    m[i] = n->a[i] + (n->b[i] - n->a[i]) / 2.0f;
    idx[i] = (p->pos.v[i] > m[i]);
    printf("m[%d]=%f, idx[%d]=%d\n",
      i, m[i], i, p->pos.v[i] >=m[i]);
    idx_final |= (idx[i]) << i;
  }
  printf("idx final = %u (%s)\n",
  idx_final, inverse_id[idx_final]);


  if (NULL != n->c[idx_final])
    oct_insert(n->c[idx_final], p);
  else {
    /* n->c[idx] is null */
    oct_node *q = n->c[idx_final] = oct_alloc();
    q->p = p;
    for (int i=0; i<3; i++) {
      if (idx[i] < 1) {
        q->a[i] = n->a[i]; q->b[i] = m[i];
      } else {
        q->b[i] = n->b[i]; q->a[i] = m[i];
      }
    }
  }
}
