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

# Design and Contributor Usability

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

## Usability Assessment

### End User Documentation

This project has a fairly extensive set of class and method-level documentation hosted at <https://drb.shardlab.dev> build from inline code comments, and a complete set of examples showcasing the library's functionality included in the repository (<https://github.com/shardlab/discordrb/tree/main/examples>). The project's README shows users how to install the library and a simple bot to verify things are working ([see #Usage](https://github.com/shardlab/discordrb#usage)).

### License

This project does feature that it is released freely under the MIT License at the end of the README, and in the repo info on the sidebar of the Github web interface. This is a very permissable license and has no restrictions on what can be done with the software, only absolving the project of all warranty and liability of its use.

### Communications

This project uses Github issues and Discord to interact with users, both of which have search features. Issues are available to the general public without an account, while the Discord channel is gated behind a login and is not great to search through the channel archives. Most of the time, once a plan for a feature is decided, an issue is created to further refine it. However, recently more discussions about the planned library overhaul/refactor have been done exclusively on Discord.

## Suggestions

Moving more of the feature discussion out of Discord and into a more public forum would greatly increase the opportunity for new users to contribute to them. Currently, new contributors are encouraged to join the Discord channel to discuss new features, this requires a login gate to view archives. Github only requires a login to post.


The contributing documentation should be fleshed out; currently it is somewhat bare, only a single section in the README detailing how to setup a dev environment. There is no codified standard for commits (other than a code linter) and no explicit code of conduct.
