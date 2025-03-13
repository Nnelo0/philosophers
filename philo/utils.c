/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 11:37:52 by ebroudic          #+#    #+#             */
/*   Updated: 2025/03/13 15:16:30 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	ft_atol(const char *nptr)
{
	int		i;
	long	result;
	int		sign;

	i = 0;
	result = 0;
	sign = 1;
	while ((nptr[i] >= 9 && nptr[i] <= 13) || nptr[i] == 32)
		i++;
	while (nptr[i] == '-' || nptr[i] == '+')
	{
		if (nptr[i] == '-')
			sign = -sign;
		if (nptr[i + 1] == '-' || nptr[i +1] == '+')
			return (0);
		i++;
	}
	while (nptr[i] >= '0' && nptr[i] <= '9')
	{
		result = result * 10 + (nptr[i] - '0');
		i++;
	}
	return (result * sign);
}

static int	ft_isdigit(int c)
{
	if ('0' <= c && '9' >= c)
		return (1);
	return (0);
}

int	ft_isdigit_s(char *s)
{
	int	i;

	i = 0;
	while (s[i])
	{
		if (!ft_isdigit(s[i]))
			return (0);
		i++;
	}
	return (1);
}

long long	ft_time(long long index)
{
	struct timeval	tv;

	gettimeofday(&tv, NULL);
	return (((tv.tv_sec * 1000LL) + (tv.tv_usec / 1000)) - index);
}

void	print_mutex(t_data *data, char *colors, int id, char *arg)
{
	pthread_mutex_lock(&data->print_mutex);
	printf("%s%lld %d %s" RESET, colors, ft_time(data->start_time), id, arg);
	pthread_mutex_unlock(&data->print_mutex);
}
