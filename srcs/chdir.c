//header

#include "minishell.h"

//add this to minishell.h

t_error	path_chdir(const char *newpath, const char *oldpath)
{
	t_error	err;

	err = chdir(newpath);
	if (err == OK)
	{
		env_set("OLDPWD", oldpath);
		env_set("PWD", newpath);
	}
	else
		err = ERR_ERRNO;
	return (err);
}
