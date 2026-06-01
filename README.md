# Minishell

This project has been created as part of the 42 curriculum by **caburges** and **alsuchon**.

## Description

Create a bash-inspired shell that can execute commands, handle redirections, pipes, and implement built-in commands.

## ⚠️ Disclaimer

This project adheres to the school's strict **Norminette** coding standards.
The code has been left in its original submitted state as much as possible to show my development over time.
Some example **Norminette** constraints:

- **Line length**: Maximum 80 characters per line
- **Function parameters**: Maximum 4 parameters per function
- **Function size**: Maximum 25 lines per function
- **Variable declaration**: Must be separated from initialization
- **Loops**: No for or do-while loops

## Key Features

- Interactive command-line interface with prompt display
- Command parsing with support for quotes (single and double)
- Pipes (|) for command chaining
- Input/output redirections (<, >, <<, >>)
- Here-documents (<<) with delimiter expansion
- Environment variable expansion ($VAR)
- Exit status expansion ($?)
- Built-in commands: echo, cd, pwd, export, unset, env, exit
- Signal handling (Ctrl-C, Ctrl-D, Ctrl-\\)
- Command history navigation
- Fully compliant with 42's norminette coding standards

## Instructions

### Prerequisites

- C compiler (cc)
- Make
- A Unix-based operating system
- Readline library

### Installation

Clone the repository:

```bash
git clone https://github.com/caburges/minishell.git
cd minishell
```

### Compilation

To compile the project, run:

```bash
make
```

This will generate a `minishell` executable.

### Usage

Run the program:

```bash
./minishell
```

Once running, you can enter commands just like in bash:

#### Example Commands

```bash
# Simple commands
echo "Hello, World!"
ls -la
pwd

# Pipes
cat file.txt | grep "pattern" | wc -l

# Redirections
echo "text" > output.txt
cat < input.txt
cat file.txt >> append.txt

# Here-documents
cat << EOF
This is a heredoc
Multiple lines supported
EOF

# Environment variables
export MY_VAR="value"
echo $MY_VAR
unset MY_VAR

# Exit status
ls non_existent_file
echo $?
```

### Controls

- `Ctrl+C` - Interrupts the current command
- `Ctrl+D` - Exits the shell (when line is empty)
- `Ctrl+\` - Does nothing (ignored)
- `↑` / `↓` - Navigate command history

### Available Make Rules

- `make` or `make all` - Compiles the project
- `make clean` - Removes object files
- `make fclean` - Removes object files and the executable
- `make re` - Recompiles the entire project

## Project Structure

### Core Files

- `main.c` - Entry point and main loop
- `Makefile` - Build configuration

### Builtins/

Built-in command implementations:
- `cd.c` - Change directory command
- `echo.c` - Echo command with -n option support
- `env.c` - Environment variable display
- `exit.c` - Shell exit command
- `export.c` - Export environment variables
- `export_utils.c` - Helper functions for export
- `handle_var_export.c` - Variable handling for export
- `init_export_list.c` - Export list initialization
- `pwd.c` - Print working directory
- `unset.c` - Unset environment variables

### Parsing/

Command parsing and tokenization:
- `lexer.c` - Lexical analysis
- `get_tokens.c` - Token extraction
- `handle_tokens.c` - Token processing
- `token_utils.c` - Token utility functions
- `parse_tokens.c` - Token parsing logic
- `parsing.c` - Main parsing functions
- `handle_quotes.c` - Quote handling (single and double)
- `heredocs.c` - Here-document implementation
- `heredocs_utils.c` - Here-document utilities
- `heredocs_utils2.c` - Additional here-document utilities

### Exec/

Command execution and process management:
- `executor.c` - Main execution logic
- `command_list.c` - Command list management
- `command_table_utils.c` - Command table utilities
- `prep_command_tables.c` - Command table preparation
- `execute_builtin.c` - Built-in command execution
- `pipeline.c` - Pipe implementation
- `redirections.c` - Redirection handling
- `redirections_utils.c` - Redirection utilities
- `fd_management.c` - File descriptor management
- `child_errors.c` - Child process error handling
- `exec_utils.c` - Execution utilities
- `return_status.c` - Exit status management

### Expansions/

Variable and parameter expansion:
- `expand_doubles_quotes.c` - Double quote expansion
- `extract_expansion.c` - Expansion extraction
- `expansions_cutting.c` - Expansion string manipulation
- `expansions_utils.c` - Expansion utilities

### Environment/

Environment variable management:
- `copy_env.c` - Environment copying
- `env_utils.c` - Environment utilities
- `env_utils_2.c` - Additional environment utilities
- `set_up.c` - Environment setup

### Signals/

Signal handling:
- `signals.c` - Signal handler setup
- `heredoc_signal.c` - Here-document signal handling

### Clean/

Resource cleanup:
- `clean_up.c` - Memory and resource deallocation

## Technical Details

### Parsing

The shell uses a lexer-parser architecture to break down input into tokens, identify commands, arguments, redirections, and pipes. Special handling is implemented for quotes to preserve spaces and prevent expansion where necessary.

### Process Management

Commands are executed in child processes created with fork(). The shell uses execve() to replace the child process with the target program. Pipes are implemented using pipe() and dup2() for file descriptor redirection.

### Built-in Commands

Built-in commands are executed in the parent process to allow modification of the shell's environment (like cd changing the working directory or export modifying environment variables).

### Signal Handling

The shell properly handles signals according to bash behavior:
- SIGINT (Ctrl-C): Interrupts the current command and displays a new prompt
- SIGQUIT (Ctrl-\\): Ignored in interactive mode
- EOF (Ctrl-D): Exits the shell when the input line is empty

### Memory Management

The project includes comprehensive memory management with proper allocation, tracking, and deallocation of all dynamically allocated resources to prevent memory leaks.
