//header

#include "minishell.h"

t_error	builtin_env(size_t argc, char **argv)
{
	t_env	*entry;

	(void)argv;
	if (argc > 0)
		return (ERR_TOOMUCH_ARGS);
	entry = *(env_dictionary());
	while (entry)
	{
		if (!env_isinternal(entry->key))
			if (printf("%s=%s\n", entry->key, entry->value) < 0)
				return (ERR_PRINTF);
		entry = entry->next;
	}
	return (OK);
}
