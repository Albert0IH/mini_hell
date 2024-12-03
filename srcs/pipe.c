/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamuyel <ahamuyel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/02 14:58:33 by ahamuyel          #+#    #+#             */
/*   Updated: 2024/12/02 14:58:47 by ahamuyel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

// Função para dividir comandos por '|'
void split_pipes(char *input, char **commands)
{
    int i = 0;
    char *token = my_strtok(input, "|");

    while (token)
    {
        commands[i] = token;
        token = my_strtok(NULL, "|");
        i++;
    }
    commands[i] = NULL;
}

// Executa um único comando
void execute_single_command(char *cmd)
{
    char *args[100];
    tokenize_inputs(cmd, args);

    char *cmd_path = get_command_path(args[0]);
    if (cmd_path == NULL)
    {
        fprintf(stderr, "Command not found: %s\n", args[0]);
        exit(EXIT_FAILURE);
    }
    if (execve(cmd_path, args, NULL) == -1)
    {
        perror("execve");
        exit(EXIT_FAILURE);
    }
}

// Executa dois comandos conectados por um pipe
void execute_pipeline(char *cmd1, char *cmd2)
{
    int fd[2];
    pid_t pid1, pid2;

    if (pipe(fd) == -1) // Criação do pipe
    {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid1 = fork();
    if (pid1 == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid1 == 0) // Processo filho 1 (executa cmd1 e escreve no pipe)
    {
        close(fd[0]);           // Fecha a leitura do pipe
        dup2(fd[1], STDOUT_FILENO); // Redireciona a saída para o pipe
        close(fd[1]);

        execute_single_command(cmd1);
    }

    pid2 = fork();
    if (pid2 == -1)
    {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid2 == 0) // Processo filho 2 (lê do pipe e executa cmd2)
    {
        close(fd[1]);           // Fecha a escrita no pipe
        dup2(fd[0], STDIN_FILENO); // Redireciona a entrada para o pipe
        close(fd[0]);

        execute_single_command(cmd2);
    }

    // Processo pai fecha os descritores do pipe e espera os filhos
    close(fd[0]);
    close(fd[1]);
    waitpid(pid1, NULL, 0);
    waitpid(pid2, NULL, 0);
}

int main(void)
{
    char *input;
    char *commands[10]; // Para até 10 comandos separados por pipe

    while (1)
    {
        input = readline("minishell> ");
        if (!input || ft_strncmp(input, "exit", 5) == 0)
        {
            free(input);
            break ;
        }
        if (*input)
            add_history(input);

        // Dividir os comandos por pipe
        split_pipes(input, commands);

        if (commands[1] == NULL) // Nenhum pipe, comando único
        {
            if (fork() == 0)
                execute_single_command(commands[0]);
            wait(NULL);
        }
        else // Pipeline com dois comandos
        {
            execute_pipeline(commands[0], commands[1]);
        }

        free(input);
    }
    rl_clear_history();
    return (0);
}
