/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handle_quotes.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: alize <alize@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/17 12:27:45 by caburges          #+#    #+#             */
/*   Updated: 2025/02/19 16:18:13 by alize            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int	is_quote(char c)
{
	return (c == '\'' || c == '\"');
}

int	unclosed_quote_detected(char *input)
{
	while (*input != '\0')
	{
		if (is_quote(*input))
		{
			input = ft_strchr(input + 1, *input);
			if (input == NULL)
				return (TRUE);
		}
		input++;
	}
	return (FALSE);
}
