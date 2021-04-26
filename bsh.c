/**
 * bsh - A UNIX-like shell written in C.
 */

#include <fcntl.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/wait.h>
#include <unistd.h>

#define MAX_TOKS 64
#define STR_MAX 1024
#define TOK_DELIM " \r\n\t\a"

extern char **environ;
static int num_pipes;
static bool backgnd;

/**
 * Read a line of input.
 *
 * @return
 *      A buffer containing the line that was read.
 */
char *read_input(void)
{
<<<<<<< HEAD
    char *buf = NULL;
    size_t len = 0;

    if (getline(&buf, &len, stdin) == -1)
    {
        return NULL;
    }
=======
	char *buf = NULL; /* line input buffer */
    	size_t len = 0;   /* number of bytes to read */

    	/* Read input from the user */
   	if (getline(&buf, &len, stdin) == -1)
    	{
        	return NULL;
    	}
>>>>>>> 4e6eb8042d2716aeb14d0db9a543f5c88ba837e1

    	return buf;
}

/**
 * Tokenize a string. This function checks for tokens specifying pipes and
 * and background processes.
 *
 * @param str
 *      String to be tokenized.
 *
 * @return
 *      Buffer containing the tokens.
 */
char **tokenize(char *str)
{
<<<<<<< HEAD
    int len = MAX_TOKS;
    char **buf = malloc(len * sizeof(char *));
    if (buf == NULL)
    {
        perror("malloc");
        exit(EXIT_FAILURE);
    }

    char *tok = strtok(str, TOK_DELIM);

    int pos = 0;
    while (tok != NULL)
    {
        if (strcmp(tok, "|") == 0)
        {
            num_pipes++;
            buf[pos++] = NULL;
        }
        else if (strcmp(tok, "&") == 0)
        {
            backgnd = true;
            buf[pos++] = NULL;
        }
        else
        {
            buf[pos++] = tok;
        }
        
        tok = strtok(NULL, TOK_DELIM);

        if (pos < len - 1)
        {
            continue;
        }

        len += MAX_TOKS;
        char **temp = realloc(buf, len * sizeof(char *));
        if (temp == NULL)
        {
            perror("realloc");
            exit(EXIT_FAILURE);
        }
        else if (buf != temp)
        {
            buf = temp;
        }
    }

  buf[pos] = NULL;

  return buf;
=======
	char **buf;         /* token buffer */
   	char **temp;        /* temporary buffer for reallocation */
    	char *tok;          /* current token */
   	int len = MAX_TOKS; /* length of the buffer */
    	int i = 0;          /* index variable */

    	/* Allocate space for the token buffer */
    	buf = malloc(len * sizeof(char *));
    	if (buf == NULL)
    	{
        	perror("malloc");
        	exit(EXIT_FAILURE);
    	}

	/* Get the first token */
	tok = strtok(str, TOK_DELIM);

    	/* Iterate over the string and add each token to the buffer */
   	while (tok != NULL)
    	{
        	buf[i] = tok;

        	/* Check for pipe and background tokens */
		if (strcmp(tok, "|") == 0)
        	{
            		num_pipes++;
           	 	buf[i] = NULL;
        	}
        	else if (strcmp(tok, "&") == 0)
        	{
            		backgnd = true;
            		buf[i] = NULL;
        	}

		/* Move to the next token */
		i++;
		tok = strtok(NULL, TOK_DELIM);

		/* Check if the length of the buffer has been exceeded */
		if (i < len - 1)
		{
		    continue;
		}

		/* Reallocate the buffer if necessary */
		len += MAX_TOKS;

		temp = realloc(buf, len * sizeof(char *));
		if (temp == NULL)
		{
		    	perror("realloc");
		    	exit(EXIT_FAILURE);
		}
		else if (buf != temp)
		{
            		buf = temp;
        	}
    	}

	/* Append a null byte to the end of the buffer */
	buf[i] = NULL;

	return buf;
>>>>>>> 4e6eb8042d2716aeb14d0db9a543f5c88ba837e1
}

/**
 * Redirect a file descriptor.
 *
 * @param name
 *      Name of the file to redirect to.
 * @param flags
 *      Flags used for opening the file descriptor.
 * @param dest_fd
 *      File descriptor to be redirected.
 *
 * @return
 *      Nothing.
 */
void redirect(char *name, int flags, int dest_fd)
{
    int new_fd = open(name, flags, S_IRUSR | S_IWUSR);
    if (new_fd != -1)
    {
        dup2(new_fd, dest_fd);
        close(new_fd);
    }
}

