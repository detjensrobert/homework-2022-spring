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

These benchmarks were done on my local machine, with an AMD Ryzen 2600 CPU and RX480 GPU.

The RX480 only supports work group sizes of up to 256, so these tests go from 8 to 256 instead of up to 512.

## Data

```table
---
include: results-mult.csv
caption: OpenCL Multiply Data
---
```

```table
---
include: results-multadd.csv
caption: OpenCL Multiply & Add Data
---
```

\newcommand{\globalsize}{1024,2048,4096,8192,16384,32768,65536,131072,262144,524288,1048576,2097152,4194304,8388608}
\newcommand{\globallabel}{1K  ,    ,    ,8K  ,     ,     ,64K  ,      ,      ,      ,1M     ,       ,       ,8M    }

\newcommand{\localsize}{8,16,32,64,128,256}

\pgfplotsset{
  axis lines = left,
  grid = major,
  grid style = {dashed,gray!30},
  legend pos = outer north east,
  legend cell align = left,
  scaled ticks = false,
  log ticks with fixed point,
  cycle list name = color list,
}

\begin{figure}[H]
  \centering
  \begin{tikzpicture}
    \begin{axis}[
      xmode = log,
      ymode = log,
      xlabel = {Global Array Size},
      ylabel = {Performance (GMult/s) (log scale)},
      xtick/.expand once = \globalsize,
      xticklabels/.expand once = \globallabel,
    ]
      \foreach \L in \localsize {
        \addplot table[col sep=comma,x=globalsize,y=\L]{results-mult-global.csv};
        \addlegendentryexpanded{\L \ local workgroup size}
      }
     \end{axis}
  \end{tikzpicture}
  \caption{Performance vs. Global Array Size across different local sizes}
\end{figure}

\begin{figure}[H]
  \centering
  \begin{tikzpicture}
    \begin{axis}[
      xmode = log,
      xlabel = {Local Workgroup Size},
      ylabel = {Performance (GMult/s)},
      xtick/.expand once = \localsize,
    ]
      \foreach \G in \globalsize {
        \addplot table[col sep=comma,x=localsize,y=\G]{results-mult-local.csv};
        \addlegendentryexpanded{\G \ global array size}
      }
     \end{axis}
  \end{tikzpicture}
  \caption{Performance vs. Local Work Size across different global sizes}
\end{figure}

## Analysis
