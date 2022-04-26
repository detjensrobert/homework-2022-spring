---
header-includes:
  - \usepackage{pgfplots}

title: 'CS 475 Project 2: Numeric Integration'
author:
  - Robert Detjens
  - detjensr@oregonstate.edu
---

## Runtime information

This benchmark was done on a 12-core AMD Ryzen 2600 running Arch Linux.


- Tell what machine you ran this on

- What do you think the actual volume is?

- Show the performances you achieved in tables and two graphs showing:
  - Performance as a function of NUMNODES with colored lines showing different NUMT values
  - Performance as a function of NUMT with colored lines showing different NUMNODES values

- What patterns are you seeing in the speeds?

- Why do you think it is behaving this way?

- What is the Parallel Fraction for this application, using the Inverse Amdahl equation?

- Given that Parallel Fraction, what is the maximum speed-up you could ever get?

## Graphs

\newcommand{\threads}{1,2,4,8,12,16,20,24,32}
\newcommand{\nodes}{1000,2000,4000,8000,16000}

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
    \begin{axis}[
      xlabel = {No. of Integration Slices},
      ylabel = {Performance (MT/s)},
      xtick  = \nodes
    ]
      \foreach \N in \threads {
        \addplot table[col sep=comma,x=nodes,y=performance,group by={0}{\N}]{results.csv};
        \addlegendentryexpanded{\N \ threads}
      }
    \end{axis}
  \end{tikzpicture}
  \caption{Slices vs Performance across different numbers of threads}
\end{figure}

\begin{figure}[h]
  \centering
  \begin{tikzpicture}
    \begin{axis}[
      xlabel = {Threads used},
      ylabel = {Performance (MT/s)},
      xtick  = \threads
    ]
      \foreach \N in \nodes {
        \addplot table[col sep=comma,x=threads,y=performance,col sep=comma,group by={1}{\N}]{results.csv};
        \addlegendentryexpanded{\N \ nodes}
      }
    \end{axis}
  \end{tikzpicture}
  \caption{Threads vs Performance across different numbers of slices}
\end{figure}