/**
 * Execute built-in commands.
 *
 * @param args
 *      Buffer containing the command and arguments.
 *
 * @return
 *      True if the command was built-in, false otherwise.
 */
bool handle_builtins(char **args)
{
<<<<<<< HEAD
    if (strcmp(args[0], "cd") == 0)
    {
        int ret;
        if (args[0] == NULL)
        {
            ret = chdir(getenv("HOME"));
        }
        else
        {
            ret = chdir(args[1]);
        }

        if (ret == -1)
        {
            perror("cd");
            exit(EXIT_FAILURE);
        }
    }
    else if (strcmp(args[0], "printenv") == 0)
    {
        for (int i = 0; environ[i] != NULL; i++)
        {
            printf("%s\n", environ[i]);
        }
    }
    else if (strcmp(args[0], "setenv") == 0)
    {
        if (args[1] == NULL || args[2] == NULL)
        {
            printf("Invalid arguments.\n");
            exit(EXIT_FAILURE);
        }

        char str[STR_MAX];
        snprintf(str, sizeof(str), "%s=%s", args[1], args[2]);
        putenv(str);
    }
    else if (strcmp(args[0], "unsetenv") == 0)
    {
        if (args[1] == NULL)
        {
            printf("Invalid arguments.\n");
            exit(EXIT_FAILURE);
        }

        unsetenv(args[1]);
    }
    else if (strcmp(args[0], "help") == 0)
    {
        printf("Ben's shell (bsh) is a UNIX-like shell.\n");
        printf("Usage:\n");
        printf("cd        change the current working directory\n");
        printf("printenv  print the user environment\n");
        printf("setenv    set an environment variable\n");
        printf("unsetenv  remove an environment variable\n");
        printf("exit      close the shell\n");
    }
    else
    {
        return false;
    }

    return true;
=======
    	int ret;           /* return status */
    	int i;             /* index variable */
    	char str[STR_MAX]; /* input used for setenv */

	/* Check if the command is built-in */
	if (strcmp(args[0], "cd") == 0)
	{
        	/* Change the current working directory */
        	if (args[0] == NULL)
       	 	{
            		ret = chdir(getenv("HOME"));
        	}
        	else
        	{
            		ret = chdir(args[1]);
        	}

        	/* Check for errors */
        	if (ret == -1)
        	{
            		perror("cd");
            		exit(EXIT_FAILURE);
        	}
    	}
    	else if (strcmp(args[0], "printenv") == 0)
    	{
        	/* Print the user environment */
        	for (i = 0; environ[i] != NULL; i++)
        	{
            		printf("%s\n", environ[i]);
        	}
    	}
	else if (strcmp(args[0], "setenv") == 0)
	{
		/* Set an environment variable */
		snprintf(str, sizeof(str), "%s=%s", args[1], args[2]);
		putenv(str);
	}
	else if (strcmp(args[0], "unsetenv") == 0)
	{
		/* Remove an environment variable */
		unsetenv(args[1]);
	}
	else if (strcmp(args[0], "help") == 0)
	{
		/* Display helpful information about the shell */
		printf("Ben's shell (bsh) is a UNIX-like shell.\n");
		printf("Usage:\n");
		printf("cd        change the current working directory\n");
		printf("printenv  print the user environment\n");
		printf("setenv    set an environment variable\n");
		printf("unsetenv  remove an environment variable\n");
		printf("exit      close the shell\n");
	}
	else
	{
	   	return false;
	}

	return true;
>>>>>>> 4e6eb8042d2716aeb14d0db9a543f5c88ba837e1
}

/**
 * Execute a single shell command, handling I/O redirection if necessary.
 *
 * @param args
 *      Buffer containing the command and arguments.
 *
 * @return
 *      Nothing.
 */
