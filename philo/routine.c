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
			if (get_timestamp_ms() - data->philo[i].last_meal >= data->time_die)
			{
				printf("%lld %d died\n", get_timestamp_ms(), data->philo[i].id);
				data->simu_over = 1;
				pthread_mutex_unlock(&data->simu_mutex);
				return (NULL);
			}
			pthread_mutex_unlock(&data->simu_mutex);
			i++;
		}
	}
	return (NULL);
}

static void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	while (1)
	{
		pthread_mutex_lock(&data->simu_mutex);
		if (data->simu_over != 0)
			return (pthread_mutex_unlock(&data->simu_mutex), NULL);
		pthread_mutex_unlock(&data->simu_mutex);
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
