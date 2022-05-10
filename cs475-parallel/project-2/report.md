---
header-includes:
  - \usepackage{pgfplots}

title: 'CS 475 Project 2: Numeric Integration'
author:
  - Robert Detjens
  - detjensr@oregonstate.edu
---

## Runtime information

This benchmark was done on an 4-core/8-hw-thread Intel i7-1165G7 running Fedora Linux.

## Volume

As the number of slices increases, the volume approaches 7.757854.

## Data

```table
---
include: results-slices.csv
caption: Pivot table for Slices vs. Performance
---
```


\newcommand{\threads}{1,2,4,8,12,16,20}
<!-- \newcommand{\threads}{1,2,4,8,12,16,20,24,32} -->
\newcommand{\slices}{50,100,500,1000,2000,4000,8000}
<!-- \newcommand{\slices}{1000,2000,4000,8000,16000} -->

\pgfplotsset{
  axis lines = left,
  grid = major,
  grid style = {dashed,gray!30},
  legend pos = outer north east,
  legend cell align = left,
  cycle list name = color list,
}

\begin{figure}[h]
  \centering
  \begin{tikzpicture}
    \begin{semilogxaxis}[
      xlabel = {No. of Integration Slices},
      ylabel = {Performance (MNode/s)},
      xtick  = {\slices},
      scaled ticks = false,
      log ticks with fixed point,
    ]
      \foreach \N in {\threads} {
        \addplot table[col sep=comma,x=slices,y=\N]{results-slices.csv};
        \addlegendentryexpanded{\N \ threads}
      }
    \end{semilogxaxis}
  \end{tikzpicture}
  \caption{Integration Slices vs Performance across different numbers of threads}
\end{figure}

```table
---
include: results-threads.csv
caption: Pivot table for Threads vs. Performance
---
```

\begin{figure}[h]
  \centering
  \begin{tikzpicture}
    \begin{axis}[
      xlabel = {Threads used},
      ylabel = {Performance (MNode/s)},
      xtick  = {\threads},
    ]
      \foreach \N in {\slices} {
        \addplot table[col sep=comma,x=threads,y=\N]{results-threads.csv};
        \addlegendentryexpanded{\N \ slices}
      }
    \end{axis}
  \end{tikzpicture}
  \caption{Threads vs Performance across different numbers of slices}
\end{figure}

## Analysis

Performance peaks with 8 threads and 2000 slices, which is expected for this machine with 8 hardware threads.

Threads

### Parallel Fraction

- What is the Parallel Fraction for this application, using the Inverse Amdahl equation?

### Maximum Speedup

- Given that Parallel Fraction, what is the maximum speed-up you could ever get?
