#ifndef OCT_H
#define OCT_H

#include "particle.h"

#define CAP 8

#define FSW 0
#define FSE 1
#define FNW 2
#define FNE 3
#define BSW 4
#define BSE 5
#define BNW 6
#define BNE 7

#define X 0
#define Y 1
#define Z 2

typedef struct oct_node {

  struct oct_node *c[8];
  struct oct_node *parent;
  float a[3], b[3];
  particle *p;
  unsigned char id;
} oct_node;

extern oct_node *oct_alloc(void);
extern void oct_release(oct_node *n);
extern void oct_print(oct_node *n);
extern void oct_reset(oct_node *n);
extern void oct_insert(oct_node *n, particle *p);

#endif
