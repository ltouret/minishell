# minishell

A small Bash-like shell written in C, made for the 42 school curriculum.

## Build & run

```sh
make        # builds libft, then ./minishell
./minishell
```

Other targets: `make clean`, `make fclean`, `make re`, `make test` (builds, runs, then resets the terminal).

Needs `gcc` and `ncurses` (`-lncurses`), and `TERM` must be set.

## Features

- **Prompt and line editing**: the terminal runs in non-canonical mode (termcap), so input is read one key at a time. Backspace works and long lines wrap.
- **History**: use ↑ / ↓ to move through it. It is loaded from `~/.bash_history` at startup, and duplicate lines in a row are skipped.
- **Command separators**: `;`
- **Pipes**: `cmd1 | cmd2 | ...`
- **Redirections**: `<`, `>`, `>>`
- **Quotes**: `'single'`, `"double"`, and `\` escapes
- **Environment variables**: `$VAR` expansion, and `$?` holds the last exit status
- **Signals**:
  - `Ctrl-C` clears the line and gives a new prompt (exit status 130).
  - `Ctrl-\` quits a running child (exit status 131).
  - `Ctrl-D` on an empty line exits the shell.
- **Executables**: found through `PATH`, or run directly from a relative or absolute path.

### Builtins

| Builtin  | Notes                          |
|----------|--------------------------------|
| `echo`   | supports `-n`                  |
| `cd`     | `cd` with no argument goes to `$HOME`; it updates `PWD` and `OLDPWD` |
| `pwd`    |                                |
| `export` |                                |
| `unset`  |                                |
| `env`    |                                |
| `exit`   | takes an optional status code  |

When a builtin is the only command (no pipe), it runs in the shell's own process, so `cd`, `export` and the rest change the shell's own state. Inside a pipeline, a builtin runs in a forked child.

## How it works

Each line goes through these steps (see `srcs/main.c`):

1. **`wait_input`**: reads the line from raw terminal input.
2. **`parse_line`**: splits the line on `;` (quotes are respected) and reports unclosed quotes.
3. **`parse_cmds`**: splits each command on `|`.
4. **`parse_redirs`**: pulls out the `<`, `>` and `>>` redirections and opens their files.
5. **`post_process`**: expands `$VAR`, removes quotes and escapes, and builds `argv`.
6. **`exec_process`**: forks and pipes the commands, runs `execve` or the builtin, then waits for the children and sets `$?`.

## Project layout

```
srcs/
  main*.c, errors.c           main loop, signals, error handling
  parse_*.c, post_process*.c  lexing and parsing
  environ*.c, getters.c,
  setters.c                   environment (linked-list dictionary)
  exec_*.c, whereis.c,
  path_utils.c, exists.c      execution and PATH lookup
  builtins.c, cd.c, echo.c,
  env.c, exit.c, export.c,
  pwd.c, unset.c              builtins
  history*.c, termcap.c       terminal handling and history
libft/                        42 libft (with t_string helpers)
```
