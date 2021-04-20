//header

#include "minishell.h"

t_error	builtin_unset(size_t argc, char **argv)
{
	while (argc)
	{
		env_unset(*argv);
		argv++;
		argc--;
	}
	return (OK);
}
