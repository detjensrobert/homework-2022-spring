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

# Documentation Report

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

## Summary of Work

### Identified Issue

Some newly-added methods exposed by the library have very ambiguous descriptions that are not very helpful without
further context, or no documentation at all.

Additionally, there is a mix of cases and reference conventions for the parent object, with `the thing` and `this thing` both used to refer to the parent instance.

For example:

![[Message#buttons](https://drb.shardlab.dev/main/Discordrb/Message.html#buttons-instance_method)](images/bad-buttons.png){ width=40% }
![[Channel#join, #leave](https://drb.shardlab.dev/main/Discordrb/Channel.html#join_thread-instance_method)](images/bad-threads.png){ width=40% }

### Contribution

I have improved these docstrings, and my contribution is available [as a GitHub diff](https://github.com/shardlab/discordrb/compare/main...detjensrobert:detjensrobert/thread-doc-improvements).
