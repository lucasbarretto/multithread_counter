#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

#define NUM_WORKERS 4
#define MAX_INPUT_SIZE 100

unsigned long long int input[MAX_INPUT_SIZE]; // input storage
int prime_count, input_idx, input_size, necessary_threads;
pthread_mutex_t lock;

// function to detec is integer x is prime
bool is_prime(int x){
	int i;

	// primos sao sempre maiores ou igual a 2
	if(x < 2) return false;

	// se x for 2 ou 3, x eh primo
	else if(x == 2 || x == 3) return true;

	// se x for divisivel por 2 ou por 3, x nao eh primo
	else if(x % 2 == 0 || x % 3 == 0) return false;

	// todo primo maior que 3 eh da forma 6k +- 1, k inteiro
	i = 5;
	while(i*i <= x){
		if(x % i == 0 || x % (i+2) == 0) return false;
		i = i + 6;
	}

	return true;
}

void* worker(void* arg){
	int *N = (int*)(arg);
  	int M = (*N);

  	//printf("entrei na thread %d\n", M);

  	while(true){
  		//printf("thread %d: trying to lock\n", M);
		pthread_mutex_lock(&lock);
		//printf("thread %d: locked\n", M);
		//printf("thread %d: input idx = %d\n", M, input_idx);

		if(input_idx >= input_size){
			//printf("thread %d: idx is %d -> break\n", M, input_idx);
			pthread_mutex_unlock(&lock);
			break;
		}

	 	if(is_prime(input[input_idx])){
			prime_count++;
			//printf("thread %d: input %llu is prime\n", M, input[input_idx]);
		}

		//else printf("thread %d: input %llu is NOT prime\n", M, input[input_idx]);
		input_idx++;
		//printf("thread %d: increased idx to %d\n", M, input_idx);

		pthread_mutex_unlock(&lock);
		//printf("thread %d: unlocked\n", M);
  	}

  	//printf("saindo da thread %d\n", M);
	return NULL;
}

// main program
int main() {

	int i; // index variable
	char c; // auxiliary char variable 
	pthread_t workers[NUM_WORKERS];
	int thread_id[NUM_WORKERS];

	prime_count = 0;
	input_idx = 0;

	// receives a list of integers as input
	i = 0;
	while(c != '\n'){
		scanf("%llu", &input[i]);
		c = getchar();
		i++;
	}

	// stores the total input size
	input_size = i;
	//printf("input size: %d\n", input_size);

	if(input_size > NUM_WORKERS) necessary_threads = NUM_WORKERS;
	else necessary_threads = input_size;

	//printf("necessary threads: %d\n", necessary_threads);

	// generates thread IDs
	for (int i = 0; i < necessary_threads; i++) {
		thread_id[i] = i;
	}

	// creates threads
	for (int i = 0; i < necessary_threads; i++) {
		pthread_create(&(workers[i]), NULL, worker, (void*)(&thread_id[i]));
	}

	// waits for threads
	for (int i = 0; i < necessary_threads; i++) {
		pthread_join(workers[i], NULL);
	}

	printf("%d\n", prime_count);
	return 0;
}