void execute(char **args)
{
<<<<<<< HEAD
    for (int i = 0; args[i + 1] != NULL; i++)
    {
        if (i != 0 && strcmp(args[i], "<") == 0)
        {
            redirect(args[i + 1], O_RDONLY, STDIN_FILENO);
            args[i] = NULL;
        }
        else if (i != 0 && strcmp(args[i], ">") == 0)
        {
            redirect(args[i + 1], O_WRONLY | O_APPEND | O_CREAT, STDOUT_FILENO);
            args[i] = NULL;
        }
    }

    if (handle_builtins(args) == true)
    {
        return;
    }

    if (execvp(args[0], args) == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }
=======
	int i; /* index variable */

    	/* Search the arguments for I/O redirect tokens */
    	for (i = 0; args[i + 1] != NULL; i++)
    	{
        	/* Check for input redirection */
        	if ((i != 0) && (strcmp(args[i], "<") == 0))
        	{
            		redirect(args[i + 1], O_RDONLY, STDIN_FILENO);
            		args[i] = NULL;
        	}

        	/* Check for output redirection */
        	if ((i != 0) && (strcmp(args[i], ">") == 0))
        	{
            		redirect(args[i + 1], O_WRONLY | O_APPEND | O_CREAT, STDOUT_FILENO);
            		args[i] = NULL;
        	}
    	}

    	/* Check if the command is built-in */
    	if (handle_builtins(args) == true)
    	{
        	return;
    	}

    	/* Execute the command */
    	if (execvp(args[0], args) == -1)
    	{
		error("fork");
		exit(EXIT_FAILURE);
    	}
>>>>>>> 4e6eb8042d2716aeb14d0db9a543f5c88ba837e1
}

/**
 * Execute a pipeline of shell commands.
 *
 * @param args
 *      Buffer containing the command and arguments.
 *
 * @return
 *      Nothing.
 */
void pipeline(char **args)
{
<<<<<<< HEAD
    if (num_pipes == 0)
    {
        execute(args);
    }

    int pipefd[2];
    if (pipe(pipefd) == -1)
    {
        perror("pipe");
        return;
    }

    pid_t cpid = fork();
    if (cpid == -1)
    {
        perror("fork");
        return;
    }

    if (cpid == 0)
    {
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);

        execute(args);
    }
    else
    {
        close(pipefd[1]);
        dup2(pipefd[0], STDIN_FILENO);
        close(pipefd[0]);
=======
	int pipefd[2]; /* pipe file descriptors */
	pid_t cpid;    /* process ID of the child */
	int status;    /* status info for the terminating process */

	/* If there are no more pipes, execute the command */
	if (num_pipes == 0)
	{
		execute(args);
	}

	/* Create a new pipe */
	if (pipe(pipefd) == -1)
	{
		perror("pipe");
		return;
	}

	/* Fork a child process */
	cpid = fork();
	if (cpid == -1)
	{
		perror("fork");
		return;
	}

	/* Handle parent and child processes */
	if (cpid == 0)
	{
		/* Redirect output to the write end of the pipe */
        	dup2(pipefd[1], STDOUT_FILENO);
		close(pipefd[1]);
		
		/* Execute the command */
		execute(args);
	}
	else
	{
		/* Redirect input to the read end of the pipe */
		close(pipefd[1]);
        	dup2(pipefd[0], STDIN_FILENO);
        	close(pipefd[0]);
>>>>>>> 4e6eb8042d2716aeb14d0db9a543f5c88ba837e1

        waitpid(cpid, NULL, 0);

        num_pipes--;

        while(*(++args) != NULL);
        pipeline(++args);
    }
}

/**
 * Prepare a command for execution.
 *
 * @param args
 *      Buffer containing the command and arguments.
 *
 * @return
 *      Nothing.
 */
void launch(char **args)
{
<<<<<<< HEAD
    pid_t cpid = fork();
    if (cpid == -1)
    {
        perror("fork");
        return;
    }

    if (cpid == 0)
    {
        if (num_pipes > 0)
        {
            pipeline(args);
        }
        else
        {
            execute(args);
        }
    }

    if (backgnd == false)
    {
        waitpid(cpid, NULL, 0);
    }
    else
    {
        printf("Background process started with PID: %d\n", cpid);
    }
=======
	pid_t cpid; /* process ID of the child */
    	int status; /* status info for the terminating process */

    	/* Fork a child process */
    	cpid = fork();
    	if (cpid == -1)
    	{
       		perror("fork");
        	return;
    	}

	/* Handle parent and child processes */
    	if (cpid == 0)
    	{
	    	/* Handle pipes if necessary */
		if (num_pipes != 0)
		{
		    pipeline(args);
		}
		else
		{
		    execute(args);
		}
    	}

    	/* Wait for the child process to terminate */
    	if (backgnd == false)
    	{
       		waitpid(cpid, &status, 0);
    	}
    	else
    	{
        	printf("Background process started with PID: %d\n", cpid);
    	}
>>>>>>> 4e6eb8042d2716aeb14d0db9a543f5c88ba837e1
}

/**
 * Free allocated memory and reset global variables.
 *
 * @param line
 *      Buffer containing the line that was read.
 * @param toks
 *      Buffer containing the command and arguments.
 *
 * @return
 *      Nothing.
 */
