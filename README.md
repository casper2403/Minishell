# Minishell

A small, interactive Unix shell implemented in C. It supports essential shell features—builtins, environment management, pipes, redirections, and heredocs—while focusing on robust parsing and signal handling. Built for Linux and linked against `readline` for line editing and history.

---

## Highlights
- **Interactive shell:** Prompted REPL with `readline` history (`minishell> ...`).
- **Builtins:** `echo`, `cd`, `pwd`, `export`, `unset`, `env`, `exit`.
- **Pipes:** Multi-command pipelines with proper parent/child process management.
- **Redirections:** Input `<`, output `>`, append `>>`, and heredocs `<<` with expansion control.
- **Variable expansion:** `$VAR` and `$?` expansion, respecting quotes.
- **Signals:** Clean handling of `Ctrl-C`, `Ctrl-D`, and heredoc interrupts.
- **Environment:** Copy-on-init + in-memory updates for `PWD`, `OLDPWD`, `SHLVL`, and exported variables.

## Why This Project
- **Systems foundations:** Process creation (`fork`), I/O redirection (`dup2`), pipes, and signals.
- **Parsing rigor:** Tokenization with quotes, redirection syntax validation, and command graphs.
- **Intern readiness:** Demonstrates practical C engineering, robust error paths, and clean runtime behavior.

## Build & Run
- **Prereqs (Linux):** Install development headers for `readline`.

```bash
sudo apt-get update
sudo apt-get install -y build-essential libreadline-dev
```

- **Build:**

```bash
make
```

- **Run:**

```bash
./minishell
```

Build flags and sources are defined in [Makefile](Makefile). The entry point is [src/main.c](src/main.c).

## Features
- **Builtins:**
  - `echo [-n] ...` with expansion unless single-quoted.
  - `cd [path]` updates `PWD` and `OLDPWD`; errors on invalid paths.
  - `pwd` prints current directory.
  - `export NAME=VALUE` adds/updates env; prints sorted export list when no args.
  - `unset NAME` removes env entries.
  - `env` prints the environment.
  - `exit [status]` exits the shell; validates numeric status.
- **Pipes:** Arbitrary-length pipelines with parent wait and exit-status propagation.
- **Redirections:** `<`, `>`, `>>` attach to any command; `<<` heredoc supports optional quoting of the delimiter (quoted → no expansion).
- **Expansion:** `$VAR`, `$?` handled before execution, respecting quote metadata per-arg.
- **Signals:**
  - In prompt: `Ctrl-C` clears the line and redisplays the prompt; `Ctrl-D` signals EOF.
  - In children: `Ctrl-C` prints a newline and sets exit status.
  - In heredoc: `Ctrl-C` aborts the heredoc safely.

## Usage Examples
- **Basics:**
```bash
echo Hello, world!
pwd
cd /tmp && pwd
```

- **Variables:**
```bash
export NAME=Casper
echo "Hi $NAME"       # expands
echo '$NAME'          # single-quoted → no expansion
unset NAME
```

- **Pipes and redirections:**
```bash
echo hello | wc -c
cat < infile | grep foo | sort > outfile
```

- **Heredocs:**
```bash
cat <<EOF
Line 1
$USER expands here
EOF

cat <<'EOF'
$USER does NOT expand here (quoted delimiter)
EOF
```

- **Environment inspection:**
```bash
env | grep PWD
export SHLVL=42
env | grep SHLVL
```

## Architecture
- **Tokenizer/Lexer:** Splits input by pipes and arguments, tracking quote state.
- **Parser:** Validates redirection syntax and constructs a linked list of `t_token` with `argv`, `quoted` flags, and `t_redir` chains. See [src/parser.c](src/parser.c), [src/parser_utils.c](src/parser_utils.c).
- **Executor:** Builds pipelines (`t_piper`), applies redirections, expands variables, and runs builtins or external executables via `PATH`. See [src/executor.c](src/executor.c) and utilities in [src/executor_utils*.c](src/).
- **Signals:** Mode-based handlers for prompt, child, and heredoc contexts. See [src/signals.c](src/signals.c).
- **Environment:** Utility helpers for lookups, sorting, updates (`PWD`, `OLDPWD`, `SHLVL`). See [src/builtin](src/builtin) and [src/executor_utils.c](src/executor_utils.c).

## Self-Test
After building, try the following:
```bash
echo Start && echo Done
export FOO=bar && echo $FOO && unset FOO
printf "one\ntwo\nthree\n" | grep t | wc -l
cat <<EOF > notes.txt
Minishell notes
EOF
cat notes.txt
```

For more scenarios, see the bundled tester folder [minishell_tester](minishell_tester).

## Limitations
- Not a full POSIX shell: no job control, no subshells `(...)`, no advanced expansions.
- Globbing (`*`, `?`) is not implemented as shell expansion.
- Only interactive mode; passing a script file is rejected by design.

## Tech Stack
- **Language:** C (C99)
- **Libs:** `readline`, custom [lib/libft](lib/libft)
- **OS:** Linux
