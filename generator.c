#include <unistd.h>
#include <stdio.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h> 
#include <string.h> 

struct range_struct {
	int lower;
	int upper; 
};

struct fd_array {
	int parent_fd[2];
       	int child_fd[2];	
};

int getNumberOfPrimes(int lower, int upper); 

int main(int argc, char** argv) {
	// get total number of processes needed
	int number_of_processes = argc - 1; 
	// get total number of pipes needed
	int number_of_pipes = 2 * number_of_processes; 
	// allocate based on number of processes, in order to differentiate between the children 
	pid_t* process_id = malloc(number_of_processes*sizeof(pid_t));
	// allocate fds, for both parent and children. each process creates 2 pipes
	struct fd_array* fd = malloc(number_of_processes*sizeof(struct fd_array));
	// create struct array to hold upper and lower ranges	
       	struct range_struct* ranges = malloc(number_of_processes*sizeof(struct range_struct)); 	
	// parse through arguments
	for ( int i = 0; i < argc-1; i++ ) {
		int lower_num, upper_num;
		lower_num = atoi(strtok(argv[i+1], ":"));
		upper_num = atoi(strtok(NULL, ":")); 
		// make sure that lower is smaller than upper
		if ( lower_num > upper_num ) { 
			free(process_id); 
			perror("Lower range must be less than Upper range! Exiting\n"); 
			return -1; 
		}
		// place upper and lower values into struct
		ranges[i].lower = lower_num; 
		ranges[i].upper = upper_num; 
		// create pipes
		pipe(fd[i].parent_fd);
		pipe(fd[i].child_fd);
	       		
	}
	// fork appropriate number of processes 
	for ( int i = 0; i < number_of_processes; ++i) {
		*(process_id+i) = fork(); 
	}
	// children
	for ( int i = 0; i < number_of_processes; i++) {
		if (*(process_id+i) == 0) {
			// printf("Child\n");
			close(fd[i].parent_fd[1]); 
			close(fd[i].child_fd[0]); 
			struct range_struct* range;	
			read(fd[i].parent_fd[0], range, sizeof(struct range_struct)); 
			// printf("Lower range: %d\nUpper range: %d\n", range->lower, range->upper); 
			int nbrOfPrimes = getNumberOfPrimes( range->lower, range->lower); 
			// printf("done"); 
			printf("Child %d generates %d primes in [%d:%d]\n", getpid(), nbrOfPrimes, range->lower, range->upper); 
			write(fd[i].child_fd[1], &nbrOfPrimes, 8);  	
		       	exit(0);	
		}
	}
	// parent 
	printf("Parent\n");
	for ( int i = 0; i < number_of_processes; i++) {
		close(fd[i].parent_fd[0]);
		struct range_struct* to_pass = &ranges[i]; 
		write(fd[i].parent_fd[1], to_pass, sizeof(struct range_struct));  
	}
	
	int status; 
	pid_t pid; 
	while (number_of_processes > 0) {
		pid = wait(&status); 
		printf("%d exited\n", pid); 
		number_of_processes--; 
	}	
	
}
// get the total number of primes and return that value 
int getNumberOfPrimes(int lower, int upper) {
	int numberOfPrimes = 0;

	for( int i = lower+1; i <= upper; i++) {
		if ( i == 1 || i == 0 )
		       continue; 

		int isPrime = 1;

		for (int j = 2; j < i / 2; j++)  {
			if ( i % j == 0 ) {
				isPrime = 0; 
				break; 
			}
		}
		if (isPrime) {
			numberOfPrimes++; 
		}	

	}

	return numberOfPrimes; 
}
