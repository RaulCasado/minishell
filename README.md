🐚🐚🐚 Minishell - A Miniature Bash Shell Implementation
Overview a.k.a everything we know about the project and how we did it.

Minishell is a simplified Unix shell built as a school project for the 42 curriculum. It replicates some of the functionalities of the Bash shell, allowing users to interact with the operating system via a command-line interface. The goal of the project is to create a functional shell from scratch in C, reinforcing understanding of process creation, inter process communication (pipes), signal handling, and parsing user input according to shell grammar.

This shell supports interactive command execution with features like pipelines, I/O redirection (including heredocs), environment variable expansion, and a set of built-in commands. While not fully as powerful as Bash, Minishell provides the essential behavior needed to run and manage processes in a Unix-like environment. It follows POSIX standards for shells in many aspects, handling things like quoting and exit status codes similarly to Bash. By building Minishell, we learned how shells work under the hood, how to parse command syntax, manage file descriptors, create child processes, and handle signals gracefully all within the constraints of C and the libraries allowed in the 42 project.
Features

Minishell implements many key features expected of a basic shell:

    Interactive Prompt & History: Displays an interactive prompt (Minishell> ) and reads user input using the GNU Readline library 

    Built-in Commands: Supports a set of shell built-in commands executed internally: echo, cd, pwd, export, unset, env, and exit. (Details on each in the next section.)

    External Command Execution: Launches external programs by forking the current process and using execve. The $PATH environment variable is used to find executables, and the shell reports errors (command not found, permission denied, etc.) just like Bash.

    Pipes (|): Implements piping, allowing the output of one command to write its value to other command. Minishell can chain multiple pipes to connect several commands in a pipeline.

    Redirection: Handles I/O redirection similar to Bash:

        Output redirection to a file with overwrite (>), or append mode (>>).

        Input redirection from a file (<).

        Heredoc (here-document <<): Allows multi-line input to be provided as a temporary input file, using a delimiter word to end the input. For example, <<EOF reads subsequent lines until an EOF line is seen.

    Quotes and Expansion: Supports single ' and double " quotes:

        Single quotes preserve the literal value of each character inside them (no expansion).

        Double quotes preserve most characters but still allow environment variable expansion (e.g. $HOME) and the special $? variable (last command´s exit status) inside.
        Minishell expands environment variables in commands just like a real shell (e.g. typing echo $USER will substitute the value of the USER variable). If a variable is not defined, it expands to an empty string.

    Environment Variable Management: The shell maintains its own copy of environment variables. The export and unset built-ins allow setting new variables or removing them. These affect the environment for subsequent commands.

    Exit Status: After each command execution, Minishell stores the return status (exit code) which can be accessed via $?. Built-ins and external commands set this status (following Bash conventions for error codes).

    Signal Handling: The shell properly handles terminal signals in interactive mode:

        Ctrl+C (SIGINT) interrupts the current input or running command. In the prompt, it causes a new prompt on a new line (just like Bash, it doesn´t exit the shell). If an external command is running, Ctrl+C will terminate that command (but not the shell).

        Ctrl+D (EOF) on an empty line signals end-of-input: Minishell will recognize this as a request to exit the shell (just as Bash exits on EOF).

        Ctrl+\ (SIGQUIT) is ignored in the interactive shell (so pressing it won´t quit Minishell or produce core dumps).

Despite implementing many features, Minishell omits some advanced Bash capabilities (job control, aliases, subshells, wildcard globs, logical operators &&/||, etc.), focusing on the core mechanisms of a Unix shell in a constrained, educational context. The result is a functional shell suitable for everyday command execution and scripting within its supported feature set.

Installation

Prerequisites

To build and run Minishell, you need:

    GNU/Linux or macOS environment (Minishell uses POSIX APIs).

    GCC or another C compiler.

    GNU Make build system.

    The readline library installed (for input editing and history). On Linux, this usually means having the readline development package (e.g. libreadline-dev) available.

Building the Project

Clone the repository and compile the source using the provided Makefile:

# Clone the repository
git clone https://github.com/RaulCasado/minishell.git
cd minishell

# Build the Minishell executable
make

This will produce an executable called minishell. All required components (including our custom libft library of common C functions) will be compiled and linked.

Running Minishell

After a successful build, launch the shell with:

./minishell

You should see the custom prompt appear, for example:

Minishell> 

At this point, you can start typing commands. Minishell will execute the command and then return to the prompt. You can run it within a normal terminal; it behaves similarly to Bash or other shells for supported features.

Note: Minishell does not accept command-line arguments when launched. If you try to provide arguments to the minishell executable, it will display an error and exit. (This design is intentional - Minishell is meant to be started with no arguments and then used interactively.)

Usage and Examples

Once running, Minishell reads input from the user and executes commands. Here are a few example interactions to illustrate its functionality:

Minishell> echo "Hello, world!"
Hello, world!

Minishell> pwd
/home/username/projects/minishell

Minishell> cd /tmp
Minishell> pwd
/tmp

Minishell> ls -l | grep ".c" > c_files.txt

You can also use environment variables and expansions:

Minishell> echo $HOME
/home/username

Minishell> export MYVAR="42 Malaga"
Minishell> echo The school is $MYVAR
The school is 42 Malaga

