
#include "../includes/minishell.h"

void	free_args(char **array)
{
	int	i;

	i = 0;
	if (array != NULL)
	{
		while (array[i] != NULL)
		{
			free(array[i]);
			i++;
		}
		free(array);
	}
}

char	*get_command_path(char *cmd)
{
	char	*dir;
	char	*full_path;
	char	*tmp;
	char	**directories;
	int		i;
	char	*path;

	i = 0;
	path = getenv("PATH");
	if (path == NULL)
		return (NULL);
	if (cmd[0] == '/')
		return (cmd);
	directories = ft_split(path, ':');
	while (directories[i])
	{
		dir = directories[i];
		tmp = ft_strjoin(dir, "/");
		full_path = ft_strjoin(tmp, cmd);
		free(tmp);
		if (access(full_path, F_OK) == 0)
		{
			free_args(directories);
			return (full_path);
		}
		free(full_path);
		i++;
	}
	free_args(directories);
	return (NULL);
}

int	execute_command(char **args)
{
	pid_t pid;
	int status;
	char *cmd_path;

	if (!args[0])
		return (-1);
	pid = fork();
	if (pid == 0)
	{
		cmd_path = get_command_path(args[0]);
		if (cmd_path == NULL)
		{
			fprintf(stderr, "Comand not found: %s\n", args[0]);
			exit(EXIT_FAILURE);
		}
		if (execve(cmd_path, args, NULL) == -1)
		{
			perror("exece");
			exit(EXIT_FAILURE);
		}
	}
	else
		waitpid(pid, &status, 0);
	return (0);
}