#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/wait.h> 
#include <unistd.h>
#include <stdlib.h>
#include <errno.h> 

struct range {
	int lower;
	int upper; 
} *ranges; // struct to hold the upper and lower ranges of the numbers 

int getPrimes(int lower, int upper); 

int main(int argc, char** argv) {
	
	printf("%lu\n", sizeof(struct range)); 
	int processes = (argc - 1) * 2
	// int fd[2]; 
	pid_t *process_id; // dynamic array to hold all the process ids 
	pid_t parent_id = getpid(); 
	int* parent_write_fd; // for the parent to write to the children 
       	int* child_write_fd; 	// for the children to write to the parent
	parent_write_fd = (int*)malloc((2*argc)*sizeof(int)); 
	child_write_fd = (int*)malloc((2*argc)*sizeof(int)); 
	// parent_write_fd = (int**)malloc(argc*sizeof(int*)); 
	// child_write_fd = (int**)malloc(argc*sizeof(int*)); 
//	ranges = (struct range*)malloc(argc*sizeof(struct range));  // allocate space for each range 
	
	for ( int i = 1; i < argc; i++) {
//		(ranges+i)->lower = atoi(strtok(argv[i], ":")); // place lower range into struct 
//	       	(ranges+i)->upper = atoi(strtok(NULL, ":"));  // place upper range into struct
		// *(parent_write_fd+i) = (int*)malloc(2*sizeof(int)); // create pipe for parent to write to child 
		// *(child_write_fd+i) = (int*)malloc(2*sizeof(int)); // create pipe for child to write to parent
		// *(child_write_fd+i)[0] = malloc(sizeof(int));
	       	//*(parent_write_fd+i)[1] = malloc(sizeof(int)); 
		// parent_write_fd++;
	       	// child_write_fd++; 	
	       	/*if( pipe(*(parent_write_fd+(i-1))) < 0) {
			perror("Pipe creation failed"); 	
		} // pipe system call for parent to child 
		if( pipe(*(child_write_fd+(i-1))) < 0) {

			perror("PIpe creation failed"); 	
		}; // pipe system call for child to parent */  
			
	/*	if ( (ranges+i)->lower >= (ranges+i)->upper) { // if the lower range is larger than the upper range, then the program will throw an error
			free(ranges); 
		       	free(process_id); 	
			free(parent_write_fd);
			free(child_write_fd);	
			perror("LowerRange:UpperRange, Lower range must be smaller than Upper Range!"); 
			return -1; 
		}*/
		pipe(parent_write_fd+(i*2));
	       	pipe(parent_write_fd+(i*2)); 	
	}
		

	process_id = (pid_t*)malloc(argc*sizeof(pid_t)); // allocating space for all fork process ids
	
	// fd = (int**)malloc(argc*sizeof(int*)); 
	// pipe(fd); 
	for ( int i = 1; i < argc; i++) {
		 //pipe(parent_write_fd[i]); 
		 //pipe(child_write_fd[i]); 
		*(process_id+i) = fork(); // fork
	        // *(fd+i) = (int*)malloc(2*sizeof(int)); 
		// pipe(*(fd+i));i	
	}
	
	// int children = argc; 
	// children go here 
	for ( int i = 1; i < argc; i++) {
		if(*(process_id+i) == 0) { // child 
			char *num_ranges; 
			//sleep(10);
			// struct range *current_range; 
			// close(*(*(parent_write_fd+1)+i)); // closing write end of pipe, using different pipe to write to parent 
			//close(parent_write_fd[i][1]); 	
			//read(*(*(parent_write_fd)+i), current_range, sizeof(struct range)); 
			printf("CHILD\n"); 
		       		
			int len = read(parent_write_fd[i], num_ranges, sizeof(argv[i]));
			// int len = read(fd[0], num_ranges, sizeof(argv[i])); 
			int lower = atoi(strtok(num_ranges, ":"));
		       	int upper = atoi(strtok(NULL, ":")); 	
			printf("%i", errno); 
			printf("%i\n", len);  
			num_ranges[len] = 0; 
			printf("CHILD!\n"); 
			printf("%s", num_ranges); 
		       	// printf("Lower: %d, Upper: %d", current_range->lower, current_range->upper); 	
			int numberOfPrimes = getPrimes(lower, upper); 
			// printf("%d\n", numberOfPrimes);
		       	printf("I'm a child!\n");	
			
			return 0; 
		       		
		}
	}	
	
	// Parent should start here
	printf("parent\n"); // TODO take out 
	for (int i = 1; i < argc; i++) {
	//	struct range *current_range = NULL;
	       		
	//	current_range = &(ranges[i]); 
		close(parent_write_fd[i-1]); // close read end of pipe, using different pipe to get information from child
		write(parent_write_fd[i], &argv[i], sizeof(argv[i]));
		// write(fd[1], argv[i], sizeof(argv[i])); 
	}

	 
	wait(NULL);
	free(ranges); 
       	free(process_id); 	
	free(parent_write_fd);
	free(child_write_fd);	

	return 0; 
}

int getPrimes(int lower, int upper) {
	int numberOfPrimes = 0;
	
	for( int i = lower; i <= upper; i++) {
		if ( i == 1 || i == 0 )
		       continue; 
		
		int isPrime = 1;

		for (int j = 2; j <= i / 2; ++j)  {
			if ( i % j == 0 ) {
				isPrime = 0; 
				break; 
			}
		}
		if (isPrime == 1) {
			numberOfPrimes++; 
		}	

	}

	return numberOfPrimes;
}

// primes: url TODO 