Here we set a new environment variable and then echo it with other text; the variable $MYVAR is expanded to its value. If you run env, you will see MYVAR in the list of environment variables for the shell.

To terminate the shell, use the exit built-in:

Minishell> exit

This will clean up and close Minishell. (As mentioned, pressing Ctrl+D at an empty prompt will also exit.)

Minishell prints error messages and status codes similar to Bash for invalid inputs or issues. For example, if you try to run a command that doesn´t exist:

Minishell> foobar
foobar: command not found

If you don´t close the quotes properly, it will also inform you:

Minishell> echo "Hello
Minishell: Syntax error: unclosed quotes

Minishell strives to inform the user of what went wrong without crashing. Next, we´ll see how Minishell is designed internally to achieve these behaviors.

Project Structure and Architecture

I think this is the worst part of the project since we didn´t have a clear idea of how to do it. We had to refactor a lot of code and we ended up with a project that is not very modular. We will try to explain the structure and the architecture of the project.

The project is organized into several source files and directories, each responsible for a specific part of the shell´s functionality. The main components are:

    src/ - Contains the main source code files for the shell. This includes the lexer, parser, executor, built-ins, and signal handling.

    includes/ - Contains header files that declare the functions and data structures used in the source files.

    libft/ - A self-written library of common C functions used throughout the project.

    Makefile - The build script to compile the project.

    README.md - This file, which provides an overview of the project and its usage.

Key components:

    libft/ - Our self-written library of standard C functions (e.g. ft_strlen, ft_split, ft_strdup, etc.), used throughout the project instead of the standard library (as required by 42 coding norms).

    main.c - Initializes the shell, enters the main read-execute loop, and handles final cleanup.

    Lexer/Tokenizer - Responsible for breaking the raw input line into a stream of tokens. This handles quoting and special characters.

    Parser - Takes the tokens produced by the tokenizer and checks if there are any syntax errors. It builds a linked list of command structures (t_command) that represent the commands to be executed. Each command contains information about its arguments, input/output redirections, and whether it´s part of a pipeline.

    Executor - Runs the parsed commands. For pipelines or external commands, it forks child processes and sets up pipes and redirections, then uses execve to run external programs. It also contains helpers for waiting on processes and obtaining exit statuses.

    Built-ins - Contains one file per built-in command (or related group) implementing the internal logic for echo, cd, etc. These functions manipulate the shell state (t_minishell) as needed (for example, updating the environment array on export/unset).

    Signals - Contains code to set custom signal handlers for SIGINT and SIGQUIT. This ensures the shell behaves correctly when the user sends interrupts or EOF.

    Utils - Various helper functions, such as string utilities (beyond those in libft), error printing, memory management (e.g., functions to free lists of tokens or commands), and file descriptor handling helpers.

This separation into files means each part of the shell´s job is handled in isolation: the lexer knows how to scan a line of text but nothing about executing commands, the executor doesn´t worry about how the command was parsed, etc. This modular architecture makes the program more understandable and allowed us to test and develop each piece independently.

Inner Workings: Input to Execution Flow

