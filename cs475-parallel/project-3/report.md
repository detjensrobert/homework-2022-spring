---
header-includes:
  - \usepackage{pgfplots}
  - \usepackage{pgfplotstable}

title: 'CS 475 Project 3: Functional Decomposition'
author:
  - Robert Detjens
  - detjensr@oregonstate.edu
---

## The Special Sauce Factor

The extra factor in this simulation was adding a small chance for a wildfire to happen. This fire burns all the grain and deer, resetting both to zero. These fires have a 10% chance to occur during the summer months (May to August), symbolized with a vertical line on the below graph.

## Analysis

Grain grows readily in the winter and spring when the rainfall is high. During the spring the deer population increases as there is a large amount of grain to sustain them. By summer, the grain is dwindling, and the deer population starts to die off.

In mid-2023 and 2024, a fire occurred and burned away all the grain and population, causing both populations to drop down to zero. Both years, the grain doesn't recover until the following winter. The fire in late 2025 happens after the deer have completely eaten all the grain, so only the deer population is reset.

## Data

\pgfplotstableread[col sep=comma]{results.csv}\results

\pgfplotsset{
  axis lines = left,
  grid = major,
  grid style = {dashed,gray!30},
  legend pos = outer north east,
  legend cell align = left,
  cycle list name = color list,
  width=0.8\textwidth,
  height=25em
}

\begin{figure}[h]
  \centering
  \begin{tikzpicture}
    \begin{axis}[
      xmin = 0,
      xmax = 72,
      ymax = 50,
      xlabel = {Time (years)},
      ylabel = {},
      xtick distance = 12,
      %xticklabels from table = {\results}{year},
      xticklabels = {2021,...,2028},
      x tick label as interval=true,
      bar width=0px,
    ]
      \addplot+[smooth] table[x expr=\coordindex,y expr=(\thisrow{temp}-32) * 5 / 9]{\results};
      \addlegendentry{Temperature (C)}

      \addplot+[smooth] table[x expr=\coordindex,y=precip]{\results};
      \addlegendentry{Precipitation (in)}

      \addplot+[smooth] table[x expr=\coordindex,y=grain]{\results};
      \addlegendentry{Grain Height (in)}

      \addplot+[smooth] table[x expr=\coordindex,y=deer]{\results};
      \addlegendentry{No. of Deer}

      \addplot+[ybar] table[x expr=\coordindex,y expr=\thisrow{burned} * 50]{\results};
      \addlegendentry{Burn Occured}
    \end{axis}
  \end{tikzpicture}
  \caption{Environment Simulation across 6 Years}
\end{figure}

\pagebreak

```table
---
include: results.csv
caption: Original Data
---
```
