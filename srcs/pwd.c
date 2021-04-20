//header

#include "minishell.h"

t_error	builtin_pwd(size_t argc, char **argv)
{
	char	*cwd;

	(void)argv;
	if (argc > 0)
		return (ERR_TOOMUCH_ARGS);
	cwd = getcwd(NULL, 0);
	if (!cwd)
		return (ERR_MALLOC);
	if (printf("%s\n", cwd) < 0)
		return (ERR_PRINTF);
	free(cwd);
	return (OK);
}
