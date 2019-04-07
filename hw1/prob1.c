#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>


int main(){
	int n = 0;

	pid_t pid;

	do {
		printf("Enter a positive integer: ");
		scanf("%d", &n);
	} while (n <= 0);
	
	pid = fork();


	if (pid < 0){
		fprintf(stderr, "Error occurred.\n" );
		return 1;
	}
	else if (pid == 0){
		//child process

		while (n != 1) {
			printf("%d, ", n);
			if (n % 2 == 0) {
				n = n / 2;
			}
			else {
				n = 3 * n + 1;
			}
		}
		printf("%d\n", n);

	}
	else {
		// parent process
		wait(NULL);
		
	}

	return 0;
}