Minishell´s core is an event loop that reads input, processes it, executes commands, and repeats (this sounds kinda easy but trust me it is not :c ). Here´s a step-by-step breakdown of what happens internally for each command line you enter:

    Read Input: The shell displays the prompt (Minishell> ) and waits for the user to type a command line. We need to use readline() from the Readline library to handle this. This provides editing features and a command history. After the user presses Enter, readline returns the input line as a string.

        If readline() returns NULL, it means an EOF (End Of File) condition occurred (typically, the user pressed Ctrl+D or something went really really bad). In that case, Minishell interprets it as a signal to exit the shell. It will perform cleanup and terminate the loop.

        The shell also handles SIGINT (Ctrl+C) during input: our custom handler catches the signal and prints a new line, then re-displays the prompt. This way, an interrupt aborts the current input line but does not exit the shell. (This behavior is the same as Bash - hitting Ctrl+C at an empty prompt just gives a fresh prompt on a new line.)

    Before going to the next step I want to mention that readline can cause some memory leaks as said in the project description. So don´t worry if you see some leaks in your project. We tried to avoid them as much as possible but we couldn´t get rid of all of them. We used valgrind to check for memory leaks and we got some of them but not all.

    Input Validation: Before heavy processing, Minishell does some quick validations:

        It checks for an empty line (or line consisting only of whitespace). If the user just pressed Enter, the shell does nothing and immediately returns to step 1 for a new prompt.

        (Optional) It can check for certain syntax errors in a rudimentary way (for example, an input line that begins or ends with a pipe |, or contains an unclosed quote). Some of these checks are folded into the tokenization/parsing phase, described next.

    Lexical Analysis (Tokenization): The first major processing step is to tokenize the input string. The Lexer module goes through the input character by character and groups characters into meaningful tokens:

        Words: sequences of characters that form a command name or argument. By default, tokens are split by whitespace. For example, in ls -l /usr/bin, the lexer will produce three word tokens: "ls", "-l", and "/usr/bin".

        Operators: special symbols or sequences that have a meaning in shell grammar. Minishell recognizes the pipe symbol |, the redirection symbols >, >>, <, << as distinct tokens. These tokens will later instruct the parser how to build the command structure.

        Quotes: The lexer handles quotes by not splitting tokens on spaces inside quotes. If the user input contains text within single ' ' or double " " quotes, that content is kept as part of a single token (with the quotes initially still present). For example, echo "Hello world" results in two tokens: echo and "Hello world" (including the quotes at first). The quotes are handled specially rather than treated as normal characters. This caused some confusion in our implementation, as seen in the commits name (Please, don´t judge us this project is kinda hard).

        Escape Characters: In Bash, backslashes can escape characters, but per the project specification, Minishell does not handle the backslash \ as an escape (it´s simply treated as a regular character, or in our case we consider an unescaped \ outside quotes as an invalid character since the project does not require implementing general escapes).

    The lexer basically breaks the input into a linked list of t_token structures. Each token has a type (WORD, PIPE, REDIR_IN, REDIR_OUT, HEREDOC, etc.) and a string value. During this phase, we also classify >> as a single token of type "append redirection" and << as HEREDOC. The tokenization process also identifies unclosed quotes or invalid sequences. For instance, if there's a quote without a matching closing quote, the lexer can flag an error. Or if special characters like ; or & appear (which our Minishell doesn´t support as operators), it can mark that as a syntax error. If any of this issues are found, Minishell will print an error message (e.g., "Syntax error: unclosed quotes") and skip execution for this line, returning the prompt for a new input.

    After tokenizing, Minishell performs a syntax check on the token list to catch errors like:

        Errors like consecutive pipes (||) or redirections (>> or <<) without a command in between. For example, "ls || echo" is invalid.

        Redirection operators not followed by a filename token.
        If this patterns are found, it prints a syntax error message and jumps to cleanup for the next cycle.

    Environment Variable Expansion: Once we have a list of tokens, Minishell processes expansions on them (this is typically considered part of the parsing phase, but conceptually it's a distinct step). For each token of type WORD, the shell scans the string for the $ character:

        If $? is found, it is replaced with the string representation of the last command´s exit status (we have the exit status saved as an integer so we need to parse it to string everytime we need to print the value maybe we could have just stored the same value but with different type this could be a good idea). This allows constructs like echo $? to display the last return code.

        If $ANY is found (where ANY is an environment variable name), it gets replaced with the value of that environment variable. The shell looks for this value in the environment list. If found, the token´s value is modified to be that value. If not found, the $ANY is removed (empty string substitution). For example, if USER is set to bob, a token "Hello $USER!" becomes "Hello bob!" after expansion. If USER was not set, that token would become "Hello !" (this is intended Bash does the same).

        Edge cases: a lone $ with no name, or $ followed by a space, doesn´t expand to anything (effectively removed). Also, in single quotes, we do not perform expansion. In echo 'Path: $PATH', the $PATH remains literal because the entire token is single-quoted. Our lexer and expander handle this by not marking $ as special when inside single quotes. (If inside double quotes, expansion still happens)


    After expansion, the token list is updated with any $ANY replaced. If an expansion results in an empty string, the corresponding token might become an empty word in this cases we handle it by either removing the token or marking it appropriately so it won´t cause issues (for example, an empty argument is effectively ignored). Finally, we remove any quote characters from tokens now that their job (grouping text and preventing expansion) is done. So "Hello world" (with quotes) becomes a token with value Hello world without quotes, but still counted as one argument. This was not behavior we had at first, but we realized it was needed to match Bash behavior. Since we had some problems with inputs like echo       ""  ""  "" Hello World or things like this.

    Parsing (Building Commands): The parser now takes the stream of tokens and assembles commands. We create a linked list of t_command structures, each representing one command in the pipeline (or a single command if there are no pipes). During this process:

        We iterate through the token list in order. We accumulate consecutive WORD tokens as part of the current command´s arguments list. For example, tokens [ echo ][ Hello ][ world ] (all WORD types) will form one command with args ["echo", "Hello", "world"].

        When we encounter a redirection token (<, >, >>, <<), we take the token after it as the target. The parser will set the current command´s input or output file accordingly:

            < filename sets the command´s input source to filename (we store this as cmd->infile = "filename").

            > filename sets output redirect to filename (with overwrite mode). >> filename is similar but marked as append mode. We have a variable that indicates if the output should be appended or truncated.

            << DELIM triggers a heredoc: the shell will read from stdin until it encounters the delimiter DELIM (on a line written by the user), and save all the lines into a temporary file. The cmd->infile for this command is then set to that temporary file´s path. We implemented heredoc by creating a unique file in /tmp (something like /tmp/minishell_heredoc_12345 with a random suffix or incremental number) and writing the heredoc content into it. Signal note: If the user hits Ctrl+C during a heredoc input, Minishell should abort the heredoc and return to the prompt our implementation takes care to handle signals in this scenario (by checking for interrupts during heredoc reading).

        If we see a pipe token |, this indicates the end of one command and the start of another in a pipeline:

            We finalize the current command being built and link it into the t_command list.

            We then start a new command structure for the tokens after the pipe. We also mark that the previous command´s output is piped out, and the next command´s input is piped in. (In the t_command struct, we have flags or fields like pipe_out and pipe_in to denote these connections for later.)

        The parser effectively groups tokens into commands until it runs out of tokens.

    After parsing, we have a linked list minishell->commands of commands ready to execute. Each command contains:

        An array of argument strings (cmd->args), where args[0] is the command name and the rest are arguments. This array is NULL-terminated, so it can be directly passed to execve.

        Fields for input/output:

            cmd->infile (if not NULL, it´s a filename to open for stdin for this command).

            cmd->outfile (if not NULL, filename to redirect stdout to).

            A flag whether outfile is append or truncate mode.

        Piping flags or next pointers:

            If this command´s output should go into a pipe we have cmd->pipe_out = 1.

            If its input comes from a previous pipe, cmd->pipe_in = 1.

            The cmd->next pointer links to the next command in the pipeline (if any).

    The parser also catches any syntax errors that couldn´t be detected earlier. For instance, if a redirection token was at the very end of the input with no filename after, or if a pipe was the last token (meaning no next command), the parser will identify that cmd->args might be empty or a needed token is missing. In this cases, we set an error state. Minishell would then free the partially built structures and return to the prompt without executing (printing an error like Syntax error: unexpected token).

    Execution Phase: With the commands prepared, Minishell now executes them:

        Built-in Command Check: Minishell checks if the pipeline contains a single command and that command is one of the built-in commands (echo, cd, pwd, export, unset, env, exit). If yes, it will execute that built-in directly in the current process, without forking. This is important for commands like cd or export, which need to modify the shell´s own state (current directory and environment) if we forked and did these in a child, the changes would not reflect in the main shell. This right here is such a big problem that could take you some time to figure out, since in the beginning we thought that we could just fork everyting but no.

            To execute a built-in, we simply call the corresponding C function, e.g. builtin_cd() or builtin_export(), and pass it the shell state. These functions return an exit status code which we assign to minishell->exit_code. (They also handle printing any output or error messages on their own.)

            After a built-in is done, since it didn´t create a new process, we skip the usual forking and go straight to cleanup for the next prompt.

        If the command is not a built-in or there are multiple commands in a pipeline, Minishell will use the fork/exec approach for execution. Even a single external command (not built-in) is run via fork and exec to isolate it from the main shell.

    Pipeline Forking: Executing a pipeline of N commands involves N processes (for N commands) that run concurrently, connected by pipes. Minishell´s executor handles this as follows:

        It sets up the needed number of pipes. A pipe in Unix is a pair of file descriptors (read end and write end). For a pipeline of two commands, one pipe is needed. In general, for N commands, N-1 pipes are needed to connect them. Our implementation creates each pipe on the fly as we fork the next process.

        It then enters a loop over the t_command list:

            For each command, it checks if it has any valid arguments. (If somehow a command node has no args which can happen in certain error cases like an empty command between pipes we simply skip it and move to the next. This prevents trying to execute a non-existent command.)

            It forks a new child process using fork(). In the child:

                Redirections: Prior to executing the command, the child sets up its standard input/output according to any redirections:

                    If the command has an infile, the child opens that file and dup2 it onto STDIN (file descriptor 0). This makes the command read from the file as if it were standard input.

                    If the command has an outfile, the child opens/creates that file with appropriate flags (O_TRUNC or O_APPEND) and dup2 it onto STDOUT (fd 1). This makes the command´s output go to the file.

                    If the command is part of a pipeline (not the last command), the executor will have created a pipe. The child will dup2 the pipe´s write end onto STDOUT so that its output goes into the pipe instead of the terminal or file. On the other hand, if the command is not the first in a pipeline, it will have inherited a pipe read end for its STDIN from the previous iteration the child dup2s that onto STDIN to get input from the previous command.

                    After duplicating, the child closes the original pipe file descriptors (we close the ends we don´t need to avoid leaks). For this we created the better_close function that closes the file descriptor and checks for errors. This is important to avoid file descriptor leaks in the child process.

                Executing the Command in Child: After setting up redirections, the child process calls the function execve() if the command is an external program. We build the full path for the command:

                    If the command name contains a / or starts with ./ or /, we treat it as a path and try to execute it directly.

                    Otherwise, we search through the directories in the PATH environment variable to find an executable file that matches the command name. This is done by concatenating each path with the command name and checking access() for execute permission. This is why if you unset the PATH variable, the shell will not find any commands unless you provide a full path. We also check if the command is a built-in before searching in PATH.

                    If a matching executable is found, execve is called with that path, the args array, and the current environment array.

                    If the execve call succeeds, it begins execution. If execve fails, we handle errors:

                        If no command was found in PATH, the child prints "command: command not found" and exits with code 127.

                        If execve returns an error (like permission denied or not a valid executable format), we print a message ("...: Permission denied" for code EACCES, or "...: Is a directory" if trying to execute a directory, etc.) and exit with code 126 or appropriate value. We took care to match Bash´s conventions for these exit codes.

                    If the command is a built-in even in a pipeline context, we don´t use execve; instead we directly call the built-in function in the child process. For example, if the pipeline is export VAR=42 | other_command, the first process (child) will run our builtin_export function. This won´t affect the parent´s environment, but that´s fine because in a pipeline, each segment runs in a subshell in real Bash as well. The built-in in a child will still produce output or effects for that child process (like export setting an env var that would only live for that subprocess lifetime). The child then exits with the status code returned by the built-in.

                After execve or the built-in function call, the child calls exit(exit_code) to terminate, if it hasn´t been replaced by execve. (In case of a successful execve, the child will terminate when the executed program finishes on its own). Watch out with exit this is a common mistake we made in the beginning. If you forget to exit the child after execve, it will continue executing the rest of the shell code and may cause undefined behavior or crashes. Another common mistake is to exit when you are not in a child process. This can happen if you forget to check if the process is a child before calling exit. In this case, the shell will terminate and you will not be able to use it anymore(well just relaunch it but you know what I mean).

            Meanwhile, in the parent process (the main shell):

                After forking each child, the parent sets up for the next command in the pipeline. It keeps track of the read end of the current pipe so it can use it as the input for the next command. For example, after creating a pipe for command1 -> command2, and forking command1, the parent closes the write end (because it´s now handled by child1) but keeps the read end open. When it moves to command2, it will use that read end as the STDIN for command2´s child.

                The parent also closes the previous command´s pipe read end if it´s no longer needed (to avoid file descriptor leakage).

                It repeats the fork logic for each command in the pipeline.

            The parent records the PID of the last command´s child process (this is important to get the right exit status).

        Waiting for Children: After forking all commands in the pipeline, the parent shell uses waitpid()/wait() to wait for all child processes to finish. We specifically note the exit status of the last command in the pipeline, because that becomes the shell´s own exit status ($?). For example, in cmd1 | cmd2, if cmd2 exits with status 0, and cmd1 perhaps exited with 1, the shell´s $? will be 0 (similar to Bash´s rule that $? is the last command´s status in a pipeline).

            We also handle signals here: if a child was terminated by a signal (like SIGINT), we translate that into an exit status as Bash does (128 + signal number). For instance, if you hit Ctrl+ C and it killed a child process, that child´s status might be "terminated by SIGINT" we would set the shell´s exit code to 130 in that case (128+2 for SIGINT). This way $? correctly reflects an interruption.

        Edge-case adjustments: If a pipeline was started but one command in the middle had a redirection error (input file not found), we had logic to avoid executing the remaining pipeline commands unnecessarily. For instance, if you run cat < nonexisting | grep foo, the shell will detect "No such file or directory" for nonexisting before forking cat. We print the error, skip starting cat, but we still need to launch grep foo with an empty input (since the pipeline expects something). In our implementation, we handle this by redirecting grep´s input from /dev/null in such cases so that it runs and immediately sees EOF. This is a subtle behavior to make sure the pipeline semantics are preserved (Bash does something similar).

        After all children have exited and we collected their statuses, the execution phase is complete. At this point, minishell->exit_code is set to the exit code of the last command.

    Cleanup: Once a command (or pipeline) has been executed, Minishell performs cleanup before returning to the prompt:

        It frees the data structures allocated for tokens and command lists for this cycle. All the t_token and t_command linked structures are deallocated to avoid memory leaks.

        It also closes any file descriptors that were kept open in the parent (leftover pipe ends, even though our logic typically closes them in the parent after forking each child).

        If a temporary heredoc file was created, the shell can delete it at this point. (In our case, we generate unique filenames in /tmp for heredocs. We opted not to immediately unlink them to avoid issues on some systems, but we ensure they don´t conflict and they´ll be removed on reboot or can be cleaned up later. This could be improved by unlinking right after opening, so it deletes when closed.)

        The shell resets any state if necessary for the next loop iteration (for example, clearing any flags in the t_minishell struct, resetting signal handling to ensure signals are caught again in the prompt context rather than in children).

    The shell then goes back to step 1, printing a new prompt and waiting for the next command. This loop continues until an exit condition is met (the user types exit or sends EOF).

In pseudocode, the main loop looks roughly like this:

initialize_shell_state(&minishell);
setup_signals();
while (running) {
    char *input = readline("Minishell> ");
    if (input == NULL) {    // EOF (Ctrl+D)
        break;
    }
    if (is_empty(input)) {
        free(input);
        continue;
    }
    add_history(input);
    tokens = tokenize_input(input);
    if (!tokens or syntax_error_in(tokens)) {
        // print error if needed
        free(input); free_tokens(tokens);
        continue;
    }
    expand_environment(tokens);
    commands = parse_tokens(tokens);
    if (!commands) {
        // syntax error (already handled), cleanup and continue
        free(input); free_tokens(tokens);
        continue;
    }
    execute_commands(commands);  // forks and execs as needed
    // exit_code is set inside execute_commands or builtin calls
    free(input);
    free_tokens(tokens);
    free_commands(commands);
}
cleanup_shell_state(minishell);

This illustrates the flow: read input, tokenize, parse, execute, and loop.

## Built-in Commands

Minishell implements the following built-in commands internally (without calling external binaries). These behave similarly to their Bash counterparts:

| Command | Description | Usage example |
| ------- | ----------- | ------------- |
| `echo` | Display text to standard output. Supports the -n option to omit the trailing newline. | `echo "Hello world"`<br>`echo -n "No newline"` |
| `cd` | Change the current working directory of the shell. Updates PWD and OLDPWD environment variables. | `cd path/to/dir` |
| `pwd` | Print the current working directory. Outputs the value of PWD or the actual directory. | `pwd` |
| `export` | Set or update environment variables. With no arguments, it prints all environment variables (sorted, in our implementation). Accepts arguments of the form NAME=value to set variables, or NAME to mark as exported (shell will ensure it exists). | `export VAR=42`<br>`export PATH=$PATH:/my/bin` |
| `unset` | Remove variables from the environment. Takes variable names as arguments and deletes them from the shell's environment list. | `unset VAR` |
| `env` | Display the shell's environment variables (similar to running the external /usr/bin/env but this is internal). It simply prints each KEY=value pair. This command ignores any arguments (Minishell will print an error if arguments are given, since env doesn't take options in this context). | `env` |
| `exit` | Exit the shell. exit can take an optional numeric argument which will be the return code of the shell process. If more than one argument is provided, it prints an error and does not exit. | `exit`<br>`exit 42` |

