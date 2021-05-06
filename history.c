#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "libft/libft.h"

int		get_map(char ***hist, char *line)
{
	int		i;
	char	**tmp;

	if (!(*hist))
	{
		if (!(*hist = malloc(sizeof(char *) * 1)))
			return (0); // err mall
		(*hist)[0] = NULL;
	}
	i = 0;
	while ((*hist)[i])
		i++;
	if (!(tmp = malloc(sizeof(char *) * (i + 2))))
		return (0); // err mall
	i = -1;
	while ((*hist)[++i])
		tmp[i] = (*hist)[i];
	if (!(tmp[i] = ft_strdup(line)))
		return (0);
	tmp[i + 1] = NULL;
	free(*hist);
	*hist = tmp;
	return (1); //check this and return type
}

int		open_hist()
{
	char		*home;
	t_string	*hist_str;
	int			hist_fd;
	char		**hist;
	hist = NULL;

	get_map(&hist, "sapo");
	printf("%s\n", hist[0]);
	get_map(&hist, "qpasa");
	printf("%s\n", hist[1]);
	if (!(home = getenv("HOME")))
		return (0); // err malloc
	if (!(hist_str = ft_string_new()))
		return (0); // err malloc
	if (!(ft_string_push(hist_str,  getenv("HOME"))))
		return (0); // err malloc
	if (!(ft_string_push(hist_str,  "/.bash_history")))
		return (0); // err malloc
	//hist_fd = open(hist_str,);
	printf("%s\n", hist_str->str);
}

int		main()
{
	open_hist();
}
