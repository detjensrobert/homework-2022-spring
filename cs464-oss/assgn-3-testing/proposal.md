---
header-includes:
  - \usepackage{helvet}
  - \renewcommand{\familydefault}{\sfdefault}
  - \usepackage{fancyhdr}
  - \pagestyle{fancy}
  - \rhead{Robert Detjens - detjensr}
  - \renewcommand{\headrulewidth}{0pt}
colorlinks: true
---

# Test & Inspection Proposal

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

## Proposal

I plan on adding unit tests to the command parser and dispatcher (code in question available
[here](https://github.com/shardlab/discordrb/blob/main/lib/discordrb/commands/parser.rb)). The error handling portion of
this code is not tested thoroughly, only the intended path. I will be adding test cases to make sure that the errors are
handled correctly.

DiscordRB uses RSpec to run unit tests. RSpec is a powerful testing framework with a large number of features, including
generating a code coverage report. Said coverage report currently reports that only 60\% of the library is being tested,
with the command parser being one of the larger portions not thoroughly covered.
