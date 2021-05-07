#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include "libft/libft.h"

typedef struct	s_history {
	char		**history; //try to dos this with t_string
	t_string	*path;
	int			hist_fd;
	int			size;
}				t_history;

int		add_line(char ***hist, char *line)
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

void	read_hist(t_history *history)
{
	char	*line;
	int		gnl;

	gnl = 1;
	while (gnl == 1)
	{
		gnl = get_next_line(history->hist_fd, &line);
		if (gnl == 1)
		{
			add_line(&history->history, line);
			history->size++;
		}
	}
}

void	print_hist(t_history *history)
{
	int		i;

	i = -1;
	while (history->size > ++i)
		printf("%d %s\n", i + 1, history->history[i]);
}

int		open_hist(t_history *history)
{
	history->path = ft_string_new();
	if (!(ft_string_push(history->path,  getenv("HOME"))))
		return (0); // err malloc
	if (!(ft_string_push(history->path,  "/.bash_history")))
		return (0); // err malloc
	history->hist_fd = open(history->path->str, O_RDONLY);
	if (history->hist_fd == -1)
		return (1); // err no file found - set errno!!
	read_hist(history);
	close(history->hist_fd);
}

int		main()
{
	t_history	history;

	ft_bzero(&history, sizeof(t_history));
	open_hist(&history);
	print_hist(&history);
}
