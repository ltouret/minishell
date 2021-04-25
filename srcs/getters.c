//header

#include "minishell.h"

t_env	*env_get(const char *key)
{
	t_env	*current;

	current = *(env_dictionary());
	if (!current)
		panic(ERR_UNINIT_ENV_DICO);
	while (current && ft_strcmp(current->key, key) != 0)
		current = current->next;
	return (current);
}

char	*env_get_value(const char *key)
{
	t_env	*current;

	current = env_get(key);
	if (!current)
		return (NULL);
	return (current->value);
}

char	**env_get_array(void)
{
	size_t	listlen;
	t_env	*current;
	char	**envs;
	char	**envs_cpy;

	current = *(env_dictionary());
	listlen = env_listlen(current);
	envs = malloc(sizeof(char*) * (listlen + 1));
	if (!envs)
		panic(ERR_MALLOC);
	envs_cpy = envs;
	while (current)
	{
		if (!env_isinternal(current->key))
			*(envs_cpy++) = env_toequality(current);
		current = current->next;
	}
	*envs_cpy = NULL;
	return (envs);
}

void	env_clear_array(char **envs)
{
	char	**envs_cpy;

	envs_cpy = envs;
	while (*envs)
		free(*(envs++));
	free(envs_cpy);
}