Some additional details on built-ins:

    echo: We implemented the -n flag to suppress the newline (like Bash). Other flags are not in scope, so echo -n hello will output hello without a newline, and echo -e or other flags are not recognized (they´ll just be treated as strings to echo). Echo handles quoted text and variable expansion via the parsing stage (so echo "$HOME" will print the home directory path).

    cd: Supports relative and absolute paths. After a successful directory change using chdir(), it updates the PWD environment variable to the new path and also updates OLDPWD to the previous value of PWD. If cd is used with no arguments, our Minishell currently prints a usage message (in Bash, cd with no args goes to $HOME; implementing that could be a potential improvement, but was not mandatory). If too many arguments are given (cd arg1 arg2), it prints an error "too many arguments". If the path is invalid, it prints the system error ("No such file or directory"). We took care to use getcwd() to get the actual path for PWD (for reliability in case of symbolic links, etc.).

    pwd: Uses getcwd() to retrieve the current directory and prints it. If getcwd fails (which would be rare, if in a directory that no longer exists), it prints an error using perror. Typically, it should always succeed after a valid cd.

    export: We allow multiple variables to be exported in one command (export VAR1=val1 VAR2=val2). Each argument is processed:

        If it´s not a valid identifier (starts with a number or contains invalid chars), we print an error "export: not a valid identifier" and continue to the next one (the shell doesn´t exit it just returns a non-zero status).

        If it´s of the form NAME=value, we either create a new environment variable or update an existing one. The NAME must be a valid identifier. Our implementation updates the minishell->envp array. If NAME exists, replace its value and if not, add a new string in the form "NAME=value".

        With no arguments, export prints all environment variables in a sorted order, one per line, prefixed with declare -x (for Bash compatibility in output format). For example, it might print: declare -x PATH="/usr/bin:/bin" for each variable. We implemented sorting and this output format to mimic Bash as closely as possible.

    unset: For each argument, remove that variable from the environment list if it exists. Unsetting a variable is basically finding it in the envp array and freeing that string, then shifting the remaining entries down by one. (We maintain envp as a NULL-terminated array of strings, so after removal we ensure the array still ends with NULL and no gap.)

    env: Simply iterates through the envp array and prints each string (which is already in KEY=value format). If any argument is passed to env, our Minishell prints an error (env: too many arguments). This is because, unlike the external /usr/bin/env which can run programs with a modified environment, our built-in env is just for displaying variables.

    exit: If no argument is given, exit the shell with the current minishell->exit_code (the status of the last executed command). If one numeric argument is given, we convert it to an integer (if the number is outside the range of an unsigned char 0-255, Bash wraps it around, but we simply take it as an int and return its modulo 256 as exit code). If the argument isn´t a pure number (e.g., exit abc), we print "exit: numeric argument required" and exit with code 2 (following Bash behavior). If more than one argument is given, we print "exit: too many arguments" and do not exit (in interactive mode, Bash stays alive if you try to exit with too many args). In our case, we set the status to 1 for that error and continue running.

