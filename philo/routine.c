#include "philo.h"

void	*check_death(void *arg)
{
	t_data	*data;
	int		i;

	data = (t_data *)arg;
	while (1)
	{
		i = 0;
		while (i < data->nb_philo)
		{
			pthread_mutex_lock(&data->simu_mutex);
			pthread_mutex_lock(&data->philo[i].meal_mutex);
			if (get_timestamp_ms() - data->philo[i].last_meal >= data->time_die)
			{
				pthread_mutex_unlock(&data->philo[i].meal_mutex);
				printf("%lld %d died\n", ft_time(data), data->philo[i].id);
				data->simu_over = 1;
				pthread_mutex_unlock(&data->simu_mutex);
				return (NULL);
			}
			pthread_mutex_unlock(&data->philo[i].meal_mutex);
			pthread_mutex_unlock(&data->simu_mutex);
			i++;
		}
		usleep(500);
	}
	return (NULL);
}

static void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	if (philo->id % 2 == 0)
		usleep(100);
	while (1)
	{
		pthread_mutex_lock(&data->simu_mutex);
		if (data->simu_over != 0)
			return (pthread_mutex_unlock(&data->simu_mutex), NULL);
		pthread_mutex_unlock(&data->simu_mutex);
		pthread_mutex_lock(&data->fork[philo->left_fork]);
		printf("%lld %d has taken a fork\n", ft_time(data), philo->id);
		pthread_mutex_lock(&data->fork[philo->right_fork]);
		printf("%lld %d has taken a fork\n", ft_time(data), philo->id);
		pthread_mutex_lock(&data->simu_mutex);
		pthread_mutex_lock(&philo->meal_mutex);
		philo->last_meal = get_timestamp_ms();
		pthread_mutex_unlock(&philo->meal_mutex);
		printf("%lld %d is eating\n", ft_time(data), philo->id);
		pthread_mutex_unlock(&data->simu_mutex);
		usleep(data->time_eat * 1000);
		pthread_mutex_lock(&data->simu_mutex);
		if (data->simu_over)
			return (pthread_mutex_unlock(&data->simu_mutex),
				pthread_mutex_unlock(&data->fork[philo->left_fork]),
				pthread_mutex_unlock(&data->fork[philo->right_fork]), NULL);
		pthread_mutex_unlock(&data->simu_mutex);
		pthread_mutex_unlock(&data->fork[philo->left_fork]);
		pthread_mutex_unlock(&data->fork[philo->right_fork]);
		printf("%lld %d is sleeping\n", ft_time(data), philo->id);
		usleep(data->time_sleep * 1000);
		pthread_mutex_lock(&data->simu_mutex);
		if (data->simu_over)
			return (pthread_mutex_unlock(&data->simu_mutex), NULL);
		pthread_mutex_unlock(&data->simu_mutex);
		printf("%lld %d is thinking\n", ft_time(data), philo->id);
	}
	return (NULL);
}

int	create_threads(t_philo *philo, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		if (pthread_create(&philo[i].thread, NULL, philo_routine,
				&philo[i]) != 0)
			return (printf("Error creating thread\n"), 1);
		i++;
	}
	i = -1;
	while (++i < data->nb_philo)
		pthread_join(philo[i].thread, NULL);
	return (0);
}
