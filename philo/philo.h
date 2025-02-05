#ifndef PHILO_H
# define PHILO_H
# include <stdlib.h>
# include <string.h>
# include <stdio.h>
# include <unistd.h>
# include <fcntl.h>
# include <sys/time.h>
# include <pthread.h>
# include <limits.h>

typedef struct s_philo	t_philo;

typedef struct s_data
{
	int				nb_philo;
	int				time_die;
	int				time_eat;
	int				time_sleep;
	int				nb_eat;
	int				simu_over;
	long long		start_time;
	t_philo			*philo;
	pthread_mutex_t	*fork;
	pthread_mutex_t	write_mutex;
	pthread_mutex_t	simu_mutex;
}	t_data;

typedef struct s_philo
{
	int				id;
	int				left_fork;
	int				right_fork;
	int				meal_count;
	long long		last_meal;
	pthread_mutex_t	meal_mutex;
	t_data			*data;
	pthread_t		thread;
}	t_philo;

int			ft_atoi(const char *nptr);
int			ft_isdigit_s(char *s);
long long	get_timestamp_ms(void);
int			create_threads(t_philo *philo, t_data *data);
void		*check_death(void *arg);
long long	ft_time(t_data *data);
void		destroy_mutex(t_data *data);

#endif