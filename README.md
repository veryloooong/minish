# minish

IT3070, Project 1

A `mini`mal `sh`ell written in C.

## Features

- Run commands from specified path (cmds: `path`/`addpath`)
- Lists all processes and their IDs (cmds: `procls`)
- Kill a process (cmds: `prockill <PID>`)
- Stop a process (cmds: `procstop <PID>`)
- Resume a process (cmds: `procresume <PID>`)
- Builtins (`cd`, `ls`, `pwd`, `help`, `exit`, `clear`)

## Feature tracker

- The basics (`minish_builtins.c`)
  - [x] `cd`
  - [x] `dir`
  - [x] `pwd`
  - [x] `help`
  - [x] `exit`
  - [x] `clear`
- The input parsing utils (`minish_readline.c`)
  - [x] `read_line`
  - [x] `make_args`
- The execution utils (`minish_run.c`)
  - [x] `run`
    - [ ] Run processes in foreground or background mode
    - [ ] Use our own path (currently delegated to system PATH because `execvp`)
  - [x] `execute`
- The path variable commands (`minish_path.c`)
  - [ ] `pathls`
  - [ ] `pathadd`
  - [ ] `pathrm`
- The process manager (`minish_process.c`)
  - [ ] `fg` / `bg`
  - [ ] `procls`
  - [ ] `prockill`
  - [ ] `procstop`
  - [ ] `proccont`
- Other
  - [ ] Add control key handling (`Ctrl + D`, `Ctrl + Z`, etc.)
    - [ ] `Ctrl + C` interrupts and kills foreground task
    - [ ] `Ctrl + D` exits shell 
  - [x] Makefile
  - [x] Colours
  - [ ] `cwd` in shell prompt
  - [ ] Tab completion

## Instructions

```bash
make # For building

./build/minish

make clean # For cleanup
```

## Bugs

- `Ctrl + D` fucks everything up!!!
- Status color doesn't change for whatever fucking reason
