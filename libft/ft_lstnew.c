/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstnew.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ltouret <ltouret@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/10/22 14:32:30 by ltouret           #+#    #+#             */
/*   Updated: 2021/04/28 19:23:35 by ltouret          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

t_list	*ft_lstnew(const char *name, void *content)
{
	t_list *ret;

	if ((ret = (t_list*)(malloc(sizeof(t_list)))))
	{
		ret->name = ft_strdup(name);
		ret->content = content;
		ret->next = NULL;
	}
	return (ret);
}
