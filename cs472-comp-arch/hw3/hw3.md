---
title: CS 472 HW 3
author:
  - Robert Detjens
  - '933604667'
  - Undergraduate
---

## Problem 1 [10 points]

You are building a computer where the memory system has a split L1 cache. Both
the I-cache and the D-cache hold 32 KB each. The I-cache has a 2% miss rate and
64 byte blocks, and the D-cache is a write-through, no-write-allocate cache with
a 5% miss rate and 64 byte blocks. The hit time for both the I-cache and the
D-cache is 1 ns.

The L2 cache is a unified write-back, write-allocate cache with a total size of
512 KB and a block size of 64-bytes. The hit time of the L2 cache is 15ns for
both read hits and write hits. Tag comparison for hit/miss is included in the
15ns in all cases, do not add hit time to miss time on a miss. The local hit
rate of the L2 cache is 80%. Also, 50% of all L2 cache blocks replaced are
dirty. The 64-bit wide main memory has an access latency of 20ns (including the
time for the request to reach from the L2 cache to the main memory), after which
any number of bus words may be transferred at the rate of one bus word (64-bit)
per bus cycle on the 64-bit wide 100 MHz main memory bus. Assume inclusion
between the L1 and L2 caches and assume there is no write- back buffer at the L2
cache. Assume a write-back takes the same amount of time as an L2 read miss of
the same size.

Assume all caches in the system are blocking; i.e., they can handle only one
memory access (load, store, or writeback) at a time. When calculating the miss
penalty for a load or store for a writeback cache, the time for any needed
writebacks should be included in the miss penalty.

While calculating any time values (such as hit time, miss penalty, AMAT), please
use ns (nanoseconds) as the unit of time. For miss rates below, give the local
miss rate for that cache. By miss penalty~L2~ , we mean the time from the
miss request issued by the L2 cache up to the time the data comes back to the L2
cache from main memory.

### Part A [7 points]

Computing the AMAT (average memory access time) for instruction accesses. (Note:
All the questions in Part A are only asking about instruction accesses. Do not
worry about data accesses.)

i. Give the values of the following terms for instruction accesses: [1 point]

    - hit time~L1~: 1ns
    - miss rate~L1~: 0.02
    - hit time~L2~: 15ns
    - miss rate~L2~: 0.20

ii. Give the formula for calculating miss penalty~L2~, and compute the value of
miss penalty L2. [4 points]

\begin{align*}
  MP_{L2} &= 20ns + 8 * \frac{1}{100 MHz} \\
          &= 20ns + 8 * 10ns \\
          &= 100ns \\
\end{align*}

iii. Give the formula for calculating the AMAT for this system using the five
terms whose values you computed above and any other values you need. [1 point]
(Hint: Miss penalty for L1 depends on how long on average it takes to get the
data back from L2)

\begin{align*}
  AMAT &= HT_{L1} + MR_{L1} * MP_{L1} \\
       &= HT_{L1} + MR_{L1} * (HT_{L2} + MR_{L2}*MP_{L2})
\end{align*}

iv. Plug in the values into the AMAT formula above, and compute a numerical
value for AMAT for instruction accesses. [1 point]

\begin{align*}
  AMAT &= HT_{L1} + MR_{L1} * (HT_{L2} + MR_{L2}*MP_{L2}) \\
       &= 1ns + 0.02 * (15ns + 0.20 * 100ns) \\
       &= 1.7ns \\
\end{align*}

### Part B [3 points]

Computing the AMAT for data reads. (Note: Part B is only asking about data
reads. Do not worry about behavior during a data write)

i. Give the value of miss rate~L1~ for data reads. [1 point]

    - miss rate~L1~: 0.05

ii. Calculate the value of the AMAT for data reads using the above value, and
other values you need. [2 point]

\begin{align*}
  AMAT &= HT_{L1} + MR_{L1} * (HT_{L2} + MR_{L2}*MP_{L2}) \\
       &= 1ns + 0.05 * (15ns + 0.20 * 100ns) \\
       &= 2.75ns \\
\end{align*}

## Problem 2 [30 points]

A processor accesses a 256 byte cache, with a block size of 64 bytes, with the
following sequence of 32-bit addresses.

- `0xFF0057F2`
- `0xFF002311`
- `0xEA022000`
- `0xEA022011`
- `0xFF006682`
- `0x00000000`
- `0xFF0057EA`
- `0xEA022015`

For the three different cache organizations  (direct mapped, fully associative
and 2-way set associative)  show the contents of the cache after each access,
noting the hits/misses  and replacements and when they occur. LRU replacement
should be your replacement scheme when necessary. If a cache entry has been
replaced, please note the address of the block that was replaced. If there are
no index bits, leave the column blank.  You may assume these caches are empty as
their initial state. Tables  have been provided for your convenience.

### Part A: Direct Mapped Cache [10 points]

.

| Address    | Tag       | Index | Block Offset | Hit/Miss Replace? |
|------------|-----------|-------|--------------|-------------------|
| `ff0057f2` | `1fe00af` | `111` | `110010`     | Miss              |
| `ff002311` | `1fe0046` | `000` | `010001`     | Miss              |
| `ea022000` | `1d40440` | `000` | `000000`     | Replace           |
| `ea022011` | `1d40440` | `000` | `010001`     | Replace           |
| `ff006682` | `1fe00cd` | `100` | `000010`     | Miss              |
| `00000000` | `0000000` | `000` | `000000`     | Replace           |
| `ff0057ea` | `1fe00af` | `111` | `101010`     | Replace           |
| `ea022015` | `1d40440` | `000` | `010101`     | Replace           |

### Part B: Fully Associative Cache [10 points]

| Address    | Tag        | Index | Block Offset | Hit/Miss Replace? |
|------------|------------|-------|--------------|-------------------|
| `ff0057f2` | `07f802bf` |       | `110010`     | Miss              |
| `ff002311` | `07f80118` |       | `010001`     | Miss              |
| `ea022000` | `07501100` |       | `000000`     | Miss              |
| `ea022011` | `07501100` |       | `010001`     | Miss              |
| `ff006682` | `07f80334` |       | `000010`     | Miss              |
| `00000000` | `00000000` |       | `000000`     | Miss              |
| `ff0057ea` | `07f802bf` |       | `101010`     | Miss              |
| `ea022015` | `07501100` |       | `010101`     | Miss              |

### Part C: 2-Way Set Associative Cache [10 points]

| Address    | Tag       | Index | Block Offset | Hit/Miss Replace? |
|------------|-----------|-------|--------------|-------------------|
| `ff0057f2` | `3fc015f` | `1`   | `110010`     | Miss              |
| `ff002311` | `3fc008c` | `0`   | `010001`     | Miss              |
| `ea022000` | `3a80880` | `0`   | `000000`     | Miss              |
| `ea022011` | `3a80880` | `0`   | `010001`     | Replace           |
| `ff006682` | `3fc019a` | `0`   | `000010`     | Replace           |
| `00000000` | `0000000` | `0`   | `000000`     | Replace           |
| `ff0057ea` | `3fc015f` | `1`   | `101010`     | Miss              |
| `ea022015` | `3a80880` | `0`   | `010101`     | Replace           |
