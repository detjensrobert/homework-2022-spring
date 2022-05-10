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
  xtick       = {1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576,2097152,4194304,8388608,16777216},
  xticklabels = {1KB ,    ,    ,    ,16KB ,     ,     ,128KB ,      ,      ,1MB    ,       ,       ,       ,16MB    },
}

\begin{figure}[h]
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
\end{figure}

\begin{figure}[h]
  \centering
  \begin{tikzpicture}
    \begin{axis}[
      xmode = log,
      xlabel = {Array Sizes (bytes)},
      ylabel = {Speedup},
    ]
      \addplot table[col sep=comma,x=array-size,y=mult-speedup]{results.csv};
      \addlegendentry{Speedup (Multiply)}
      \addplot table[col sep=comma,x=array-size,y=mult-speedup]{results-flip.csv};
      \addlegendentry{Speedup (Multiply) on \texttt{flip}}
      %\addplot table[col sep=comma,x=array-size,y=reduce-speedup]{results.csv};
      %\addlegendentry{Speedup (Mult+Reduce)}
    \end{axis}
  \end{tikzpicture}
  \caption{Array Size vs. SIMD Speedup Factor for multiply-reduce}
\end{figure}


- Show the table of performances for each array size and the corresponding speedups
- Show the graph of SIMD/non-SIMD speedup versus array size (one curve only)
- What patterns are you seeing in the speedups?
- Are they consistent across a variety of array sizes?
- Why or why not, do you think?