void cleanup(char *line, char **toks)
{
<<<<<<< HEAD
    free(line);
    free(toks);

    num_pipes = 0;
    backgnd = false;
=======
	/* Free allocated memory */
	free(line);
	free(toks);

	/* Reset global variables */
	num_pipes = 0;
	backgnd = false;
>>>>>>> 4e6eb8042d2716aeb14d0db9a543f5c88ba837e1
}

/**
 * Shell execution loop.
 *
 * @return
 *      Nothing.
 */
void run_shell(void)
{
<<<<<<< HEAD
    printf("Welcome to Ben's Shell (bsh).\n");
    printf("Type 'help' for more information.\n");
    
    while (true)
    {
        printf("$ ");

        char *line = read_input();
        char **toks = tokenize(line);

        if (strcmp(toks[0], "exit") == 0)
        {
            printf("Goodbye!\n");
            cleanup(line, toks);
            exit(EXIT_SUCCESS);
        }

        launch(toks);

        cleanup(line, toks);
    }
=======
	char *line;  /* line input buffer */
   	char **toks; /* token buffer */

    	while (true)
    	{
        	/* Display the shell prompt */
        	printf("$ ");

        	/* Read a command */
        	line = read_input();

        	/* Parse the command into tokens */
        	toks = tokenize(line);

        	/* Handle the exit command */
        	if (strcmp(toks[0], "exit") == 0)
        	{
           		printf("Goodbye!\n");
            		cleanup(line, toks);
            		exit(EXIT_SUCCESS);
        	}

        	/* Launch the command */
		launch(toks);

        	/* Handle cleanup */
        	cleanup(line, toks);
    	}
>>>>>>> 4e6eb8042d2716aeb14d0db9a543f5c88ba837e1
}

/**
 * Signal handler for SIGINT.
 *
 * @param signo
 *      Signal number, unused.
 *
 * @return
 *      Nothing.
 */
void sigint_handler(int signo)
{
	printf("Interrupted!\n");
}

/**
 * Signal handler for SIGCHLD.
 *
 * @param signo
 *      Signal number, unused.
 *
 * @return
 *      Nothing.
 */
void sigchld_handler(int signo)
{
	while(waitpid(-1, NULL, WNOHANG) > 0);
}

/**
 * Initialize the shell environment.
 *
 * @return
 *      Nothing.
 */
void shell_init(void)
{
<<<<<<< HEAD
    struct sigaction sa_sigint;
    sa_sigint.sa_handler = sigint_handler;
    sigemptyset(&sa_sigint.sa_mask);
    sa_sigint.sa_flags = SA_RESTART;

    if (sigaction(SIGINT, &sa_sigint, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    struct sigaction sa_sigchld;
    sa_sigchld.sa_handler = sigchld_handler;
    sigemptyset(&sa_sigchld.sa_mask);
    sa_sigchld.sa_flags = SA_RESTART;

    if (sigaction(SIGCHLD, &sa_sigchld, NULL) == -1)
    {
        perror("sigaction");
        exit(EXIT_FAILURE);
    }

    num_pipes = 0;
    backgnd = false;
=======
	struct sigaction sa_sigint;  /* Sigaction structure for SIGINT */
    	struct sigaction sa_sigchld; /* Sigaction structure for SIGCHLD */

    	/* Set up the SIGINT signal handler */
    	sa_sigint.sa_handler = sigint_handler;
    	sigemptyset(&sa_sigint.sa_mask);
    	sa_sigint.sa_flags = SA_RESTART;

    	if (sigaction(SIGINT, &sa_sigint, NULL) == -1)
    	{
        	perror("sigaction");
        	exit(EXIT_FAILURE);
    	}

    	/* Set up the SIGCHLD signal handler */
    	sa_sigchld.sa_handler = sigchld_handler;
    	sigemptyset(&sa_sigchld.sa_mask);
    	sa_sigchld.sa_flags = SA_RESTART;

    	if (sigaction(SIGCHLD, &sa_sigchld, NULL) == -1)
    	{
        	perror("sigaction");
        	exit(EXIT_FAILURE);
    	}

	/* Initialize global variables */
	num_pipes = 0;
	backgnd = false;

    	/* Display the welcome message */
    	printf("Welcome to Ben's Shell (bsh).\n");
    	printf("Enter 'help' for more information.\n");
>>>>>>> 4e6eb8042d2716aeb14d0db9a543f5c88ba837e1
}

/**
 * Main function.
 *
 * @return
 *      Should not return.
 */
int main(void)
{
	shell_init();
    	run_shell();
    	return 0;
}
