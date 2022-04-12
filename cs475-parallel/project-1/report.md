---
header-includes:
  - \usepackage{pgfplots}
---

# CS 475 Project 1: Monte Carlo Simulation

## Robert Detjens

---

## Peak performance

The highest performance from these trials is:

```{.run cmd="python" in="script" out="text"}
import csv

with open('results.csv', 'r') as csvfile:
  reader = csv.DictReader(csvfile)
  mperf = 0
  ml = {}
  for row in reader:
    # print(row)
    if (float(row['performance']) > mperf):
      mperf = float(row['performance'])
      ml = row

print(f"{mperf} MT/s, with {ml['threads']} threads and {ml['trials']} trials")
```

## Graphs

\newcommand{\threads}{1,2,4,8,12,16,20,24,32}
\newcommand{\trials}{1,10,100,1000,10000,100000,1000000}

\pgfplotsset{
  axis lines = left,
  grid = major,
  grid style = {dashed,gray!30},
  legend pos = outer north east,
  legend cell align = left,
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

## Likely probability

Choosing one of the runs (the one with the maximum number of trials would be good), tell me what you think the actual probability is.

```{.run cmd="python" in="script" out="text"}
import csv

with open('results.csv', 'r') as csvfile:
  reader = csv.DictReader(csvfile)
  mperf = 0
  ml = {}
  for row in reader:
    # print(row)
    if (float(row['performance']) > mperf):
      mperf = float(row['performance'])
      ml = row

print(f"")
```

## Parallel fraction

Compute Fp, the Parallel Fraction, for this computation.

\begin{align*}
  PF &= \frac{n}{n-1}(1 - \frac{1}{Speedup}) \\
  &= \frac{24}{23}(1 - \frac{1}{Speedup}) \\
\end{align*}
