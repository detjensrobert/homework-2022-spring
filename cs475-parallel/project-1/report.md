---
header-includes:
  - \usepackage{pgfplots}

title: 'CS 475 Project 1: Monte Carlo Simulation'
author:
  - Robert Detjens
  - detjensr@oregonstate.edu
---

## Graphs

\newcommand{\threads}{{1,2,4,8,12,16,20,24,32}}
\newcommand{\trials}{{1,10,100,1000,10000,100000,1000000}}

\pgfplotsset{
  axis lines = left,
  grid = major,
  grid style = {dashed,gray!30},
  legend pos = outer north east,
  legend cell align = left,
  cycle list name = color list,
}

\pgfplotsset{
  group by/.style 2 args={
    x filter/.code={
      \edef\tempa{\thisrowno{#1}}
      \edef\tempb{#2}
      \ifx\tempa\tempb
      \else
        \def\pgfmathresult{inf}
      \fi
    }
  }
}

\begin{figure}[h]
  \centering
  \begin{tikzpicture}
    \begin{semilogxaxis}[
      xlabel = {No. of Monte-Carlo trials (log scale)},
      ylabel = {Performance (MT/s)},
    ]
      \foreach \N in \threads {
        \addplot table[col sep=comma,x=trials,y=performance,group by={0}{\N}]{results.csv};
        \addlegendentryexpanded{\N \ threads}
      }
    \end{semilogxaxis}
  \end{tikzpicture}
  \caption{Trials vs Performance across different numbers of threads}
\end{figure}

\begin{figure}[h]
  \centering
  \begin{tikzpicture}
    \begin{axis}[
      xlabel = {Threads used},
      ylabel = {Performance (MT/s)},
      xtick  = \threads
    ]
      \foreach \N in \trials {
        \addplot table[col sep=comma,x=threads,y=performance,col sep=comma,group by={1}{\N}]{results.csv};
        \addlegendentryexpanded{\N \ trials}
      }
    \end{axis}
  \end{tikzpicture}
  \caption{Threads vs Performance across different numbers of trials}
\end{figure}

## Peak performance

The highest performance from these trials is 134.79 MT/s, with 8 threads and 10000 trials.

## Likely probability

The probability for hitting the truck looks to be 29%, as as the trial count increases, the probability trends towards 29%.

## Parallel fraction

Speedup taken from the trial and thread count with the highest performance.

1-thread vs. 8-thread peformance for 10000 trials:

\begin{align*}
  PF &= \frac{n}{n-1}(1 - \frac{1}{Speedup}) \\
  &= \frac{8}{7}(1 - \frac{1}{134.79_{8t} / 24.96_{1t}}) \\
  &= 0.93123 \\
  &= 93.123 \%
\end{align*}

## Conclusion

These tests were performed on an i7-1165G7, which has 8 hw threads. As expected, the performance of these tests peaks at 8 threads when the processor is fully saturated without being overcommitted.

Performance increased for a given thread count as the number of trials increased, likely due to the longer runtime allowing better use of caches.

Performance for a fixed number of trials peaked at 8 cores, as more cores would overcommit resources and require context switching between processes.
