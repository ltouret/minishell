//header

#include "minishell.h"

t_env	*env_create(char *env_str)
{
	char	*key;
	char	*value;
	t_env	*env;

	key = env_parse_key(env_str);
	if (!key)
		panic(ERR_MALLOC);
	value = env_parse_value(env_str);
	if (!value)
		panic(ERR_MALLOC);
	env = (env_set(key, value));
	free(key);
	free(value);
	return (env);
}

static t_env	*env_set_append(const char *key, const char *value)
{
	t_env	**env_start;
	t_env	*current;

	env_start = env_dictionary();
	if (!(*env_start))
		panic(ERR_UNINIT_ENV_DICO);
	while (*env_start)
		env_start = &((*env_start)->next);
	current = malloc(sizeof(t_env));
	if (!current)
		panic(ERR_MALLOC);
	current->key = ft_strdup(key);
	if (!current->key)
		panic(ERR_MALLOC);
	current->value = ft_strdup(value);
	if (!current->value)
		panic(ERR_MALLOC);
	current->next = NULL;
	*env_start = current;
	return (current);
}

t_env	*env_set(const char *key, const char *value)
{
	t_env	*current;

	current = env_get(key);
	if (current)
	{
		free(current->value);
		current->value = ft_strdup(value);
		if (!current->value)
			panic(ERR_MALLOC);
		return (current);
	}
	else
		return (env_set_append(key, value));
}

void	env_unset(const char *key)
{
	t_env	**env_start;
	t_env	*current;

	env_start = env_dictionary();
	current = *env_start;
	if (!current)
		panic(ERR_UNINIT_ENV_DICO);
	while (current && ft_strcmp(current->key, key) != 0)
	{
		env_start = &(current->next);
		current = current->next;
	}
	if (current)
	{
		*env_start = current->next;
		free(current->key);
		free(current->value);
		free(current);
	}
}
