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
