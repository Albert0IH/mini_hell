#include "../includes/minishell.h"

// void	split_pipes(char *input, char **cmd)
// {
// 	int		i;
// 	char	*token;

// 	i = 0;
// 	token = my_strtok(input, "|");
// 	while (token)
// 	{
// 		cmd[i] = token;
// 		token = my_strtok(NULL, "|");
// 		i++;
// 	}
// 	cmd[i] = NULL;
// }

void split_pipes(char *input, char **cmd)
{
	int i;
	char *token;

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

int	handle_redirections(char **args)
{
	int	i;
	int fd;

	i = 0;
	while (args[i])
	{
		if (ft_strcmp(args[i], ">") == 0 || ft_strcmp(args[i], ">>") == 0)
		{
			fd = open(args[i + 1], (ft_strcmp(args[i],
							">") == 0) ? (O_CREAT | O_WRONLY | O_TRUNC) : (O_CREAT | O_WRONLY | O_APPEND),
					0644);
			if (fd < 0)
				return (perror("open"), -1);
			dup2(fd, STDOUT_FILENO);
			close(fd);
			args[i] = NULL; // Remove o redirecionamento da lista de argumentos
		}
		else if (ft_strcmp(args[i], "<") == 0)
		{
			fd = open(args[i + 1], O_RDONLY);
			if (fd < 0)
				return (perror("open"), -1);
			dup2(fd, STDIN_FILENO);
			close(fd);
			args[i] = NULL; // Remove o redirecionamento da lista de argumentos
		}
		i++;
	}
	return (0);
}

void	execute_command(char *cmd)
{
	char	*args[100];
	char	*cmd_path;

	tokenize_inputs(cmd, args);
	if (handle_redirections(args) < 0)
		exit(EXIT_FAILURE); // Falha ao configurar redirecionamentos
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

void	execute_pipeline(char **commands)
{
	int		i = 0, fd[2], in_fd = 0;
	pid_t	pid;

	while (commands[i])
	{
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
			if (in_fd != 0) // Conectar entrada ao pipe anterior
			{
				dup2(in_fd, STDIN_FILENO);
				close(in_fd);
			}
			if (commands[i + 1]) // Conectar saída ao próximo pipe
			{
				dup2(fd[1], STDOUT_FILENO);
				close(fd[1]);
			}
			execute_command(commands[i]);
		}
		else
		{
			wait(NULL);
			if (in_fd != 0)
				close(in_fd);
			if (commands[i + 1])
				in_fd = fd[0];
			close(fd[1]);
		}
		i++;
	}
}

int	main(void)
{
	char	*input;

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
		split_pipes(input, commands);
		if (commands[1] == NULL) // Comando único
		{
			if (fork() == 0)
				execute_command(commands[0]);
			wait(NULL);
		}
		else // Pipeline
		{
			execute_pipeline(commands);
		}
		free(input);
	}
	rl_clear_history();
	return (0);
}
