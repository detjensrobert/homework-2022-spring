---
header-includes:
  - \usepackage{pgfplots}
  - \usepackage{pgfplotstable}
  - \pgfplotsset{compat=1.18}
  - \usepackage{float}
  - \makeatletter
  - \def\fps@figure{H}
  - \makeatother

title: 'CS 475 Project 6: OpenCL Array Operations'
author:
  - Robert Detjens
  - detjensr@oregonstate.edu

colorlinks: true
linkcolor: blue
---

# Runtime information

These benchmarks were done on the OSU DGX HPC cluster with a node count of 4.

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
      xlabel = {Sum Values},
      ylabel = {Correlated Amplitude},
    ]
      \addplot table[col sep=comma,x=sumindex,y=value]{plot.csv};
    \end{axis}
  \end{tikzpicture}
  \caption{ArrayMult Performance vs. Global Array Size across different local sizes}
\end{figure}

\begin{figure}[H]
  \centering
  \begin{tikzpicture}
    \begin{axis}[
      xlabel = {Number of Processors},
      ylabel = {Performance (MAC/s)},
    ]
      \addplot table[col sep=comma,x=numprocs,y=perf]{results.csv};
    \end{axis}
  \end{tikzpicture}
  \caption{ArrayMult Performance vs. Global Array Size across different local sizes}
\end{figure}

```table
---
include: plot.csv
caption: Sums from Autocorrelation
---
```
