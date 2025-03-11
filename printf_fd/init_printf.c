#include "ft_dprintf.h"

void	init_printf(t_printf *data)
{
	data->cutting = NULL;
	data->error = 0;
	data->final_line = NULL;
	data->line_len = -1;
}
