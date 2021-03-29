//header

#include "minishell.h"

void	panic(t_error err_code)
{
	if (err_code == ERR_MALLOC)
		printf("Error : out of memory ... exiting\n");
	else if (err_code == ERR_READ)
		printf("Error : cannot read stdin ... exiting\n");
	else if (err_code == ERR_UNIMPLENTED)
		printf("Error : unimplented function ... exiting\n");
	else if (err_code == ERR_PRINTF)
		printf("Error : printf returned negative value ... exiting\n");
	else if (err_code == ERR_UNINIT_ENV_DICO)
		printf("Error : environ dico must be initialized ... exiting\n");
	else if (err_code == ERR_DOUBLE_INIT_ENV_DICO)
		printf("Error : environ dico is already initialized ... exiting\n");
	else
		printf("Unknown Error ... exiting\n");
	exit(ERR);
}
