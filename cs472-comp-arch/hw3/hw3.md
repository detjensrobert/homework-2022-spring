# CS 472 HW 3

## Robert Detjens

---

## Problem 1 [10 points]

You are building a computer where the memory system has a split L1 cache. Both the I-cache and the D-cache hold 32 KB
each. The I-cache has a 2% miss rate and 64 byte blocks, and the D-cache is a write-through, no-write-allocate cache
with a 5% miss rate and 64 byte blocks. The hit time for both the I-cache and the D-cache is 1 ns.

The L2 cache is a unified write-back, write-allocate cache with a total size of 512 KB and a block size of 64-bytes. The
hit time of the L2 cache is 15ns for both read hits and write hits. Tag comparison for hit/miss is included in the 15ns
in all cases, do not add hit time to miss time on a miss. The local hit rate of the L2 cache is 80%. Also, 50% of all L2
cache blocks replaced are dirty. The 64-bit wide main memory has an access latency of 20ns (including the time for the
request to reach from the L2 cache to the main memory), after which any number of bus words may be transferred at the
rate of one bus word (64-bit) per bus cycle on the 64-bit wide 100 MHz main memory bus. Assume inclusion between the L1
and L2 caches and assume there is no write- back buffer at the L2 cache. Assume a write-back takes the same amount of
time as an L2 read miss of the same size.

Assume all caches in the system are blocking; i.e., they can handle only one memory access (load, store, or writeback)
at a time. When calculating the miss penalty for a load or store for a writeback cache, the time for any needed
writebacks should be included in the miss penalty.

While calculating any time values (such as hit time, miss penalty, AMAT), please use ns (nanoseconds) as the unit of
time. For miss rates below, give the local miss rate for that cache. By miss penalty$_{L2}$ , we mean the time from the miss
request issued by the L2 cache up to the time the data comes back to the L2 cache from main memory.

### Part A [7 points]

Computing the AMAT (average memory access time) for instruction accesses. (Note: All the questions in Part A are only
asking about instruction accesses. Do not worry about data accesses.)

i. Give the values of the following terms for instruction accesses: hit time $_{L1}$, miss rate $_{L1}$, hit time
$_{L2}$, miss rate $_{L2}$. [1 point]

ii. Give the formula for calculating miss penalty$_{L2}$ , and compute the value of miss penalty L2. [4 points]

iii. Give the formula for calculating the AMAT for this system using the five terms whose values you computed above and
any other values you need. [1 point] (Hint: Miss penalty for L1 depends on how long on average it takes to get the data
back from L2)

iv. Plug in the values into the AMAT formula above, and compute a numerical value for AMAT for instruction accesses. [1
point]

### Part B [3 points]

Computing the AMAT for data reads. (Note: Part B is only asking about data reads. Do not worry about behavior during a
data write)

i. Give the value of miss rateL1 for data reads. [1 point]

ii. Calculate the value of the AMAT for data reads using the above value, and other values you need. [2 point]

## Problem 2 [30 points]

A processor accesses a 256 byte cache, with a block size of 64 bytes, with the following sequence of 32-bit addresses.
