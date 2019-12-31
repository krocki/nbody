#ifndef OCT_H
#define OCT_H

#define CAP 8

#define FNW 0
#define FNE 1
#define FSW 2
#define FSE 3
#define BNW 4
#define BNE 5
#define BSW 6
#define BSE 7

#define X 0
#define Y 1
#define Z 2

typedef struct {
  float x, y, z;
} float3;

typedef struct {
  float3 pos;
  float3 vel;
  float3 acc;
  float mass;
} particle;

typedef struct oct_node {

  struct oct_node *c[8];
  float a[3], b[3];

} oct_node;

extern oct_node *oct_alloc(void);
extern void oct_release(oct_node *n);
extern void oct_print(oct_node *n);
extern void oct_reset(oct_node *n);
extern void oct_insert(oct_node *n, particle *p);

#endif
