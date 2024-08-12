# NOT FINISHED YET ⚠️
# Hi, this project called minishell but the 's' is silent 👹.
This project is about creating a simple shell, developed by me (adbouras) and [(eismail)](https://github.com/ism417).

## What is a shell?!

In computer science, a shell is a user interface that allows users to interact with the operating system. It can be command-line based or graphical, 
but it’s most commonly associated with command-line interfaces (CLIs). Here are some key points about shells:

* Types of Shells:

	* `Command-Line Shells`: These include text-based interfaces where users type commands to perform tasks. Examples are:
		Bash (Bourne Again SHell): Commonly used on Linux and macOS.\
		Zsh (Z Shell): An extended version of Bash with more features.\
		PowerShell: A powerful shell for Windows with scripting capabilities.\
		Command Prompt: The traditional Windows shell for command-line operations.
	* `Graphical Shells`: These provide a graphical user interface (GUI) for users to interact with the system. Examples include desktop environments like GNOME or KDE.
	* `Usage`: Shells are used for a variety of tasks, including file manipulation, program execution, and system management.
		They are particularly useful for developers and system administrators who need to perform repetitive tasks or automate workflows.
	* `Customization`: Many shells support customization through configuration files (like .bashrc or .zshrc), allowing users to set aliases, functions, and appearance settings.

* Our shell should:
	* Display a prompt when waiting for a new command.
	* Have a working history.
	* Search and launch the right executable (based on the PATH variable or using a relative or an absolute path).
	* Avoid using more than one global variable to indicate a received signal. Consider the implications: this approach ensures that your signal handler will not access your main data structures.
	* Not interpret unclosed quotes or special characters which are not required by the subject such as (backslash) or ; (semicolon).
	* Handle `’` (single quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence.
	* Handle `"` (double quote) which should prevent the shell from interpreting the metacharacters in the quoted sequence except for $ (dollar sign).
	* Implement redirections:
		* `<` should redirect input.
		* `>` should redirect output.
		* `<<` should be given a delimiter, then read the input until a line containing the delimiter is seen. However, it doesn’t have to update the history!
		* `>>` should redirect output in append mode.
	* Implement pipes (`|` character). The output of each command in the pipeline is connected to the input of the next command via a pipe.
	* Handle environment variables ($ followed by a sequence of characters) which should expand to their values.
	* Handle $? which should expand to the exit status of the most recently executed foreground pipeline.
	* Handle `ctrl-C`, `ctrl-D` and `ctrl-\` which should behave like in bash.
	* In interactive mode:
		* `ctrl-C` displays a new prompt on a new line.
		* `ctrl-D` exits the shell.
		* `ctrl-\` does nothing.
	* Your shell must implement the following builtins:
		* `echo` with option `-n`
		* `cd` with only a relative or absolute path
		* `pwd` with no options
		* `export` with no options
		* `unset` with no options
		* `env` with no options or arguments
		* `exit` with no options

## I - Displaying the prompt & reading the command.
For displaying the prompt and getting the command from the terminal, we have the right to work with the function `read_line()` which does both actions at the same time.
```c
	#define PROMPT "minishell $ "

	char	*rl;

	while (1)
	{
		rl = read_line(PROMPT);
		add_history(rl);		// add rl to a working history.
		printf("%s\n", rl);
	}
	clear_history();			// frees the working history.
```
Now that we've read the command, the next step is to parse it. I’ve decided to proceed with lexing.











