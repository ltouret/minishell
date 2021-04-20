//header

#include "minishell.h"

static t_error	builtin_export_printall(void)
{
	t_env			*entry;

	entry = *(env_dictionary());
	while (entry)
	{
		if (!env_isinternal(entry->key))
			if (printf("declare -x %s=\"%s\"\n",
					entry->key, entry->value) < 0)
				return (ERR_PRINTF);
		entry = entry->next;
	}
	return (OK);
}

t_error	builtin_export(size_t argc, char **argv)
{
	t_env_equality	eqlt;

	if (!argc)
		return (builtin_export_printall());
	while (argc)
	{
		eqlt = env_isvalid_equality(*argv, true);
		if (eqlt == ENV_EQ_MISFORMAT)
			return (ERR_SYNTAX);
		else if (eqlt == ENV_EQ_UNIFIED)
			env_create(*argv);
		++argv;
		--argc;
	}
	return (OK);
}
