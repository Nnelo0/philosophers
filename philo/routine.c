#include "philo.h"

static void	*philo_routine(void *arg)
{
	t_philo	*philo;

	philo = (t_philo *)arg;
	printf("Philosophe %d est prêt !\n", philo->id);
	return (NULL);
}

int	create_threads(t_philo *philo, t_data *data)
{
	int	i;

	i = 0;
	while (i < data->nb_philo)
	{
		if (pthread_create(&philo[i].thread, NULL,
				philo_routine, &philo[i]) != 0)
			return (printf("Error creating thread\n"), 1);
		i++;
	}
	i = -1;
	while (++i < data->nb_philo)
		pthread_join(philo[i].thread, NULL);
	return (0);
}
