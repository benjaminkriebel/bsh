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
    char *buf = NULL;
    size_t len = 0;

    if (getline(&buf, &len, stdin) == -1)
    {
        return NULL;
    }

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
    free(line);
    free(toks);

    num_pipes = 0;
    backgnd = false;
}

/**
 * Shell execution loop.
 *
 * @return
 *      Nothing.
 */
void run_shell(void)
{
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
