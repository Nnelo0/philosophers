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

int	take_fork(t_philo *philo, t_data *data)
{
	if (philo->id % 2 == 0)
	{
		if (check_death(philo, data))
			return (1);
		pthread_mutex_lock(&data->fork[philo->right_fork]);
		if (check_death(philo, data))
			return (pthread_mutex_unlock(&data->fork[philo->right_fork]), 1);
		print_mutex(data, RESET, philo->id, "has taken right fork\n");
		if (check_death(philo, data))
			return (pthread_mutex_unlock(&data->fork[philo->right_fork]), 1);
		pthread_mutex_lock(&data->fork[philo->left_fork]);
		if (check_death(philo, data))
		return (pthread_mutex_unlock(&data->fork[philo->right_fork]),pthread_mutex_unlock(&data->fork[philo->left_fork]), 1);
		print_mutex(data, RESET, philo->id, "has taken left fork\n");
	}
	else
	{
		if (check_death(philo, data))
			return (1);
		pthread_mutex_lock(&data->fork[philo->left_fork]);
		if (check_death(philo, data))
			return (pthread_mutex_unlock(&data->fork[philo->left_fork]), 1);
		print_mutex(data, RESET, philo->id, "has taken left fork\n");
		if (check_death(philo, data))
			return (pthread_mutex_unlock(&data->fork[philo->left_fork]), 1);
		pthread_mutex_lock(&data->fork[philo->right_fork]);
		if (check_death(philo, data))
		return (pthread_mutex_unlock(&data->fork[philo->right_fork]),pthread_mutex_unlock(&data->fork[philo->left_fork]), 1);
		print_mutex(data, RESET, philo->id, "has taken right fork\n");
	}
	if (check_death(philo, data))
		return (pthread_mutex_unlock(&data->fork[philo->right_fork]),pthread_mutex_unlock(&data->fork[philo->left_fork]), 1);
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
		usleep(100);
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
		usleep(100);
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
		usleep(1000);
		if (check_death(philo, data))
			return (NULL);
	}
	return (NULL);
}
