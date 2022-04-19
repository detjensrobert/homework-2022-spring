---
header-includes:
  - \usepackage{helvet}
  - \renewcommand{\familydefault}{\sfdefault}
colorlinks: true
---

# Contributor Usability and Accessibility Proposal

## Project Information

### URL

<https://github.com/shardlab/discordrb>

### License

MIT

### Description

DiscordRB is a library implementing the Discord chat application API to make bots. This is written in Ruby and is
active, with the latest commit to the `main` branch on Feb. 5th, and active pull requests as of today (March 28th).

This project is aimed at developers who want to use a simple scripting language like Ruby to make small- or medium-sized
Discord bots.

This repo is about 12-14KLOC, according to the `cloc` tool:

```console
$ cloc .
     153 text files.
     146 unique files.
      10 files ignored.

--------------------------------------------------------------------------------
Language                      files          blank        comment           code
--------------------------------------------------------------------------------
Ruby                            121           3585           4896          12966
Markdown                          6            358              0           1161
JSON                             15              0              0            232
YAML                              3             27             13            173
Bourne Again Shell                1              2              1              4
--------------------------------------------------------------------------------
SUM:                            146           3972           4910          14536
--------------------------------------------------------------------------------
```