Each built-in´s implementation carefully manages errors and the shell´s state. For example, cd prints errors to stderr but does not exit the program on failure, it simply returns a nonzero status. The errors messages could be different from the ones in Bash, but we tried to match them as much as possible. This messages also vary depending on the system you are using. For example, if you try to cd to a directory that does not exist, the error message will be different depending on the system you are using.

Signal Handling

One of the challenging parts of shell development is dealing with signals, especially in an interactive context. Minishell sets up custom signal handlers to achieve the following:

    Ctrl+C (SIGINT): In the shell´s main prompt, this should interrupt the current input and show a new prompt on the next line. We accomplish this by installing a handler that writes a newline to stdout, then uses rl_replace_line("", 0) and rl_on_new_line() and rl_redisplay() from the Readline library to reset the prompt state. This way, the user sees a fresh prompt immediately. The shell´s main loop does not terminate; it simply continues as if the user had entered an empty line. If a child process is running (an external command), we do not override the default for SIGINT in that child, so pressing Ctrl+C will also send SIGINT to the child process, usually killing it. The parent shell notices that the child was killed by SIGINT and will set its own $? accordingly, but will not itself exit.

    Ctrl+\ (SIGQUIT): We set this signal´s handler to do nothing (yes just ignore it) in the shell. In an interactive Bash, Ctrl+\ usually does nothing visible (it would quit a running process by default, dumping core, but shells typically ignore it). Our shell´s parent process ignores SIGQUIT so that it won´t terminate. For child processes (like running cat or other commands), once again we revert to default behavior in the exec´d program, so if an external process doesn´t handle SIGQUIT, pressing Ctrl+\ will terminate that process (and typically produce a "Quit (core dumped)" message). However, our parent shell will not be affected and will just report the termination like any other signal-caused termination.

    Ctrl+D (EOF): This isn´t a signal but an input condition. We detect it by readline() returning NULL. Our reaction is to exit the shell. We call a cleanup routine and break out of the main loop. We also had a mechanism with a global flag g_signal to communicate the intent to exit. Specifically, on EOF we set g_signal = 0 and use that to break the loop. This global is also checked in our loop condition to allow external interruption of the loop if needed. (We used a global because signal handlers in C can´t easily communicate complex info, so a common trick is to set a global flag and read it in the main loop.)

