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
