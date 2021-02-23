#include <unistd.h>
#include <stdio.h> 
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h> 
#include <string.h> 

int main(int argc, char** argv) {
	
	int number_of_processes = argc - 1;
	int number_of_pipes = 2 * number_of_processes; 
	pid_t* process_id = malloc(number_of_processes*sizeof(pid_t));
       	int* child_fd = malloc(number_of_pipes*sizeof(int)); 
	int* parent_fd = malloc(number_of_pipes*sizeof(int)); 

	for ( int i = 0; i < argc-1; i++ ) {
		int lower_num, upper_num;
		lower_num = atoi(strtok(argv[i+1], ":"));
		upper_num = atoi(strtok(NULL, ":")); 
		if ( lower_num > upper_num ) {
			free(process_id); 
			free(child_fd); 
			free(parent_fd); 
			perror("Lower range must be less than Upper range! Exiting\n"); 
			return -1; 
		}
		pipe(parent_fd+(i*2)); 
		pipe(child_fd+(i*2)); 
	}

	for ( int i = 0; i < number_of_processes; ++i) {
		*(process_id+i) = fork(); 
	}

	for ( int i = 0; i < number_of_processes; i++) {
		if (*(process_id+i) == 0) {
			int from_parent_fd[2] = ; 
			printf("Child\n");
			char* total_range; 
			if ( i % 2) {

				// close(parent_fd[i+1]); 
				read(from_parent_fd[0], &total_range, sizeof(char*));
			      	printf("%s", total_range); 	
			} else {

			}
		       	exit(0);	
		}
	}

	printf("Parent\n");
	for ( int i = 0; i < number_of_processes; i++) {
		if ( !(i % 2) ) { 
			// close(*(parent_fd+i));
			write(parent_fd[i+1], argv[i+1], sizeof(argv[i+1])); 
		} else { 
			//close(*(parent_fd+i+1)); 
		}
	}
	
	int status; 
	pid_t pid; 
	while (number_of_processes > 0) {
		pid = wait(&status); 
		printf("%d exited\n", pid); 
		number_of_processes--; 
	}	
	
}
