#ifndef PARTICLE_H
#define PARTICLE_H

struct float3 {
  float x, y, z;
};

typedef union {
  float v[3];
  struct {
    float x, y, z;
  };
} float3;

typedef struct {
  float3 pos;
  float3 vel;
  float3 acc;
  float mass;
  unsigned idx;
} particle;

extern void particle_print(particle *p);

#endif
