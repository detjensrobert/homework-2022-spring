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

## Data

```table
---
include: results.csv
caption: OpenCL Reduce Data
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
