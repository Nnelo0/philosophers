/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   routine.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 11:37:49 by ebroudic          #+#    #+#             */
/*   Updated: 2025/03/13 15:17:15 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

int	check_death(t_philo *philo, t_data *data)
{
	pthread_mutex_lock(&data->simu_mutex);
	if (data->simu_over)
	{
		pthread_mutex_unlock(&data->simu_mutex);
		return (1);
	}
	if (ft_time(0) - philo->last_meal >= data->time_die)
	{
		print_mutex(data, RED, philo->id, "died\n");
		data->simu_over = 1;
		pthread_mutex_unlock(&data->simu_mutex);
		return (1);
	}
	if (data->simu_over)
	{
		pthread_mutex_unlock(&data->simu_mutex);
		return (1);
	}
	pthread_mutex_unlock(&data->simu_mutex);
	return (0);
}

int	philo_eat(t_philo *philo, t_data *data)
{
	long	start_eat;

	if (take_fork(philo, data))
		return (1);
	philo->meal_count += 1;
	philo->last_meal = ft_time(0);
	if (check_death(philo, data))
		return (pthread_mutex_unlock(&data->fork[philo->right_fork])
			, pthread_mutex_unlock(&data->fork[philo->left_fork]), 1);
	print_mutex(data, GREEN, philo->id, "is eating\n");
	start_eat = ft_time(0);
	while (ft_time(start_eat) < data->time_eat)
	{
		if (check_death(philo, data))
			return (pthread_mutex_unlock(&data->fork[philo->right_fork])
				, pthread_mutex_unlock(&data->fork[philo->left_fork]), 1);
		usleep(500);
	}
	pthread_mutex_unlock(&data->fork[philo->right_fork]);
	pthread_mutex_unlock(&data->fork[philo->left_fork]);
	return (0);
}

int	philo_sleep(t_philo *philo, t_data *data)
{
	long	start_sleep;

	if (check_death(philo, data))
		return (1);
	start_sleep = ft_time(0);
	print_mutex(data, MAGENTA, philo->id, "is sleeping\n");
	while (ft_time(start_sleep) < data->time_sleep)
	{
		if (check_death(philo, data))
			return (1);
		usleep(500);
	}
	return (0);
}

void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	while (philo->meal_count != data->nb_eat)
	{
		if (check_death(philo, data))
			return (NULL);
		if (philo_eat(philo, data))
			return (NULL);
		if (check_death(philo, data))
			return (NULL);
		if (philo_sleep(philo, data))
			return (NULL);
		if (check_death(philo, data))
			return (NULL);
		print_mutex(data, CYAN, philo->id, "is thinking\n");
		usleep(500);
		if (check_death(philo, data))
			return (NULL);
	}
	return (NULL);
}
