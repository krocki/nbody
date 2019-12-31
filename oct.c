#include <stdio.h>
#include <stdlib.h>
#include "oct.h"

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
    n->a[0], n->b[0],
    n->a[1], n->b[1],
    n->a[2], n->b[2]);

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

}
