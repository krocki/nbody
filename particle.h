#ifndef PARTICLE_H
#define PARTICLE_H

typedef struct {
  float x, y, z;
} float3;

typedef struct {
  float3 pos;
  float3 vel;
  float3 acc;
  float mass;
} particle;

void particle_print(particle *p) {

}

#endif
