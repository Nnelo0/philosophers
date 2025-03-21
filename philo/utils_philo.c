/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_philo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 11:37:43 by ebroudic          #+#    #+#             */
/*   Updated: 2025/03/20 15:13:21 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	unlock_fork(t_data *data, t_philo *philo)
{
	pthread_mutex_unlock(&data->fork[philo->right_fork[0]]);
	pthread_mutex_unlock(&data->fork[philo->left_fork[0]]);
}

void	mutex_take_fork(t_philo *philo, t_data *data, char c)
{
	(void)data;
	if (c == 'l')
	{
		philo->left_fork[1] = 1;
	}
	else
	{
		philo->right_fork[1] = 1;
	}
}

void	mutex_untake_fork(t_philo *philo, t_data *data, char c)
{
	(void)data;
	if (c == 'l')
	{
		philo->left_fork[1] = 0;
	}
	else if (c == 'r')
	{
		philo->right_fork[1] = 0;
	}
	else
	{
		philo->left_fork[1] = 0;
		philo->right_fork[1] = 0;
	}
}

int	check_death_fork(t_philo *philo, t_data *data, char c)
{
	printf(YELLOW "temps : <%lld>\nphilo: [%d]\nright: [%d]\nleft: {%d}\n\
test :<%d>\n" RESET,ft_time(data->start_time),philo->id, philo->right_fork[1],
		philo->left_fork[1], philo->id - 1);
	if (c == 'l')
	{
		while (philo->left_fork[1])
		{
			if (check_death(philo, data))
				return (1);
		}
	}
	else
	{
		while (philo->right_fork[1])
		{
			if (check_death(philo, data))
				return (1);
		}
	}
	return (0);
}

int	take_odd_fork(t_philo *philo, t_data *data)
{
	if (check_death_fork(philo, data, 'r'))
		return (1);
	pthread_mutex_lock(&data->fork[philo->right_fork[0]]);
	mutex_take_fork(philo, data, 'r');
	if (check_death(philo, data))
	{
		pthread_mutex_unlock(&data->fork[philo->right_fork[0]]);
		mutex_untake_fork(philo, data, 'r');
		return (1);
	}
	print_mutex(data, RESET, philo->id, "has taken right fork\n");
	if (check_death(philo, data))
	{
		pthread_mutex_unlock(&data->fork[philo->right_fork[0]]);
		mutex_untake_fork(philo, data, 'r');
		return (1);
	}
	if (check_death_fork(philo, data, 'l'))
	{
		pthread_mutex_unlock(&data->fork[philo->right_fork[0]]);
		mutex_untake_fork(philo, data, 'r');
		return (1);
	}
	pthread_mutex_lock(&data->fork[philo->left_fork[0]]);
	mutex_take_fork(philo, data, 'l');
	if (check_death(philo, data))
	{
		unlock_fork(data, philo);
		mutex_untake_fork(philo, data, 'a');
		return (1);
	}
	print_mutex(data, RESET, philo->id, "has taken left fork\n");
	return (0);
}

int	take_fork(t_philo *philo, t_data *data)
{
	if (philo->id % 2 == 0)
	{
		if (check_death_fork(philo, data, 'l'))
			return (1);
		pthread_mutex_lock(&data->fork[philo->left_fork[0]]);
		mutex_take_fork(philo, data, 'l');
		if (check_death(philo, data))
		{
			pthread_mutex_unlock(&data->fork[philo->left_fork[0]]);
			mutex_untake_fork(philo, data, 'l');
			return (1);
		}
		print_mutex(data, RESET, philo->id, "has taken left fork\n");
		if (check_death(philo, data))
		{
			pthread_mutex_unlock(&data->fork[philo->left_fork[0]]);
			mutex_untake_fork(philo, data, 'l');
			return (1);
		}
		if (check_death_fork(philo, data, 'r'))
		{
			pthread_mutex_unlock(&data->fork[philo->left_fork[0]]);
			mutex_untake_fork(philo, data, 'l');
			return (1);
		}
		pthread_mutex_lock(&data->fork[philo->right_fork[0]]);
		mutex_take_fork(philo, data, 'r');
		if (check_death(philo, data))
		{
			unlock_fork(data, philo);
			mutex_untake_fork(philo, data, 'a');
			return (1);
		}
		print_mutex(data, RESET, philo->id, "has taken right fork\n");
	}
	else
	{
		if (take_odd_fork(philo, data))
			return (1);
	}
	if (check_death(philo, data))
		return (unlock_fork(data, philo), 1);
	return (0);
}

int	one_philo(t_data *data)
{
	if (data->nb_philo == 1)
	{
		if (data->time_die != 0)
			printf("%lld 1 has taken a fork\n", ft_time(data->start_time));
		usleep(data->time_die * 1000);
		printf(RED "%lld 1 died\n" RESET, ft_time(data->start_time));
		return (1);
	}
	return (0);
}
