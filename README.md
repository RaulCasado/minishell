# 🐚 Minishell

<div align="center">
  <img src="https://i.imgur.com/oHW9Vwx.png" alt="Minishell Logo" width="400"/>
  <p><i>A simple Unix shell implementation</i></p>
</div>

<div align="center">
  
  [![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
  ![C](https://img.shields.io/badge/language-C-blue)
  ![Status](https://img.shields.io/badge/status-completed-brightgreen)
  
</div>

## 📋 Table of Contents

- [Overview](#-overview)
- [Features](#-features)
- [Installation](#-installation)
- [Usage](#-usage)
- [Command Syntax](#-command-syntax)
- [Built-in Commands](#-built-in-commands)
- [Implementation Details](#-implementation-details)
- [Project Structure](#-project-structure)
- [Contributors](#-contributors)
- [License](#-license)

## 🔍 Overview

Minishell is a simplified Unix shell implementation written in C. It provides a command-line interface for users to interact with the operating system through command execution, redirection, and piping. This project is part of the 42 School curriculum, challenging students to recreate essential features of the Bash shell.

## ✨ Features

- **Interactive Command Prompt**: User-friendly interface with command history
- **Command Execution**: Run both built-in commands and external programs
- **Environment Variables**: Management and expansion of environment variables
- **Signal Handling**: Proper handling of keyboard interrupts (Ctrl+C, Ctrl+D, Ctrl+\\)
- **Redirection & Piping**:
  - Input redirection (`<`)
  - Output redirection (`>`, `>>`)
  - Heredoc support (`<<`)
  - Command piping (`|`)
- **Quote Handling**: Support for both single (`'`) and double (`"`) quotes
- **Error Handling**: Comprehensive error messages and proper exit statuses

## 💻 Installation

### Prerequisites

- GCC compiler
- GNU Make
- Readline library

### Clone and Compile

```bash
# Clone the repository
git clone https://github.com/yourusername/minishell.git
cd minishell

# Compile the project
make

# Run the shell
./minishell
```

## 🚀 Usage

Once you've launched Minishell, you'll be presented with a prompt where you can enter commands:

```
Minishell> ls -la
Minishell> echo "Hello, world!"
Minishell> cat file.txt | grep "pattern" > output.txt
```

### Exit the Shell

```
Minishell> exit
```

or press `Ctrl+D`

## 📝 Command Syntax

### Basic Command Execution

```
Minishell> command [arguments...]
```

### Redirection

```
# Input redirection
Minishell> command < input_file

# Output redirection (overwrite)
Minishell> command > output_file

# Output redirection (append)
Minishell> command >> output_file

# Heredoc
Minishell> command << DELIMITER
> line 1
> line 2
> DELIMITER
```

### Pipes

```
Minishell> command1 | command2 | command3
```

### Environment Variables

```
Minishell> echo $HOME
Minishell> export VAR=value
Minishell> echo $VAR
```

### Exit Status

```
Minishell> echo $?  # Displays the exit status of the last command
```

## 🛠️ Built-in Commands

Minishell implements the following built-in commands:

| Command | Description | Usage |
|---------|-------------|-------|
| `echo` | Displays text on the terminal | `echo [-n] [string...]` |
| `cd` | Changes current directory | `cd [directory]` |
| `pwd` | Prints current working directory | `pwd` |
| `export` | Sets environment variables | `export [name[=value]...]` |
| `unset` | Removes environment variables | `unset [name...]` |
| `env` | Displays environment variables | `env` |
| `exit` | Exits the shell | `exit [status]` |

## 🔧 Implementation Details

### Signal Handling

- **Ctrl+C (SIGINT)**: Creates a new line with a fresh prompt
- **Ctrl+D (EOF)**: Exits the shell
- **Ctrl+\\ (SIGQUIT)**: Ignored in interactive mode

### Quote Handling

- **Single quotes (`'`)**: Prevent interpretation of all metacharacters
- **Double quotes (`"`)**: Prevent interpretation of metacharacters except for `$` (variable expansion)

### Environment Variable Expansion

Variables are expanded according to these rules:
- `$VAR`: Expands to the value of the environment variable VAR
- `$?`: Expands to the exit status of the most recently executed foreground command

## 📂 Project Structure

```
minishell/
├── includes/                # Header files
│   ├── minishell.h          # Main header
│   └── libft/               # Our own library of utility functions
├── src/                     # Source files
│   ├── main.c               # Entry point
│   ├── builtins/            # Implementation of built-in commands
│   ├── executor/            # Command execution logic
│   ├── parser/              # Command parsing
│   ├── lexer/               # Tokenization
│   ├── expander/            # Variable expansion
│   ├── signals/             # Signal handling
│   └── utils/               # Utility functions
├── Makefile                 # Build instructions
├── .gitignore               # Git ignore file
└── README.md                # This file
```

## 👥 Contributors

- [Your Name](https://github.com/yourusername) - Core Development
- [Contributor Name](https://github.com/contributorname) - Testing & Documentation

## 📄 License

This project is licensed under the MIT License - see the LICENSE file for details.

---

<div align="center">
  <p>Made with ❤️ at 42 School</p>
</div>