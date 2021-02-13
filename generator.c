#include <string.h>
#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h> 

int main(int argc, char** argv) {
	pid_t process_id; 
	int* lower_range; 
	int* upper_range; 
	int** fd; 
	
	lower_range = (int*)malloc(argc*sizeof(int));
	upper_range = (int*)malloc(argc*sizeof(int)); 
	fd = (int**)malloc(argc*sizeof(int*));  
	for ( int i = 1; i < argc; i++) {
		*(lower_range+i) = atoi(strtok(argv[i], ":"));
	       	*(upper_range+i) = atoi(strtok(NULL, ":")); 
		*(fd+i) = (int*)malloc(argc*sizeof(int)); 
		pipe(*(fd+i)); 
		if ( *(lower_range+i) >= *(upper_range+i) ) {
			free(lower_range);
			free(upper_range); 
			free(fd);	
			perror("LowerRange:UpperRange, Lower range must be smaller than Upper Range!"); 
			return -1; 
		}

	}
	
	// process_id = (pid_t*)malloc(argc*sizeof(pid_t));

	for ( int i = 1; i < argc; i++) {
		process_id = fork(); // fork 		
	}

	if ( process_id < 0) { // error 
		perror("Could not create child process"); 
		free(lower_range);
		free(upper_range);
	       	free(fd); 	
		return -1; 
	} else if ( process_id == 0) {
		// child process	
	} else {
		// parent process
		for (int i = 1; i < argc; i++) {

			write(*(*(fd + 1) + i),  
		}
	}

	
	// free(process_id);
	free(lower_range);
	free(upper_range); 
	return 0; 
}
