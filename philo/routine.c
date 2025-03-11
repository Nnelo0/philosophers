#include "philo.h"

void	check_death(t_philo *philo, t_data *data)
{
	if (get_timestamp_ms() - philo->last_meal >= data->time_die)
		data->simu_over = 1;
}

void	print_mutex(t_data *data, char *colors, int id, char *arg)
{
	pthread_mutex_lock(&data->print_mutex);
	printf("%s %lld %d %s", colors, ft_time(data), id ,arg);
	pthread_mutex_unlock(&data->print_mutex);
}

void	philo_eat(t_philo *philo, t_data *data)
{
	if (philo->id % 2 == 0)
	{
		pthread_mutex_lock(&data->fork[philo->right_fork]);
		print_mutex(data, RESET, philo->id, "has taken right fork\n");
		pthread_mutex_lock(&data->fork[philo->left_fork]);
		print_mutex(data, RESET, philo->id, "has taken left fork\n");
	}
	else
	{
		pthread_mutex_lock(&data->fork[philo->left_fork]);
		print_mutex(data, RESET, philo->id, "has taken left fork\n");
		pthread_mutex_lock(&data->fork[philo->right_fork]);
		print_mutex(data, RESET, philo->id, "has taken right fork\n");
	}
	philo->meal_count += 1;
	philo->last_meal = get_timestamp_ms();
	print_mutex(data, GREEN, philo->id, "is eating\n");
	usleep(1000 * data->time_eat);
	pthread_mutex_unlock(&data->fork[philo->right_fork]);
	pthread_mutex_unlock(&data->fork[philo->left_fork]);
}

static void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	if (philo->id % 2 == 0)
		usleep(300);
	while (philo->meal_count != data->nb_eat)
	{
		philo_eat(philo, data);
		print_mutex(data, MAGENTA, philo->id, "is sleeping\n");
		usleep(1000 * data->time_sleep);
		print_mutex(data, CYAN, philo->id, "is thinking\n");
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
		usleep(50);
		i++;
	}
	i = -1;
	while (++i < data->nb_philo)
		pthread_join(philo[i].thread, NULL);
	return (0);
}
