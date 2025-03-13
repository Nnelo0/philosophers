/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ebroudic <ebroudic@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/12 11:37:47 by ebroudic          #+#    #+#             */
/*   Updated: 2025/03/13 15:17:32 by ebroudic         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

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

int	error(int argc, char **argv)
{
	int	i;

	if (argc < 5 || argc > 6)
		return (printf("usage : ./philo <nb_philo> <time_to_die> \
<time_to_eat> <time_to_sleep> (<nb_of_eat>)\n"), 1);
	i = 1;
	while (argv[i])
	{
		if (!ft_isdigit_s(argv[i]))
			return (printf("args must be a positive integer\n"), 1);
		if (ft_atol(argv[i]) > INT_MAX || ft_atol(argv[i]) < 0)
			return (printf("args must be a positive integer\n"), 1);
		i++;
	}
	return (0);
}

int	init_data(int argc, char **argv, t_data *data)
{
	int	i;

	i = 0;
	data->nb_philo = ft_atol(argv[1]);
	if (data->nb_philo <= 0 || data->nb_philo > 200)
		return (printf("nb_of_philo must be in 1 and 200\n"), 1);
	data->time_die = ft_atol(argv[2]);
	data->time_eat = ft_atol(argv[3]);
	data->time_sleep = ft_atol(argv[4]);
	if (argc == 6)
		data->nb_eat = ft_atol(argv[5]);
	else
		data->nb_eat = -1;
	data->simu_over = 0;
	data->start_time = ft_time(0);
	data->fork = malloc(sizeof(pthread_mutex_t) * data->nb_philo);
	if (!data->fork)
		return (printf("Malloc failed\n"), 1);
	while (i < data->nb_philo)
		(pthread_mutex_init(&data->fork[i], NULL), i++);
	pthread_mutex_init(&data->print_mutex, NULL);
	pthread_mutex_init(&data->simu_mutex, NULL);
	return (0);
}

t_philo	*init_philo(t_data *data)
{
	int		i;

	i = 0;
	data->philo = malloc(sizeof(t_philo) * data->nb_philo);
	if (!data->philo)
		return (printf("Malloc error\n"), NULL);
	while (i < data->nb_philo)
	{
		data->philo[i].id = i + 1;
		data->philo[i].left_fork = i;
		data->philo[i].right_fork = (i + 1) % data->nb_philo;
		data->philo[i].meal_count = 0;
		data->philo[i].last_meal = data->start_time;
		pthread_mutex_init(&data->philo[i].meal_mutex, NULL);
		data->philo[i].data = data;
		i++;
	}
	return (data->philo);
}

int	main(int argc, char **argv)
{
	t_data		data;
	t_philo		*philo;

	if (error(argc, argv))
		return (1);
	if (init_data(argc, argv, &data))
		return (1);
	if (data.nb_philo == 1)
		return (printf("%lld 1 has taken a fork\n",
				ft_time(data.start_time)), usleep(data.time_die * 1000),
			printf(RED "%lld 1 died\n" RESET, ft_time(data.start_time))
			, free(data.fork), 0);
	philo = init_philo(&data);
	if (!philo)
		return (1);
	create_threads(philo, &data);
	free(data.fork);
	free(data.philo);
	return (0);
}
