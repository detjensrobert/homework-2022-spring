# CS 475 Project 0: Simple OpenMP

## Robert Detjens

---

### What machine was this ran on?

This was done on an Intel i7-1165G7 running Fedora Linux.

### What performance results did you get?

```{.run cmd="bash" in="script" out="text"}
echo "Single-threaded: $(cat out.1t) MMults/s"
echo "  Quad-threaded: $(cat out.4t) MMults/s"
```

### What was your 4-thread-to-one-thread speedup?

```{.run cmd="python" in="script" out="text"}
t1 = float(open('out.1t').read().strip())
t4 = float(open('out.4t').read().strip())

sf = t4 / t1

print(f"Speedup factor: {round(sf, 3)}")
```

### If the 4-thread-to-one-thread speedup is less than 4.0, why do you think it is this way?

Not all of the code is parallelized, such as the array initialization or doing the timings. This part always runs
linearly regardless of how many threads OpenMP is using, and will not benefit from any speed-up. Additionally, multi-threaded loads may cause the processor to not boost as high.

### What was your Parallel Fraction, $Fp$?

```{.run cmd="python" in="script" out="text"}
t1 = float(open('out.1t').read().strip())
t4 = float(open('out.4t').read().strip())

sf = t4 / t1
pf = 4.0 / 3.0 * (1 - 1.0 / sf)

print(f"Parallel fraction: {round(pf, 3)}")
```
