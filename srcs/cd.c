//header

#include "minishell.h"

static t_error	exec_cd(t_cd_opts *options)
{
	t_error	err;

	err = OK;
	//printf("cdpath env %s\n", cdpath_env);
	//printf("path %s\n", options->path);
	options->finalpath = ft_strdup(options->path);
	//printf("joined %s\n", path_join(options->pwd, options->path));
	if (err == OK && !options->finalpath)
		return (ERR_MALLOC);
	if (err == OK)
		err = path_chdir(options->finalpath, options->pwd);
	if (err == OK)
		free(options->finalpath);
	return (err);
}

t_error	builtin_cd(size_t argc, char **argv)
{
	t_cd_opts	options;
	t_error		err;

	err = OK;
	ft_bzero(&options, sizeof(options));
	if (argc > 1)
		return (ERR_TOOMUCH_ARGS);
	options.home = env_get_value("HOME");
	//printf("home %s\n", options.home);
	if (argc)
		options.path = *argv;
	else
		options.path = options.home;
	if (!options.path)
		return (OK);
	options.relative = (*options.path != '/');
	//options.back = ft_strcmp(options.path, "-") == 0;
	//printf("rela %d back %d\n", options.relative, options.back);
	err = path_pwd(&(options.pwd));
	//printf("path %s pwd %s\n", options.path, options.pwd);
	//printf("back %s\n", options.finalpath);
	if (err == OK && options.path)
		err = exec_cd(&options);
	free(options.pwd);
	return (err);
}
