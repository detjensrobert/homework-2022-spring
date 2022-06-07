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

## Runtime information

These benchmarks were done on the OSU DGX HPC cluster with a node count of 4.

## Data

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
      xlabel = {Sum Values},
      ylabel = {Correlated Amplitude},
      cycle list name = color list,
    ]
      \addplot table[col sep=comma,x=sumindex,y=value]{plot.csv};
    \end{axis}
  \end{tikzpicture}
  \caption{Correlation Sums vs. Shift Amount}
\end{figure}

\begin{figure}[H]
  \centering
  \begin{tikzpicture}
    \begin{axis}[
      xlabel = {Number of Processors},
      ylabel = {Performance (MegaAC/s)},
      ymin = 0,
    ]
      \addplot table[col sep=comma,x=numprocs,y=performance]{results.csv};
    \end{axis}
  \end{tikzpicture}
  \caption{Correlation Performance vs. Number of Processors}
\end{figure}

```table
---
include: results.csv
caption: Correlation Performance Data
---
```

## Analysis

### State what the secret sine-wave period is, i.e., what change in shift gets you one complete sine wave?

A shift of just over 100 moves over one period -- something close to 105.

### What patterns are you seeing in the performance graph? Why do you think the performances work this way?

Performance scaling for this is almost exactly linear. As this is a very compute-heavy task and not very communicate-heavy, splitting up the work allows each node to work on their chunk of the data independently. The only synchronization needed for autocorrelation is the initial scatter and the final gather, all the compute in-between is wholly independent. In other words, the compute:communicate ratio is extremely high, and these types of workloads scale very well with additional nodes.

This workload would scale very well with the addition of one of the other flavors of parallel computing, as each node could have multiple threads for OpenMP threading, or GPUs to do massively parallel crunching. At this size of the data where the job time is fairly short, the extra overhead of setting up these additional techniques may not reap as much benefits as it would when dealing with much larger data sizes.
