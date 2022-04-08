---
header-includes:
  - \usepackage{pgfplots}
---

# CS 475 Project 1: Monte Carlo Simulation

## Robert Detjens

---

Run this for some combinations of trials and threads. Do timing for each combination. Like we talked about in the Project Notes, run each experiment some number of tries, NUMTIMES, and record just the peak performance.

Do a table and two graphs. The two graphs need to be:

    Performance versus the number of Monte Carlo trials, with the colored lines being the number of OpenMP threads.
    Performance versus the number OpenMP threads, with the colored lines being the number of Monte Carlo trials..

(See the Project Notes to see an example of this and how to get Excel to do most of the work for you.)

Chosing one of the runs (the one with the maximum number of trials would be good), tell me what you think the actual probability is.

Compute Fp, the Parallel Fraction, for this computation.

\pagebreak

\pgfplotsset{
  discard if not/.style 2 args={
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

\newcommand{\threads}{1,2,4,8}
\newcommand{\trials}{1,10,100,1000,10000,100000,1000000}

\begin{figure}[h]
  \centering
  \begin{tikzpicture}
    \begin{semilogxaxis}[
      axis lines = left,
      xlabel = {No. of Monte-Carlo trials},
      ylabel = {Performance (MT/s)},
      grid = major,
      grid style = {dashed,gray!30},
      legend pos = outer north east,
      legend cell align = left
    ]
      \foreach \N in {\threads}{
        \addplot table[x=trials,y=performance,col sep=comma,discard if not={0}{\N}] {results.csv};
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
      axis lines = left,
      xlabel = {Threads used},
      ylabel = {Performance (MT/s)},
      grid = major,
      grid style = {dashed,gray!30},
      legend pos = outer north east,
      legend cell align = left
    ]

    \addplot table[col sep=comma] {pivot.csv};
    \addlegendentryexpanded{1 \ trials}

    \end{axis}
  \end{tikzpicture}
  \caption{Threads vs Performance across different numbers of trials}
\end{figure}
