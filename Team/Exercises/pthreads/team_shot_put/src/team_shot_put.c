//  Copyright 2023 Sebastian Venegas

#define _DEFAULT_SOURCE

#include <inttypes.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>
#include <unistd.h>

enum ERRORS {
	INVALID_ARGUMENT,
	INVALID_EVEN_NUMBER,
	ERROR_THREAD_CREATE,
	ERROR_ALLOCATE_THREAD,
	ERROR_ALLOCATE_SHARE_DATA
};

// thread_shared_data_t
typedef struct shared_data {
	size_t athlete_count;
  float* results;
} shared_data_t;

// thread_private_data_t
typedef struct private_data {
  size_t thread_number;
	size_t number_team;
  shared_data_t* shared_data;
} private_data_t;

/**
 * @brief Creates and initialize private_data for each thread, and creates every
 * thread. There are 2 teams of threads.
 * @param shared_data1, public information accesible to every thread of team 1.
 * It must be initialized. Public information is total number of threads and an
 * array of floats that indicates the results of every thread.
 * @param shared_data2, public information accesible to every thread of team 2.
 * It must be initialized. Public information is total number of threads and an
 * array of floats that indicates the results of every thread.
 * @return An error code:
 *  EXIT_SUCCESS for success.
 *  ERROR_ALLOCATE_THREAD if could not create secondary thread
*/
int create_threads(shared_data_t* shared_data1, shared_data_t* shared_data2);
/**
 * @brief Simulates the shooting of a ball, using a pseudo-random number as the
 * length of the shoot, every thread shoots 3 times and saves the highest number
 * on results. The shoot can go between 0 and 25.
 * @param private_data[index], private information of every thread to work in 
 * the function. It must be initialized. Private information is the thread
 * number or rank, which team they are part of and a pointer to the shared_data.
 */
void* shoot_ball(void* data);

int main(int argc, char* argv[]) {
	size_t athlete_count = 0;
	int error = EXIT_SUCCESS;
	if (argc == 2) {
		if (sscanf(argv[1], "%" SCNu64, &athlete_count) == 1) {
		} else {
			fprintf(stderr, "Error: invalid character\n");
			return INVALID_ARGUMENT;
		}
	} else {
		fprintf(stderr, "Error: invalid argument count\n");
		return INVALID_ARGUMENT;
	}

	if ((athlete_count % 2) != 0) {
		shared_data_t* shared_data1 = (shared_data_t*)calloc(1
			, sizeof(shared_data_t));
		shared_data_t* shared_data2 = (shared_data_t*)calloc(1
			, sizeof(shared_data_t));
  	if (shared_data1 && shared_data2) {
			shared_data1->athlete_count = athlete_count;
			shared_data1->results = (float*)calloc(athlete_count, sizeof(float));

			shared_data2->athlete_count = athlete_count;
			shared_data2->results = (float*)calloc(athlete_count, sizeof(float));
			error = create_threads(shared_data1, shared_data2);
			size_t pts_team1 = 0; size_t pts_team2 = 0;
			for (size_t i = 0; i < athlete_count; ++i) {
				if (shared_data1->results[i] < shared_data2->results[i]) {
					pts_team2++;
				} else {
					pts_team1++;
				}
			}
			if (pts_team1 > pts_team2) {
				printf("result %zu:%zu, team 1 wins\n", pts_team1, pts_team2);
			} else {
				printf("result %zu:%zu, team 2 wins\n", pts_team1, pts_team2);
			}
			free(shared_data1->results);
			free(shared_data2->results);
			free(shared_data1);
			free(shared_data2);
		} else {
			fprintf(stderr, "Error: could not allocate shared data\n");
			return ERROR_ALLOCATE_SHARE_DATA;
  	}
	} else {
		fprintf(stderr, "Error: invalid athlete count\n");
		return INVALID_EVEN_NUMBER;
	}
	return error;
}

int create_threads(shared_data_t* shared_data1, shared_data_t* shared_data2) {
	int error = EXIT_SUCCESS;
	pthread_t* team1 = (pthread_t*)
		malloc(shared_data1->athlete_count * sizeof(pthread_t));
	pthread_t* team2 = (pthread_t*)
		malloc(shared_data2->athlete_count * sizeof(pthread_t));
	private_data_t* private_data1 = (private_data_t*)
		calloc(shared_data1->athlete_count, sizeof(private_data_t));
	private_data_t* private_data2 = (private_data_t*)
		calloc(shared_data2->athlete_count, sizeof(private_data_t));
	if (team1 && team2 && private_data1 && private_data2) {
		// Create first team
		for (size_t i = 0; i < shared_data1->athlete_count; ++i) {
			private_data1[i].thread_number = i;
			private_data1[i].number_team = 1;
			private_data1[i].shared_data = shared_data1;
			// create_thread
			error = pthread_create(&team1[i], NULL, shoot_ball
				, &private_data1[i]);
			if (error != EXIT_SUCCESS) {
				fprintf(stderr, "Error: could not create secondary thread\n");
				return ERROR_THREAD_CREATE;
			}
		}
		// Create second team
		size_t num = 0;
		for (size_t i = 0; i < shared_data2->athlete_count; ++i) {
			private_data2[i].thread_number = num;
			private_data2[i].number_team = 2;
			private_data2[i].shared_data = shared_data2;
			// create_thread
			error = pthread_create(&team2[i], NULL, shoot_ball
				, &private_data2[i]);
			if (error != EXIT_SUCCESS) {
				fprintf(stderr, "Error: could not create secondary thread\n");
				return ERROR_THREAD_CREATE;
			}
			++num;
		}

		for (size_t i = 0; i < shared_data1->athlete_count; ++i) {
			pthread_join(team1[i], /*value_ptr*/ NULL);
		}
		for (size_t i = 0; i < shared_data2->athlete_count; ++i) {
			pthread_join(team2[i], /*value_ptr*/ NULL);
		}
		free(private_data1);
		free(private_data2);
		free(team1);
		free(team2);
	} else {
    fprintf(stderr, "Error: could not allocate %zu threads\n"
			, shared_data1->athlete_count);
    error = ERROR_ALLOCATE_THREAD;
  }
  return error;
}

void* shoot_ball(void* data) {
	private_data_t* private_data = (private_data_t*) data;
  shared_data_t* shared_data = private_data->shared_data;
	unsigned int my_seed = pthread_self() * time(NULL) * getpid();
	float best_shot = 0;
	float shot = 0;
	for (size_t i = 0; i < 3; ++i) {
		
		shot = (float)(rand_r(&my_seed) % 25);
		if (best_shot < shot) {
			best_shot = shot;
		}
	}
	printf("%zu.%zu: best shot put %0.2fm\n", private_data->number_team
		, private_data->thread_number+1, best_shot);

	shared_data->results[private_data->thread_number] = best_shot;
	return NULL;
}
