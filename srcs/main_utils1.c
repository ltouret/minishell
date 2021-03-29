//header

#include "minishell.h"
#include "libft.h"
#include <stdlib.h>

t_data	*get_data(void)
{
	static t_data	data;

	return (&data);
}

int	*should_prompt_be_printed(void)
{
	static int	i = 0;

	return (&i);
}
