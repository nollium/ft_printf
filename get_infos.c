/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_infos.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: smaccary <smaccary@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/31 05:10:23 by smaccary          #+#    #+#             */
/*   Updated: 2020/02/12 10:34:25 by smaccary         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

static int		get_width(char *format, va_list *list)
{
	return ((*format == '*') ? va_arg(*list, int) : ft_atoi(format));
}

static int		get_precision(char *format, va_list *list)
{
	return ((format = ft_strchr(format, '.')) ? get_width(format + 1, list) : 0);
}

t_infos			check_infos(t_infos infos)
{
	if (infos.width < 0)
	{
		infos.width *= -1;
		infos.pos = 'l';
	}
	if (infos.precision < 0)
	{
		infos.precision = 0;
		infos.dot = 0;
	}
	infos.space = ((infos.space == '0' && infos.precision) || infos.pos	== 'l'
		   	|| !ft_strchr(NUMERIC_TYPES, infos.conv)) ? ' ' : infos.space;
	if (infos.conv == 's')
	{
		infos.trunc = infos.precision;
		infos.precision = 0;
	}
	return (infos);
}

t_infos			swp_printer(t_infos infos, ssize_t (*my_write)(int, const void *, size_t))
{
	infos.printer = (*my_write);
	return (infos);
}

t_infos			get_infos(char *format, va_list *list, ssize_t (*my_write)(int, const void *, size_t))
{
	t_infos	infos;

	if (*format)
		++format;
	if (*format == '-')
	{
		infos.pos = 'l';
		format++;
	}
	else
		infos.pos = 'r';
	infos.space = 0;
	while (*format == '0')
	{
		infos.space = (infos.pos == 'l') ? ' ' : '0';
		format++;
	}
	infos.space = (infos.space) ? infos.space : ' ';
	infos.width = get_width(format, list);
	infos.precision = get_precision(format, list);
	infos.conv = *get_conv(format);
	infos.printer = (*my_write);
	infos.dot = ft_strchr(format, '.');
	infos.trunc = infos.precision;
	if (ft_strchr("di", infos.conv) && infos.space == '0' && !infos.dot && get_curr_int(list) < 0)
		infos.width = (infos.width - 1 >= 0)
		 ? infos.width - 1 : infos.width;
	infos = check_infos(infos);
	return (infos);
}
