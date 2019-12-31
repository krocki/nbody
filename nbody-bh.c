#include <stdio.h>
#include <stdlib.h>
#include "oct.h"

void bh(const unsigned n,
        const unsigned steps,
        particle *p,
        float eps,
        oct_node *O) {

  puts(">>>bh");

  oct_print(O);

  puts("<<<bh");
}
