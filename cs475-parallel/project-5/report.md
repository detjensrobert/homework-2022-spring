---
header-includes:
  - \usepackage{pgfplots}
  - \usepackage{pgfplotstable}
  - \pgfplotsset{compat=1.18}

title: 'CS 475 Project 5: CUDA Monte Carlo Simulation'
author:
  - Robert Detjens
  - detjensr@oregonstate.edu
---

## Runtime information

- Tell what machine you ran this on

## Data

- Show the table and the two graphs
  - Graph of performance vs. NUMTRIALS with multiple curves of BLOCKSIZE
  - Graph of performance vs. BLOCKSIZE with multiple curves of NUMTRIALS

```table
---
include: results.csv
caption:REPLACEME
---
```

\pgfplotsset{
  axis lines = left,
  grid = major,
  grid style = {dashed,gray!30},
  legend pos = outer north east,
  legend cell align = left,
}

\begin{figure}[h]
  \centering
  \begin{tikzpicture}
    \begin{axis}[
      xmode = log,
      xlabel = {Array Sizes (bytes)},
      ylabel = {Speedup},
    ]
      \addplot table[col sep=comma,x=array-size,y=mult-speedup]{results-flip.csv};
      \addlegendentry{Speedup (Multiply) on \texttt{flip}}
     \end{axis}
  \end{tikzpicture}
  \caption{Array Size vs. SIMD Speedup Factor for multiply-reduce}
\end{figure}

## Analysis


- What patterns are you seeing in the performance curves?
- Why do you think the patterns look this way?
- Why is a BLOCKSIZE of 8 so much worse than the others?
- How do these performance results compare with what you got in Project #1? Why?
- What does this mean for the proper use of GPU parallel computing?
