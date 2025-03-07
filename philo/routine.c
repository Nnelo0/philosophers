#include "philo.h"

void	check_death(t_philo *philo, t_data *data)
{
	if (get_timestamp_ms() - philo->last_meal >= data->time_die)
		data->simu_over = 1;
}

static void	*philo_routine(void *arg)
{
	t_philo	*philo;
	t_data	*data;

	philo = (t_philo *)arg;
	data = philo->data;
	if (philo->id % 2 == 0)
		usleep(300);
	while (1)
	{
		check_death(philo, data);
		if (data->simu_over == 1)
			return (printf(RED "%lld %d died\n" RESET, ft_time(data), philo->id), NULL);
		pthread_mutex_lock(&data->fork[philo->right_fork]);
		printf("%lld %d has taken a rigth fork\n", ft_time(data), philo->id);
		pthread_mutex_lock(&data->fork[philo->left_fork]);
		printf("%lld %d has taken a left fork\n", ft_time(data), philo->id);
		philo->meal_count += 1;
		philo->last_meal = get_timestamp_ms();
		printf(GREEN "%lld %d is eating\n" RESET, ft_time(data), philo->id);
		usleep(1000 * data->time_eat);
		pthread_mutex_unlock(&data->fork[philo->right_fork]);
		pthread_mutex_unlock(&data->fork[philo->left_fork]);
		if (data->simu_over == 1)
			return (printf(RED "%lld %d died\n" RESET, ft_time(data), philo->id), NULL);
		printf(MAGENTA "%lld %d is sleeping\n" RESET, ft_time(data), philo->id);
		usleep(1000 * data->time_sleep);
		printf(CYAN "%lld %d is thinking\n" RESET, ft_time(data), philo->id);
		if (data->simu_over == 1)
			return (printf(RED "%lld %d died\n" RESET, ft_time(data), philo->id), NULL);
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
