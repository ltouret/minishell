/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environ_utils1.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltouret <ltouret@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/04/28 19:49:25 by ltouret           #+#    #+#             */
/*   Updated: 2021/04/28 21:37:39 by ltouret          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

t_env_equality	env_isvalid_equality(const char *str, bool sanitizename)
{
	char	*cur;

	if (!str)
		return (ENV_EQ_NONE);
	if (sanitizename)
	{
		if (!env_verify_name(str))
			return (ENV_EQ_MISFORMAT);
	}
	else if (!*str || *str == '=')
		return (ENV_EQ_MISFORMAT);
	cur = (char*)ft_strchr(str, '=');
	if (cur == NULL)
		return (ENV_EQ_NONE);
	return (ENV_EQ_UNIFIED);
}

char			*env_parse_key(const char *env_char)
{
	char	*delimiter;

	delimiter = ft_strchr(env_char, '=');
	return (ft_substr(env_char, 0, delimiter - env_char));
}

char			*env_parse_value(const char *env_char)
{
	char	*delimiter;

	delimiter = ft_strchr(env_char, '=');
	return (ft_strdup(delimiter + 1));
}

char			*env_toequality(t_env *env)
{
	size_t	keyl;
	size_t	vall;
	char	*env_char;

	keyl = ft_strlen(env->key);
	vall = ft_strlen(env->value);
	env_char = malloc(sizeof(char) * (keyl + vall + 2));
	if (!env_char)
		panic(ERR_MALLOC);
	ft_memcpy(env_char, env->key, keyl);
	*(env_char + keyl) = '=';
	ft_memcpy(env_char + keyl + 1, env->value, vall + 1);
	return (env_char);
}
