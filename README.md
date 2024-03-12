# minish

IT3070, Project 1

A `mini`mal `sh`ell written in C.

## Features (TODO)

- Run commands from specified path (cmds: `path`/`addpath`)
- Lists all processes and their IDs (cmds: `procls`)
- Kill a process (cmds: `prockill <PID>`)
- Stop a process (cmds: `procstop <PID>`)
- Resume a process (cmds: `procresume <PID>`)
- Builtins (`cd`, `ls`, `pwd`, `help`, `exit`, `clear`)

## Feature tracker

- The basics (`minish_builtins.c`)
  - [ ] `cd`
  - [ ] `ls`
  - [ ] `pwd`
  - [x] `help`
  - [x] `exit`
  - [x] `clear`
- The input parsing utils (`minish_readline.c`)
  - [x] `read_line`
  - [ ] `tokenize_line`
- The execution utils (`minish_run.c`)
  - [ ] `run`
  - [ ] `execute`
- The path variable commands (`minish_path.c`)
  - [ ] `path`
  - [ ] `addpath`
