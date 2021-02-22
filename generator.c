#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h> 

struct range {
	int lower;
	int upper; 
} *ranges;

int main(int argc, char** argv) {
	pid_t *process_id; // dynamic array to hold all the process ids 
	// int* lower_range; 
	// int* upper_range;
	// struct range *ranges;  // struct to hold all the ranges 
	int** parent_write_fd; // for the parent to write to the children 
       	int** child_write_fd; 	// for the children to write to the parent
	parent_write_fd = (int**)malloc(argc*sizeof(int*)); 
	child_write_fd = (int**)malloc(argc*sizeof(int*)); 
	// lower_range = (int*)malloc(argc*sizeof(int)); 
	// upper_range = (int*)malloc(argc*sizeof(int));
	ranges = (struct range*)malloc(argc*sizeof(struct range)); // allocate space for each range 
	//fd = (int**)malloc(argc*sizeof(int*));  
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
		// pipe(*(fd+i));	
	}
	
	// int children = argc; 
	
	for ( int i = 1; i < argc; ++i) {
		if(*(process_id+i) == 0) { // child 
			close(*(*(fd+1)+j)); // closing write end of pipe, using different pipe to write to parent  
		}
		else { // parent 
			printf("Parent\n"); 
			for ( int j = 1; j < argc; ++j) {
				close(*(*(fd)+j)); // close read end of pipe, using different pipe to get information from child
				write(*(*(fd+1)+j), (ranges+j), sizeof(struct range)); // give each child a struct to work with   
			}
		}
	}	
	
	
	// free(process_id);
	// free(lower_range);
	// free(upper_range); 
	free(ranges); 
       	free(process_id); 	
	free(parent_write_fd);
	free(child_write_fd);	

	return 0; 
}
