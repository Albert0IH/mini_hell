/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ahamuyel <ahamuyel@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/19 10:50:50 by ahamuyel          #+#    #+#             */
/*   Updated: 2024/12/11 17:08:48 by ahamuyel         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# define BUFFER_SIZE 42

# include "../libft/libft.h"
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/wait.h>
# include <unistd.h>
#include <fcntl.h>       // Para funções e constantes relacionadas a arquivos


// Parsing
void	tokenize_inputs(char *input, char **args);
char	*my_strtok(char *str, const char *delim);
// Execution
char	*get_command_path(char *cmd);
void	execute_command(char *cmd);
void	free_args(char **array);
// Pipes
void	split_pipes(char *input, char **commands);
void execute_pipeline(char **commands);
#endif
