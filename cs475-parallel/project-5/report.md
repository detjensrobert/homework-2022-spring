---
header-includes:
  - \usepackage{pgfplots}
  - \usepackage{pgfplotstable}
  - \pgfplotsset{compat=1.18}
  - \usepackage{float}
  - \makeatletter
  - \def\fps@figure{H}
  - \makeatother

title: 'CS 475 Project 5: CUDA Monte Carlo Simulation'
author:
  - Robert Detjens
  - detjensr@oregonstate.edu
---

## Runtime information

These tests were initially run on `rabbit`, and were later re-run on the HPC
cluster. The displayed data is from the HPC cluster,


## Data

```table
---
include: results.csv
caption: CUDA Monte Carlo Simulation Performances
---
```

\newcommand{\trials}{1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576,2097152,4194304,8388608,16777216,33554432}
\newcommand{\tlabel}{1K  ,    ,    ,8K  ,     ,     ,64K  ,      ,      ,      ,1M     ,       ,       ,8M     ,        ,32M     }

\newcommand{\blocksizes}{8,32,128}

\pgfplotsset{
  axis lines = left,
  grid = major,
  grid style = {dashed,gray!30},
  legend pos = outer north east,
  legend cell align = left,
  scaled ticks = false,
  log ticks with fixed point,
}

\begin{figure}[H]
  \centering
  \begin{tikzpicture}
    \begin{axis}[
      xmode = log,
      xlabel = {Number of Monte Carlo trials},
      ylabel = {Performance (MT/s)},
      xtick/.expand once = \trials,
      xticklabels/.expand once = \tlabel,
    ]
      \foreach \B in \blocksizes {
        \addplot table[col sep=comma,x=trials,y=\B]{results-trials.csv};
        \addlegendentryexpanded{\B \ block size}
      }
     \end{axis}
  \end{tikzpicture}
  \caption{Performance vs. Trial Count across different block sizes}
\end{figure}

\begin{figure}[H]
  \centering
  \begin{tikzpicture}
    \begin{axis}[
      xmode = log,
      xlabel = {Block Size},
      ylabel = {Performance (MT/s)},
      xtick/.expand once = \blocksizes,
    ]
      \foreach \T in \trials {
        \addplot table[col sep=comma,x=blocksize,y=\T]{results-blocksize.csv};
        \addlegendentryexpanded{\T \ trials}
      }
     \end{axis}
  \end{tikzpicture}
  \caption{Performance vs. Trial Count across different block sizes}
\end{figure}

## Analysis

Each larger block size has much higher performance, and larger arrays also
increase performance.

GPU cores are scheduled in groups of 32 called warps. When only 8 threads are
grouped together for this compute task, only a quarter of the cores of the GPU
warp are performing work. Since all threads in the warp execute the same
instruction, these extra cores both are not and cannot do other work. A block
size of 32 threads does saturate the warp, but loses performance due to waiting
on memory accesses. A larger multiple of the warp size, e.g. 128 here, fully
saturate the compute performance as while one warp of 32 threads is waiting for
memory access, another group can be swapped in so there is no idle time.

Using GPU compute for this simulation yielded over 130x the performance of Project 1, based on my 12-core system:

$$
\frac{28260.4575}{213.48} = 132.38
$$

For proper GPU usage, there must be enough threads in a group to A) saturate the warp size, and B) to saturate compute time while other threads are waiting for memory access. If there are more threads, than performance won't increase past a certain amount, as the block will already be fully saturated.
