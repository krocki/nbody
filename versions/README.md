# version a

# avx (-Ofast -fPIC -fomit-frame-pointer -std=c99 -Wfatal-errors -mavx):
```
num bodies=10000, iters=10
mflops: 17166.138, time:   0.5318 s
time/iteration:   0.053176 s
32281.68 mflop/s, 179.343 M interactions/s
```

# avx2 (-Ofast -fPIC -fomit-frame-pointer -std=c99 -Wfatal-errors -mavx2):
```
num bodies=10000, iters=10
mflops: 17166.138, time:   0.5495 s
time/iteration:   0.054954 s
31237.23 mflop/s, 173.540 M interactions/s
```
