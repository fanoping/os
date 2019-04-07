#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>


int main() {    
    int n = 0;

    pid_t pid;

    // share memory
    const int SIZE=4096;
    const char *name = "test";
    
    int shm_fd;
    int *ptr;
    int *shm_base;

    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);

    ftruncate(shm_fd, SIZE);

    ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);

    do {
        printf("Enter a positive integer: ");
        scanf("%d", &n);

    } while (n <= 0);

    pid = fork();

    if (pid < 0){
        fprintf(stderr, "Error occured.\n" );
        return 1;
    }
    else if (pid == 0) {
        // child process
        
        shm_base = mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
        if (shm_base == MAP_FAILED) {
            fprintf(stderr, "Error occurred.\n");
            return 1;
        }

        int *ptr = shm_base; 
        while(n != 1){
            *ptr = n;
            if (n % 2 == 0) {
                n = n / 2;
            }
            else {
                n = 3 * n + 1;
            }
            ptr++;    
        }
        *ptr = n; 

    } 
    else {
        // parent process
        
        shm_base = mmap(0, SIZE, PROT_READ , MAP_SHARED, shm_fd, 0);
        if (shm_base == MAP_FAILED) {
            fprintf(stderr, "Error occurred.\n");
            return 1;
        }


        wait(NULL);
        
        ptr = shm_base;
        n = *ptr; 
        
        while(n != 1) 
        {
            printf("%d, ", n);
            ptr++; 
            n = *ptr; 
        } 
        printf("%d\n", n);
    }

    shm_unlink(name);
    return 0;

}