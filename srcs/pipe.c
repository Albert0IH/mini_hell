#include "../includes/minishell.h"

void split_pipes(char *input, char **cmd)
{
	int		i;
	char	*token;

	i = 0;
	token = my_strtok(input, "|");
	while (token)
	{
		cmd[i] = token;
		token = my_strtok(NULL, "|");
		i++;
	}
	cmd[i] = NULL;
}

void execute_single_command(char *cmd)
{
	char	*args[100];
	char	*cmd_path;

	tokenize_inputs(cmd, args);
	cmd_path = get_command_path(args[0]);
	if (cmd_path == NULL)
	{
		ft_putstr_fd("Command not found: ", 2);
        ft_putstr_fd(args[0], 2);
        ft_putstr_fd("\n", 2);
        exit(EXIT_FAILURE);
	}
	if (execve(cmd_path, args, NULL) == -1)
	{
		perror("execve");
		exit(EXIT_FAILURE);
	}
}

void execute_pipeline(char **commands)
{
	int		i = 0, fd[2], in_fd = 0;
	pid_t	pid;

	while (commands[i])
	{
		// Criar um pipe para cada par de comandos
		if (commands[i + 1] && pipe(fd) == -1)
		{
			perror("pipe");
			exit(EXIT_FAILURE);
		}
		pid = fork();
		if (pid == -1)
		{
			perror("fork");
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
		{
			// No processo filho
			if (in_fd != 0) // Conectar entrada ao pipe anterior
			{
				dup2(in_fd, STDIN_FILENO);
				close(in_fd);
			}
			if (commands[i + 1]) // Se não for o último comando, conectar saída ao próximo pipe
			{
				dup2(fd[1], STDOUT_FILENO);
				close(fd[1]);
			}
			execute_single_command(commands[i]);
		}
		else
		{
			// No processo pai
			wait(NULL);
			if (in_fd != 0) // Fechar pipe anterior
				close(in_fd);
			if (commands[i + 1]) // Atualizar in_fd para o próximo comando
				in_fd = fd[0];
			close(fd[1]); // Fechar a saída do pipe atual
		}
		i++;
	}
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
		else // Pipeline com múltiplos comandos
		{
			execute_pipeline(commands);
		}

		free(input);
	}
	rl_clear_history();
	return (0);
}
