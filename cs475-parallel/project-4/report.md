---
header-includes:
  - \usepackage{pgfplots}
  - \usepackage{pgfplotstable}

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
```

\newcommand{\threads}{1,2,4,8,12,16,20,24,32}
\newcommand{\sizes}{1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576,2097152,4194304,8388608,16777216}

\pgfplotsset{
  axis lines = left,
  grid = major,
  grid style = {dashed,gray!30},
  legend pos = outer north east,
  legend cell align = left,
  %cycle list name = color list,
}

\begin{figure}[h]
  \centering
  \begin{tikzpicture}
    \begin{axis}[
      xmode = log,
      xlabel = {Array Sizes (bytes)},
      ylabel = {Performance (MMults/s)},
      ymin = 0,
      xtick/.expand once = {\sizes},
      xticklabels = {1KB,,,8KB,,,64KB,,,,1MB,,,,16MB}
    ]

      %\addplot table[col sep=comma,x=array-size,y=mult-nosimd]{results.csv};
      %\addlegendentry{Multiply (regular)}
      %\addplot table[col sep=comma,x=array-size,y=mult-simd]{results.csv};
      %\addlegendentry{Multiply (SIMD)}

      \addplot table[col sep=comma,x=array-size,y=reduce-nosimd]{results.csv};
      \addlegendentry{Reduce (regular)}
      \addplot table[col sep=comma,x=array-size,y=reduce-simd]{results.csv};
      \addlegendentry{Reduce (SIMD)}
    \end{axis}
  \end{tikzpicture}
  \caption{Array Size vs. Performance for SIMD and non-SIMD multiply-reduce}
\end{figure}

\begin{figure}[h]
  \centering
  \begin{tikzpicture}
    \begin{axis}[
      xmode = log,
      xlabel = {Array Sizes (bytes)},
      ylabel = {Speedup},
      ymin = 5, ymax = 7,
      xtick/.expand once = {\sizes},
      xticklabels = {1KB,,,8KB,,,64KB,,,,1MB,,,,16MB}
    ]
      \addplot table[col sep=comma,x=array-size,y=reduce-speedup]{results.csv};
    \end{axis}
  \end{tikzpicture}
  \caption{Array Size vs. SIMD Speedup Factor for multiply-reduce}
\end{figure}


- Show the table of performances for each array size and the corresponding speedups
- Show the graph of SIMD/non-SIMD speedup versus array size (one curve only)
- What patterns are you seeing in the speedups?
- Are they consistent across a variety of array sizes?
- Why or why not, do you think?
