
#include "../includes/minishell.h"

void execute_command(char **input)
{
    pid_t pid;
    int status;

    if (!input[0])
        return ;
    pid = fork();
    if (pid == 0)
    {
        if (execve(input[0], input, NULL) == -1)
        {
            perror("minishell");
            exit(EXIT_FAILURE);
        }
    }
    else if (pid < 0)
        perror("minishell");
    else
        waitpid(pid, &status, 0);
}