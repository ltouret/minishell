//header

#include "minishell.h"

t_error	builtin_echo(size_t argc, char **argv)
{
	bool nonewline;

	while (argc)
	{
		if (!ft_strcmp(*argv, "-n"))
			nonewline = true;
		else
			break ;
		argv++;
		argc--;
	}
	while (argc)
	{
		ft_putstr_fd(*argv, 1);
		if (argc > 1)
			ft_putstr_fd(" ", 1);
		argv++;
		argc--;
	}
	if (!nonewline)
		ft_putstr_fd("\n", 1);
	return (OK);
}