Implementation detail: We used the sigaction API to set up these handlers with SA_RESTART flag for SIGINT and SIGQUIT. SA_RESTART ensures that certain system calls (like the blocking readline call) are automatically restarted if interrupted by a signal, which is important to smoothly continue after our handler runs.

We also took care to turn off readline´s default signal catching (rl_catch_signals = 0) so that our handlers get called, not readline´s, otherwise readline would override some of this logic.

In summary, signal handling was implemented to mirror the interactive shell experience and ensure the shell process itself isn´t accidentally killed by user key presses, while child processes are properly affected.
Error Handling and Edge Cases

Throughout Minishell, we paid special attention to error handling and unusual scenarios to make the shell robust. Here are some edge cases and how they are handled:

    Unclosed Quotes: If the user enters a line with an odd number of quotes (echo "Hello without a closing quote), the lexer will detect that a quote was opened and not closed. We print an error message like Minishell: Syntax error: unclosed quotes and discard the input (no execution happens). The shell then prompts again for a new command. (Bash in this scenario would actually continue to a secondary prompt waiting for closure, but our project specification allowed us to simply throw an error and reset, which is what we do.)

    Pipes with Missing Commands: If the line starts with a pipe or has two pipes in a row, there isn´t a command where expected. For example, input like | ls or ls || grep foo is invalid. Minishell will catch these during syntax check or parsing. We output an error such as Minishell: Syntax error: unexpected token '|' and skip execution.

    Redirection Syntax Errors: Similar to pipes, if a redirection operator is not followed by a valid filename token, we detect this. e.g. cat < (with nothing after) is a syntax error. The shell will report Syntax error: unexpected token after '<' or a similar message. We made sure to do this check before execution because trying to execute such a command would have no sense.

    Invalid Redirection Targets: If a file that is supposed to be opened for input redirection does not exist, Minishell prints an error: minishell: filename: No such file or directory, and it does not execute the command (just like Bash wouldn´t execute a pipeline segment if a needed redirection file is missing). If an output file cannot be created or opened (due to permission, for example), we also print the system error (Permission denied, etc.) and skip executing that command.

    Too Many Arguments to Built-ins: As mentioned, built-ins like exit and cd check for excessive arguments. exit 1 2 will output Minishell: exit: too many arguments and not exit the shell. cd arg1 arg2 outputs Minishell: cd: too many arguments and does nothing else (returns an error code). This way, scripts or usage that misuse these built-ins get feedback.

    Numeric argument errors to exit: exit abcd where "abcd" is not a number will trigger Minishell: exit: numeric argument required and the shell will exit with status 2. We also free memory in that case before exiting.

    Permission Errors and Command Not Found: If you try to execute a file that isn´t executable or a directory name as a command, our exec error handler will catch the errno. For example:

        ./script.sh where script.sh is not executable: the child process will get errno=EACCES and we print script.sh: Permission denied and exit that child with code 126.

        Trying to execute a directory: ./myfolder where myfolder is a directory, execve will fail with EACCES or might succeed in opening then fail with a specific error we use stat() to detect if the target is a directory and print myfolder: Is a directory, returning 126.

        Unknown command: xyzcommand not found in PATH we print xyzcommand: command not found and return 127. This is exactly what Bash does.

    Memory Management: We considered memory leaks as "errors" to avoid as well. Our implementation frees all allocated memory for each command line after use. The minishell struct holds the environment and other persistent data, which we free at shell exit. We used tools like Valgrind during development to ensure that no leaks or unreachable memory remain after running a series of commands and exiting. Pls don´t be to harsh with us, we are not memory management experts and we are still learning. We tried our best to free everything we allocated, but if you find a leak please let us know :D.

    Large inputs or many commands: Minishell can handle reasonably large input lines and multiple pipes. The limits would be primarily system-level (for example, too many processes in a pipeline might hit the process limit or too many open file descriptors if you try an extremely long pipeline). However, typical usage like a pipeline of, say, 10 commands, or input lines a few thousand characters long (with many arguments) should work. We did not impose an artificial limit on input length besides what readline can handle.

    Environment limitations: Our envp is dynamically managed. If you export a lot of variables or very long values, it will realloc and handle them. One edge case: export =foo (no variable name) is an error according to our validation (invalid identifier). We also do not allow names with = in the middle (except to separate name and value at the first =). So export VAR1=foo VAR2=bar works, but export VAR1=foo=bar would treat VAR1 with value foo=bar (since we only split on the first =).

Overall, Minishell tries to handle error cases in the best way possible: print a helpful error message (prefixed with minishell: and the relevant command or file), and not crash or hang. After an error, it cleans up the state and returns to the prompt for a new input, rather than getting stuck. This resilience was tested with many combinations of wrong inputs to ensure the shell doesn´t break.

Testing and Stability

We undertook extensive testing of Minishell to ensure it is stable and behaves as expected. The development involved:

    Manual Interactive Testing: We frequently ran the shell and tried typical shell sessions, mixing built-ins, external commands, pipelines, and redirections. For instance, we tested sequences like:

        echo hello | cat -e > out.txt (pipeline with redirection at end)

        export X=42; echo $X | grep 42 (variable export then usage in pipeline)

        unset PATH; ls (to see "command not found" when PATH is missing, then export PATH=/bin:/usr/bin and try ls again)

        cd into various directories, including cd with no args or invalid dirs.

    Comparison with Bash: For many cases, we compared Minishell´s output and behavior to Bash. While not every detail is identical (especially in areas we didn´t implement, or minor differences like certain error message wording), our goal was that any script using the allowed features would produce the same results on Minishell as on Bash. We wrote sample shell scripts (using only supported features) and ran them under Minishell by feeding commands one by one, comparing to running under Bash. This helped catch discrepancies in things like exit codes and quoting.

    Valgrind (Memory Leak & Error Check): After implementing features, we ran Minishell through Valgrind with different scenarios to ensure there were no memory leaks or invalid memory accesses. Managing memory manually in C (especially with lots of strdup, splits, and linked lists) can be error-prone. We plugged every leak we found and ensured that even in error pathways (where we early-abort execution on an invalid input), we free any partially allocated data. By the end, running a series of commands and then exit showed zero leaks. This is important for stability if the shell were to run for a long time or in a loop within a script.

    Concurrency of pipes: We tried pipelines with multiple commands to ensure our pipe management doesn´t deadlock or mis-route FDs. E.g., ls | grep txt | sort -r | head -n 5 > out.txt. This touches multiple parts: pipe chaining, an external command with output redirection at end, etc.

    Stress tests: While limited by time, we gave the shell some stress tests like a very long argument list (hundreds of arguments) to see if our dynamic array allocations handle it. We also tested removing and adding environment variables repeatedly to see if our reallocation logic in export/unset is solid.

The result is a shell that is stable for normal use. It doesn´t crash under valid use cases, and it handles incorrect usage gracefully. There might still be corner cases (as with any complex program), but we eliminated all known bugs through iterative testing. The code was peer-reviewed by our teammates and also passed the 42 school evaluation.

Minishell was a challenging and rewarding project that resulted in a functioning shell program. By reimplementing shell features, we gained a deeper understanding of how command-line interpreters work at a low level. This README has covered the purpose and features of Minishell, instructions to install and run it, a deep dive into its architecture and execution flow, details of features and design choices, and how we handled various edge cases. We aimed to create a document useful for 42 students (to understand the project and our approach) and for potential recruiters or other developers (to showcase our coding practices and ability to handle complex systems programming tasks).

Minishell demonstrates proficiency in system calls usage (fork, exec, pipe, dup2, wait, signal), string parsing, dynamic memory management, and adhering to a specification to achieve a consistent user experience. While there´s always room to expand it, the current implementation should serve as a solid, stable mini-shell.
Contributors

This project was implemented by two 42 Málaga students as a collaborative effort:

- Raúl Casado Moreno - GitHub: [RaulCasado](https://github.com/RaulCasado)
- Daniel Roura - GitHub: [DanielRoura](https://github.com/DanielRoura)

We worked together on design decisions and debugging, performing extensive testing to ensure the shell works correctly.

Feel free to reach out to either contributor via GitHub for any questions, suggestions, or opp
ortunities. We are proud of Minishell and happy to discuss its implementation or potential improvements. Thank you for reading about our project!

Known Issues:

    The shell does not handle subshells or logical operators (&&, ||) as mentioned. It will throw an error if you try to use them.

    The heredoc feature is basic and does not support variable expansion inside the heredoc content.

    The signal handling is basic and could be improved for more precise behavior during certain operations.

    The shell does not support job control (background processes) or process suspension/resumption.

    There may be minor differences in error messages compared to Bash, but we aimed for consistency with common usage.

    When doing multiple redirections, it might not behave exactly like in 42 Málaga´s Bash, but we tried to keep it as close as possible. It behaves like a standard shell in this regard.

    As said in some edge cases, there might be some leaks or unhandled cases. We tried to cover most of them, but if you find any, please let us know.

And finally, we are not experts in C programming or shell development. We are still learning and improving our skills. If you have any suggestions or feedback on how to improve our code or the project in general, please let us know. We would love to hear from you!

With <3 from 42 Málaga, we hope you enjoy using Minishell as much as we enjoyed creating it!
