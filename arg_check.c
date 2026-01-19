/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   arg_check.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nayara <nayara@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/09 18:04:34 by nayara            #+#    #+#             */
/*   Updated: 2026/01/06 19:02:41 by nayara           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3D.h"

static int	check_ext(const char *str)
{
	char	*ext;

	ext = ".cub";
	if (ft_strncmp(str + ft_strlen(str) - 4, ext, 4) != 0)
		return (0);
	return (1);
}

int	arg_check(int ac, char **av)
{
	if (ac < 2)
		return (ft_printf("Wrong number of arguments"), 0);
	if (ac > 2)
		return (ft_printf("Wrong number of arguments"), 0);
	if (!check_ext(av[1]))
		return (ft_printf("Error"), 0);
	return (1);
}