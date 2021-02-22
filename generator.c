#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h> 

struct range {
	int lower;
	int upper; 
} *ranges; // struct to hold the upper and lower ranges of the numbers 

int getPrimes(int lower, int upper); 

int main(int argc, char** argv) {
	pid_t *process_id; // dynamic array to hold all the process ids 
	pid_t parent_id = getpid(); 
	int** parent_write_fd; // for the parent to write to the children 
       	int** child_write_fd; 	// for the children to write to the parent
	parent_write_fd = (int**)malloc(argc*sizeof(int*)); 
	child_write_fd = (int**)malloc(argc*sizeof(int*)); 
	ranges = (struct range*)malloc(argc*sizeof(struct range)); // allocate space for each range 
	for ( int i = 1; i < argc; ++i) {
		(ranges+i)->lower = atoi(strtok(argv[i], ":")); // place lower range into struct 
	       	(ranges+i)->upper = atoi(strtok(NULL, ":"));  // place upper range into struct
		printf("Lower Range: %i\n", (ranges+i)->lower); 
		printf("Upper Range: %i\n", (ranges+i)->upper); 
		*(parent_write_fd+i) = (int*)malloc(2*sizeof(int)); // create pipe for parent to write to child 
		*(child_write_fd+i) = (int*)malloc(2*sizeof(int)); // create pipe for child to write to parent 
	       	pipe(*(parent_write_fd+i)); // pipe system call for parent to child 
		pipe(*(child_write_fd+i)); // pipe system call for child to parent 	
		if ( (ranges+i)->lower >= (ranges+i)->upper) { // if the lower range is larger than the upper range, then the program will throw an error
			// free(lower_range);
			// free(upper_range);
			free(ranges); 
		       	free(process_id); 	
			free(parent_write_fd);
			free(child_write_fd);	
			perror("LowerRange:UpperRange, Lower range must be smaller than Upper Range!"); 
			return -1; 
		}

	}
		

	process_id = (pid_t*)malloc(argc*sizeof(pid_t)); // allocating space for all fork process ids
	
	// fd = (int**)malloc(argc*sizeof(int*)); 
	
	for ( int i = 1; i < argc; i++) {
		*(process_id+i) = fork(); // fork
	        // *(fd+i) = (int*)malloc(2*sizeof(int)); 
		// pipe(*(fd+i));i	
	}
	
	// int children = argc; 
	// children go here 
	for ( int i = 1; i < argc; ++i) {
		if(*(process_id+i) == 0) { // child 
			sleep(1);
		       	printf("child\n"); 	
			struct range *current_range; 
			close(*(*(parent_write_fd+1)+i)); // closing write end of pipe, using different pipe to write to parent 
				
			read(*(*(parent_write_fd)+i), current_range, sizeof(struct range));  
			return 0; 
		       		
		}
	}	
	
	// Parent should start here
	printf("parent\n"); 
	for (int i = 1; i < argc; ++i) {
		close(parent_write_fd[i][0]); // close read end of pipe, using different pipe to get information from child
		write(parent_write_fd[i][1], &ranges[i], sizeof(struct range)); 
	}
	free(ranges); 
       	free(process_id); 	
	free(parent_write_fd);
	free(child_write_fd);	

	return 0; 
}

int getPrimes(int lower, int upper) {
	int numberOfPrimes;

	return numberOfPrimes;
}
