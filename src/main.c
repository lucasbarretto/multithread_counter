#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <pthread.h>

// CONSTANTS
#define NUM_WORKERS 4
#define MAX_INPUT_SIZE 100

// GLOBAL VARIABLES
unsigned long long int input[MAX_INPUT_SIZE]; // input storage
int prime_count; // counts the prime number total
int input_idx; // current input index
int input_size; // total input size
int necessary_threads; // number of input size
pthread_mutex_t lock; // mutex lock

// function to detect is integer x is prime
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

// worker function
void* worker(void* arg){
	
	// iterates until every number in the input has been checked	
  	while(true){
		
		pthread_mutex_lock(&lock); // locks access to global variables

		// loop stop condition	
		if(input_idx >= input_size){
			pthread_mutex_unlock(&lock); // unlocks access to global variables
			break; // breaks out of loop
		}

		// checks if number is prime
	 	if(is_prime(input[input_idx])){
			prime_count++;
		}

		input_idx++; // updates the current input index

		pthread_mutex_unlock(&lock); // unlocks access to global variables
  	}

	return NULL;
}

// main program
int main() {

	int i; // index variable
	char c; // auxiliary char variable 
	pthread_t workers[NUM_WORKERS]; // thread vector containing all the workers
	int thread_id[NUM_WORKERS];	// thread ids vector 

	// initializes global variables
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

	// calculates the necessary number of workers
	if(input_size > NUM_WORKERS) necessary_threads = NUM_WORKERS;
	else necessary_threads = input_size;

	// generates thread IDs
	for (int i = 0; i < necessary_threads; i++) {
		thread_id[i] = i;
	}

	// creates threads
	for (int i = 0; i < necessary_threads; i++) {
		pthread_create(&(workers[i]), NULL, worker, (void*)(&thread_id[i]));
	}

	// waits for threads to finish
	for (int i = 0; i < necessary_threads; i++) {
		pthread_join(workers[i], NULL);
	}

	printf("%d\n", prime_count); // prints prime count
	return 0;
}
