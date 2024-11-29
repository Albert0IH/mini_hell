#include "../includes/minishell.h"

char	*my_strtok(char *str, const char *delim)
{
	char		*start;
	static char	*current = NULL;

	if (str != NULL)
		current = str;
	if (current == NULL)
		return (NULL);
	while (*current && ft_strchr(delim, *current))
		current++;
	if (*current == '\0')
		return (NULL);
	start = current;
	while (*current && !ft_strchr(delim, *current))
		current++;
	if (*current)
	{
		*current = '\0';
		current++;
	}
	return (start);
}

void	tokenize_inputs(char *input, char **args)
{
	int		i;
	char	*token;

	input[ft_strcspn(input, "\n")] = '\0';
	token = my_strtok(input, " ;");
	i = 0;
	while (token != NULL)
	{
		args[i] = token;
		token = my_strtok(NULL, " ;");
		i++;
	}
	args[i] = NULL;
}
