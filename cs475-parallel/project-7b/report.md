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

These benchmarks were done on my local machine, with an AMD Ryzen 2600 CPU and RX480 GPU.

The RX480 only supports work group sizes of up to 256, so these tests go from 8 to 256 instead of up to 512.

# Array Mult/MultSum

## Data

Tables are at the end of the report in [Appendix: Tables] since they are quite long.

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
      xlabel = {Global Array Size},
      ylabel = {Performance (GMult/s)},
      xtick/.expand once = \globalsize,
      xticklabels/.expand once = \globallabel,
    ]
      \addlegendimage{empty legend}
      \addlegendentry{Local workgroup size}
      \foreach \L in \localsize {
        \addplot table[col sep=comma,x=globalsize,y=\L]{results-mult-global.csv};
        \addlegendentryexpanded{\L}
      }
     \end{axis}
  \end{tikzpicture}
  \caption{ArrayMult Performance vs. Global Array Size across different local sizes}
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
      \addlegendimage{empty legend}
      \addlegendentry{Global array size}
      \foreach \G in \globalsize {
        \addplot table[col sep=comma,x=localsize,y=\G]{results-mult-local.csv};
        \addlegendentryexpanded{\G}
      }
     \end{axis}
  \end{tikzpicture}
  \caption{ArrayMult Performance vs. Local Work Size across different global sizes}
\end{figure}

\begin{figure}[H]
  \centering
  \begin{tikzpicture}
    \begin{axis}[
      xmode = log,
      xlabel = {Global Array Size},
      ylabel = {Performance (GMult/s)},
      xtick/.expand once = \globalsize,
      xticklabels/.expand once = \globallabel,
    ]
      \addlegendimage{empty legend}
      \addlegendentry{Local workgroup size}
      \foreach \L in \localsize {
        \addplot table[col sep=comma,x=globalsize,y=\L]{results-multadd-global.csv};
        \addlegendentryexpanded{\L}
      }
     \end{axis}
  \end{tikzpicture}
  \caption{ArrayMultAdd Performance vs. Global Array Size across different local sizes}
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
      \addlegendimage{empty legend}
      \addlegendentry{Global array size}
      \foreach \G in \globalsize {
        \addplot table[col sep=comma,x=localsize,y=\G]{results-multadd-local.csv};
        \addlegendentryexpanded{\G}
      }
     \end{axis}
  \end{tikzpicture}
  \caption{ArrayMultAdd Performance vs. Local Work Size across different global sizes}
\end{figure}

# Reduction

\begin{figure}[H]
  \centering
  \begin{tikzpicture}
    \begin{axis}[
      xmode = log,
      xlabel = {Global Array Size},
      ylabel = {Performance (GMult/s)},
      xtick/.expand once = \globalsize,
      xticklabels/.expand once = \globallabel,
    ]
      \addlegendimage{empty legend}
      \addlegendentry{Local workgroup size}
      \foreach \L in \localsize {
        \addplot table[col sep=comma,x=globalsize,y=\L]{results-multadd-global.csv};
        \addlegendentryexpanded{\L}
      }
     \end{axis}
  \end{tikzpicture}
  \caption{Reduction Performance vs. Global Array Size across different local sizes}
\end{figure}

# Analysis

For all of these trials, the larger the global size, the higher the performance. As GPU compute is highly parallel, the GPU is able to process these very large arrays and have more of the silicon doing work. Smaller global sizes have smaller performance as there is less work overall being done, as less of the GPU is doing stuff.

Larger local sizes also beget larger performance (up to a point on my local GPU). A larger local workgroup size can better saturate the cores in a compute unit instead of leaving some idle with smaller sizes.

My local GPU (AMD RX 480) has 36 compute units and a max workgroup size of 256, but performance gains level off at a local size of 64.

## Multiply vs Multiply & Add

The addition of addition to the OCL job brought performance down by about 14%. This would be due to the extra time needed to complete the addition operation; although there is a fused-multiply-add operation in hardware that can do part of the addition while the multiply is finishing, the extra time to add the last part together does still take longer.


# Appendix: Tables

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

```table
---
include: results-reduce.csv
caption: OpenCL Reduction Data
---
```
