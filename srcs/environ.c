//header

#include "minishell.h"

t_env	**env_dictionary(void)
{
	static t_env	*entries;

	return (&entries);
}

static void	env_init_fill(char **environ, t_env **env_start)
{
	t_env	*current;

	while (*environ)
	{
		if (env_isvalid_equality(*environ, false))
		{
			current = malloc(sizeof(t_env));
			if (!current)
				panic(ERR_MALLOC);
			*env_start= current;
			current->key = env_parse_key(*environ);
			if (!current->key)
				panic(ERR_MALLOC);
			current->value = env_parse_value(*environ);
			if (!current->value)
				panic(ERR_MALLOC);
			env_start = &(current->next);
		}
		environ++;
	}
	current->next = NULL;
}

void	env_init(char **environ)
{
	t_env	**env_start;

	env_start = env_dictionary();
	if (*env_start)
		panic(ERR_DOUBLE_INIT_ENV_DICO);
	env_init_fill(environ, env_start);
	env_set("?", "0");
}

void	env_shutdown(void)
{
	t_env	*current;
	t_env	*next;

	current = *env_dictionary();
	while (current)
	{
		next = current->next;
		free(current->key);
		free(current->value);
		free(current);
		current = next;
	}
}
