/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_philo.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 11:37:43 by ebroudic          #+#    #+#             */
/*   Updated: 2025/03/14 10:03:48 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

void	unlock_fork(t_data *data, t_philo *philo)
{
	pthread_mutex_unlock(&data->fork[philo->right_fork]);
	pthread_mutex_unlock(&data->fork[philo->left_fork]);
}

int	take_odd_fork(t_philo *philo, t_data *data)
{
	if (check_death(philo, data))
		return (1);
	pthread_mutex_lock(&data->fork[philo->right_fork]);
	if (check_death(philo, data))
		return (pthread_mutex_unlock(&data->fork[philo->right_fork]), 1);
	print_mutex(data, RESET, philo->id, "has taken fork\n");
	if (check_death(philo, data))
		return (pthread_mutex_unlock(&data->fork[philo->right_fork]), 1);
	pthread_mutex_lock(&data->fork[philo->left_fork]);
	if (check_death(philo, data))
		return (unlock_fork(data, philo), 1);
	print_mutex(data, RESET, philo->id, "has taken fork\n");
	return (0);
}

int	take_fork(t_philo *philo, t_data *data)
{
	if (philo->id % 2 == 0)
	{
		if (check_death(philo, data))
			return (1);
		pthread_mutex_lock(&data->fork[philo->left_fork]);
		if (check_death(philo, data))
			return (pthread_mutex_unlock(&data->fork[philo->left_fork]), 1);
		print_mutex(data, RESET, philo->id, "has taken fork\n");
		if (check_death(philo, data))
			return (pthread_mutex_unlock(&data->fork[philo->left_fork]), 1);
		pthread_mutex_lock(&data->fork[philo->right_fork]);
		if (check_death(philo, data))
			return (unlock_fork(data, philo), 1);
		print_mutex(data, RESET, philo->id, "has taken fork\n");
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
