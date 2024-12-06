/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamuyel <ahamuyel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/25 11:11:09 by ahamuyel          #+#    #+#             */
/*   Updated: 2024/12/03 09:57:35 by ahamuyel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	main(void)
{

	char	*input;
	char	**tokens;

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
		tokens = malloc(sizeof(char *) * 100);
		tokenize_inputs(input, tokens);
		execute_command(tokens);
		free(tokens);
		free(input);
	}
	rl_clear_history();
	return (0);
}
