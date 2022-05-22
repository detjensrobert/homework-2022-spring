---
title: CS 472 HW 4
author:
  - Robert Detjens
  - '933604667'
  - Undergraduate
---

## Problem 1 [10 points]

> Consider the following loop which takes 8 cycles to execute one iteration
> (assuming cache hits).
>
> ```c
> for (i = 0; i < 562; i++) {
>   A[i] = c * A[i];
> }
> ```
>
> The penalty for a cache miss is 64 cycles. Each element of array A is 4 bytes.

### Part (a) [2 points]

> Assuming a cache block size of 4 bytes, show the above loop with compiler
> inserted prefetch instructions (as seen in class).

I assume the integer type is also 4 bytes / 32 bits, since MIPS works on 32-bit
words, so each element is one cache block.

$$k = \frac{miss\ penalty}{iter\ time} = \frac{64\ cyc}{8\ cyc/iter} = 8\ iters\ ahead$$

```c
for (i = 0; i < 562; i++) {
  prefetch(x[i+8]);
  A[i] = c * A[i];
}
```

### Part (b) [2 points]

> Now assume that the cache block size is 16 bytes (assume aligned cache blocks
> i.e., a cache block starts at `A[0]`). Show the loop with compiler inserted
> prefetches.

4 elements fit in one cache block, so only need to get the next line every 4
elements. As the number of iterations of this loop is divisible by four, the
loop can be partially unrolled to use one prefetch per 4 accesses.

```c
for (i = 0; i < 562; i += 4) {
  prefetch(x[i+8]);
  A[i]   = c * A[i];
  A[i+1] = c * A[i+1];
  A[i+2] = c * A[i+2];
  A[i+3] = c * A[i+3];
}
```

However, the prefetch from **1(a)** would still work, as the first prefetch for
the block would cause a miss (as expected), and the subsequent three prefetches
would just hit the already-prefetched block and would do nothing, which is still
valid.

\pagebreak

## Problem 2 [6 points]

> Consider a tiny system with virtual memory. Physical addresses are 8 bits
> long, but only $2^7 = 128$ bytes of physical memory is installed, at physical
> addresses 0 up to 127. Pages are $2^4 = 16$ bytes long. Virtual addresses are
> 10 bits long. An exception is raised if a program accesses a virtual address
> whose virtual page has no mapping in the page table, or is mapped to a
> physical page outside of installed physical memory.
>
> Here are the contents of main memory. To find the physical address of a byte,
> read the least significant digit from the column label and the most
> significant digit from the row label. For example, the shaded byte in the
> second row is at physical address `0x12`. All entries are in hexadecimal.

|        | `0`  | `1`  |   `2`    | `3`  |    `4` | `5`  | `6`  | `7`    |    `8` | `9`  | `A`  | `B`    |    `C` | `D`  | `E`  | `F`    |
|--------|------|------|:--------:|------|-------:|------|------|--------|-------:|------|------|--------|-------:|------|------|--------|
| `0x0_` | `4e` | `65` |   `76`   | `65` |   `72` | `20` | `67` | `6f`   |   `6e` | `6e` | `61` | `20`   |   `67` | `69` | `76` | `65`   |
| `0x1_` | `20` | `79` | [ `6f` ] | `75` |   `20` | `75` | `70` | `0a`   |   `4e` | `65` | `76` | `65`   |   `72` | `20` | `67` | `6f`   |
| `0x2_` | `6e` | `6e` |   `61`   | `20` | [ `6c` | `65` | `74` | `20` ] |   `79` | `6f` | `75` | `20`   |   `64` | `6f` | `77` | `6e`   |
| `0x3_` | `0a` | `4e` |   `65`   | `76` |   `65` | `72` | `20` | `67`   |   `6f` | `6e` | `6e` | `61`   |   `20` | `72` | `75` | `6e`   |
| `0x4_` | `20` | `61` |   `72`   | `6f` |   `75` | `6e` | `64` | `20`   | [ `61` | `6e` | `64` | `20` ] |   `64` | `65` | `73` | `65`   |
| `0x5_` | `72` | `74` |   `20`   | `79` |   `6f` | `75` | `0a` | `4e`   |   `65` | `76` | `65` | `72`   |   `20` | `67` | `6f` | `6e`   |
| `0x6_` | `6e` | `61` |   `20`   | `6d` |   `61` | `6b` | `65` | `20`   |   `79` | `6f` | `75` | `20`   | [ `63` | `72` | `79` | `0a` ] |
| `0x7_` | `4e` | `65` |   `76`   | `65` |   `72` | `20` | `67` | `6f`   |   `6e` | `6e` | `61` | `20`   |   `73` | `61` | `79` | `20`   |

| Virtual page | Physical page |
|--------------|---------------|
| `000000`     | `0x2`         |
| `000001`     | `0x4`         |
| `000010`     | `0x1`         |
| `000011`     | `0x5`         |
| `000100`     | `0x4`         |
| `000101`     | `0x7`         |
| `000110`     | `0x9`         |

### [1 point for each question]

A) List the four bytes in the word beginning at physical address `0x34`.

    `0x 65 72 20 67`

B) How many virtual addresses refer to the first byte of the shaded word in row
`0x2_`? List them.

    1. `000000 0100` / `0x004`

C) How many virtual addresses refer to the first byte of the shaded word in row
`0x4_`? List them.

    1. `000001 1000` / `0x018`
    2. `000100 1000` / `0x048`

D) How many virtual addresses refer to the first byte of the shaded word in row
`0x6_`? List them.

    No virtual addresses are mapped to page `0x6`.

E) What data is returned if the program loads a word from virtual address
`0x05C` (`000101 1100`)?

    `0x05c` virtual -> `0x7C` physical = `0x73`

F) What is the result if the program loads a word from virtual address `0x064`
(`000110 0010`)?

    `0x064` virtual -> `0x94` physical = page fault exception, address is
    outside physical memory

\pagebreak

## Problem 3 [4 points]

> Consider a system with the following processor components and policies:

- A direct-mapped L1 data cache of size 4KB ($4 * 2^{10}$ bytes) and block size
  of 16 bytes, indexed and tagged using physical addresses, and using a
  write-allocate, write-back policy
- A fully-associative data TLB with 4 entries and an LRU replacement policy
- Physical addresses of 32 bits, and virtual addresses of 40 bits
- Byte addressable memory
- Page size of 1MB

### Part A [2 points]

> Which bits of the virtual address are used to obtain a virtual to physical
> translation from the TLB? Explain exactly how these bits are used to make the
> translation, assuming there is a TLB hit.



### Part B [2 points]

> Which bits of the virtual or physical address are used as the tag, index, and
> block offset bits for accessing the L1 data cache? Explicitly specify which of
> these bits can be used directly from the virtual address without any
> translation.
