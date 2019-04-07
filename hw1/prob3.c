#include <stdio.h>
#include <unistd.h>
#include <fcntl.h> 
#include <sys/stat.h>


int main (int argc, char* argv[]) {

    int fd[2];
    pid_t pid;
    char buffer[100];
    char child_buffer[100];
    
    if (argc != 3) {
        fprintf(stderr, "Invalid arguments (%d): <target file> <output file>.\n", argc - 1);
        return 1;
    }
    
    char* srcfile = argv[1];
    char* outfile = argv[2];

    // Create a pipe, return -1 when error exists
    if (pipe(fd) < 0) {
        fprintf(stderr, "Pipe creating error.\n");
        return 1;
    }

    pid = fork();

    if (pid < 0){
        printf("Error forking child process.");
        return 1;
    }
    
    else if (pid == 0) { 
        // child process
        close(fd[1]);
        ssize_t child_num_bytes = read(fd[0], child_buffer, sizeof(child_buffer));
        close(fd[0]);

        // write output file
        printf("Copy %s to %s...\n", argv[1], argv[2]);
        int outfd = open(outfile, O_CREAT | O_WRONLY);
        write(outfd, child_buffer, child_num_bytes);
        chmod(outfile, 0644);
        printf("Done!\n");
    }
    
    else {
        // parent process
        
        close(fd[0]);
        int infd = open(srcfile, O_RDONLY);
        ssize_t num_bytes = read(infd, buffer, sizeof(buffer));
        write(fd[1], buffer, num_bytes);
        close(fd[1]);
    }
    
    return 0;
}
