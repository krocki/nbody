typedef struct {
  float x;
  float y;
  float z;
} vec3f;

typedef struct {
  float x;
  float y;
  float z;
  float w;
} vec4f;

#define epssq 1e-5f

/*
float __rsqrt(float x) {
  float xhalf = 0.5f;
  int i = *(int*) &x;
  i = 0x5f3759df - (i >> 1);
  x = *(float*) &i;
  x = x * (1.5f - (xhalf * x * x));
  return x;
}
*/

float __rsqrt(float x) {
  float y;
  __asm__ volatile
    (
     "rsqrtss %[x], %%xmm0;"
     "movss %%xmm0, %[y];"
     :
     : [x] "m" (x), [y] "m" (y)
     : "xmm0"
    );
  return y;
}

void inner(vec3f *ai, vec4f *bi, vec4f *bj) {

  vec3f r;

  // r_ij 3F
  r.x = bj->x - bi->x;
  r.y = bj->y - bi->y;
  r.z = bj->z - bi->z;

  // dot(r_ij, r_ij) + eps^2, 6F
  float d = r.x * r.x + r.y * r.y + r.z * r.z + epssq;

  // 4F
  float invd = __rsqrt(d);
  float invd3 = invd * invd * invd;

  // 1F, mass
  float s = bj->w * invd3;

  // 6F
  ai->x += r.x * s;
  ai->y += r.y * s;
  ai->z += r.z * s;

}

// K steps
void outer(vec4f *p, vec3f *v, vec3f *a, float damping, float dt, int N, int K) {

  for (int k=0; k<K; k++) {
    for (int i=0; i<N; i++) {

      a[i] = (vec3f){0, 0, 0};

      for (int j=0; j<N; j++) {
        inner(&a[i], &p[i], &p[j]);
      }

      v[i].x = v[i].x * damping + a[i].x * dt;
      v[i].y = v[i].y * damping + a[i].y * dt;
      v[i].z = v[i].z * damping + a[i].z * dt;

      p[i].x += v[i].x * dt;
      p[i].y += v[i].y * dt;
      p[i].z += v[i].z * dt;
    }
  }
}

