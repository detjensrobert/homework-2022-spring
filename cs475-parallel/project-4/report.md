---
header-includes:
  - \usepackage{pgfplots}
  - \usepackage{pgfplotstable}
  - \pgfplotsset{compat=1.18}

title: 'CS 475 Project 4: SIMD Parallelization via SSE'
author:
  - Robert Detjens
  - detjensr@oregonstate.edu
---

## Runtime information

This benchmark was done on an 6-core/12-hw-thread AMD 2600 running CentOS 7 (containerized).

## Data

```table
---
include: results.csv
caption: SIMD vs. non-SIMD multiply and reduce, raw data
---
```

\pgfplotsset{
  axis lines = left,
  grid = major,
  grid style = {dashed,gray!30},
  legend pos = outer north east,
  legend cell align = left,
  ,
  ymin = 0,
  xtick       = {1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576,2097152,4194304,8388608},
  xticklabels = {1KB ,    ,    ,8K  ,     ,     ,64K  ,      ,      ,      ,1MB    ,       ,       ,8M     },
}

<!-- \begin{figure}[h]
  \centering
  \begin{tikzpicture}
    \begin{axis}[
      xmode = log,
      xlabel = {Array Sizes (bytes)},
      ylabel = {Performance (MMults/s)},
    ]
      \addplot table[col sep=comma,x=array-size,y=mult-nosimd]{results.csv};
      \addlegendentry{Multiply (regular)}
      \addplot table[col sep=comma,x=array-size,y=mult-simd]{results.csv};
      \addlegendentry{Multiply (SIMD)}

      \addplot table[col sep=comma,x=array-size,y=mult-nosimd]{results-flip.csv};
      \addlegendentry{Multiply (regular) on \texttt{flip}}
      \addplot table[col sep=comma,x=array-size,y=mult-simd]{results-flip.csv};
      \addlegendentry{Multiply (SIMD) on \texttt{flip}}
    \end{axis}
  \end{tikzpicture}
  \caption{Array Size vs. Performance for SIMD and non-SIMD multiply}
\end{figure} -->

\begin{figure}[h]
  \centering
  \begin{tikzpicture}
    \begin{axis}[
      xmode = log,
      xlabel = {Array Sizes (bytes)},
      ylabel = {Speedup},
    ]
      %\addplot table[col sep=comma,x=array-size,y=mult-speedup]{results.csv};
      %\addlegendentry{Speedup (Multiply)}
      %\addplot table[col sep=comma,x=array-size,y=reduce-speedup]{results.csv};
      %\addlegendentry{Speedup (Mult+Reduce)}
      \addplot table[col sep=comma,x=array-size,y=mult-speedup]{results-flip.csv};
      \addlegendentry{Speedup (Multiply) on \texttt{flip}}
      %\addplot table[col sep=comma,x=array-size,y=reduce-speedup]{results-flip.csv};
      %\addlegendentry{Speedup (Mult+Reduce) on \texttt{flip}}
    \end{axis}
  \end{tikzpicture}
  \caption{Array Size vs. SIMD Speedup Factor for multiply-reduce}
\end{figure}

## Analysis

Performance of the SIMD graphs decreases in steps as the size of the array
increases. This speedup decay parallels that of cache latency graphs, see below.

![Example Cache Latency Chart (source [Anandtech](https://www.anandtech.com/show/9482/intel-broadwell-pt2-overclocking-ipc/3))](https://images.anandtech.com/doci/9482/5775C%20Memory%20Latency_575px.png){ width=60% }

As the array size increases and no longer fits in the per-core 32KB L1 cache after
16K elements, the performance drops as now the L2 cache is being used with
higher latency. The L2 cache on `flip` is 256KB, so after that the performance takes a further hit once the arrays large enough to need to use the L3.

The non-SIMD performance was fairly constant across all array sizes; the performance of doing the multiply-reduce is not limited by the memory latency/bandwidth.
