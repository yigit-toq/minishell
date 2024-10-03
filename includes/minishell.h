/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abakirca <abakirca@student.42kocaeli.co    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/26 17:08:52 by ytop              #+#    #+#             */
/*   Updated: 2024/10/03 15:38:16 by abakirca         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "libft.h"
# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>

# define FAILURE	1
# define SUCCESS	0

# define STD_OUTPUT 1
# define STD_INPUT  0

# define STD_ERROR	2

# define TRUE		1
# define FALSE		0

# define SEMICOLON	';'
# define REDIRECT	'>'
# define INPUT		'<'
# define PIPE		'|'

# define S_QUOTE	'\''
# define D_QUOTE	'\"'

# define QUESTION	'?'
# define DOLLAR		'$'
# define SPACE		' '

# define BLACK		"\033[1;90m"
# define RED		"\033[1;91m"

# define GREEN		"\033[1;92m"

# define RESET		"\033[0m"

# define SYNTAX_ERR	"syntax error near unexpected token"
# define ERR_TITLE	"minishell: "
# define PROMPT		"minishell> "

extern int	g_signal;

typedef enum e_token
{
	SEMICOLON_T = SEMICOLON,
	REDIRECT_T = REDIRECT,
	INPUT_T = INPUT,
	PIPE_T = PIPE,
}			t_token;

typedef struct s_fd
{
	int		change;
	int		std_in;
	int		std_out;
}			t_fd;

typedef struct s_value
{
	char	*old_pwd;
	int		*pipe_fd;
	int		*hrdc_fd;
	int		pipe_count;
	int		hrdc_count;
	int		exit_code;
	int		status;
	int		sign;
}			t_value;

typedef struct s_minishell
{
	t_list		*hrdc_cmd;
	t_list		*token;
	t_list		*env;
	int			*pid;
	char		*path;
	char		*line;
	t_fd		fd_hl;
	t_value		value;
	t_parser	*parser;
}				t_minishell;

t_minishell	*get_minishell(void);

int			type_control(t_parser *parser, char **envs);

// Pipe

int			ft_pipe(char **cmd, t_parser *parser);

// Replace arg

int			replace_arg(char **args);

// Check line

int			check_quote(char *line, int value);

int			check_line(void);

// Compiler

int			parser(t_minishell *minishell);

void		lexer(t_minishell *minishell);

t_parser	*allocate_parser(int count);

// Executor

void		check_pid(t_parser *parser, pid_t *pid);

int			check_builtin(t_minishell *minishell, char **cmd, t_parser *parser, int *i);

int			execute_command(void);

void		ft_all_lower(char **str);

void		remove_quotes(t_parser *parser);

void		init_cmd(t_parser *parser, char **cmd);

char		*handle_quotes(const char *str);

char		*find_path(char *cmd);

// Builtins

char		**env(void);

int			print_env(void);

int			export(t_minishell *minishell, char **args);

char		*get_pwd(void);

t_list		*search_env(t_minishell *minishell, char *key);

void		env_to_list(char *env[]);

void		get_env(t_minishell *minishell, char **result, char **str, int *i);

int			get_key(char *line);

char		*get_value(char *line);

int			ft_exit(t_minishell *shell, char **av);

int			cd(t_minishell *minishell, char *av);

int			echo(char **args);

int			unset(t_minishell *shell, char **keys);

void		change_oldpwd(t_list *old_pwd_env, t_list *pwd_env, char *val);

void		change_pwd(t_minishell *minishell, char *pwd);

// Dollar

void		dollar(t_minishell *shell);

void		get_ext_code(t_minishell *shell, char **result, int *i);

// Utils

void		save_fd(void);
void		reset_fd(void);

void		handle_signals(void);
void		disable_echo(void);

char		*strjoin_char(char *str, char c);
char		**parser_split(char *str, char delimiter);

int			err_msg(char *cmd, char *arg, char *msg);

// Heredoc

int			heredoc(void);

int			delimiter(t_minishell *shell);

int			heredoc_syntax(char **args);

// Redirect

int			check_redirect(char **args);
#endif
