#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define NUM_THREADS 10

void *generate(void* num);

unsigned int in_circle[NUM_THREADS] = {0};
unsigned int points_each_threads[NUM_THREADS];

int main(int argc, char *argv[])
{
    pthread_t thread[NUM_THREADS];

    if (argc != 2 || atoi(argv[1]) <= 0){
        fprintf(stderr, "Please enter an positive integer.\n" );
        return 1;
    }

    unsigned int total_points = atol(argv[1]);

    int tmp = 0;
    for (int i = 0; i < NUM_THREADS; i++){
        if (i != NUM_THREADS - 1){
            points_each_threads[i] = total_points / NUM_THREADS;
            tmp += total_points / NUM_THREADS;
        }
        else{
            points_each_threads[i] = total_points - tmp;
        }
    }

    int thread_idx[NUM_THREADS];

    for (int i = 0; i < NUM_THREADS; i++){
        thread_idx[i] = i;
    }

    for (int k = 0; k < NUM_THREADS; k++){
        if (pthread_create(&(thread[k]), NULL, generate, &(thread_idx[k]))){
            fprintf(stderr,"Error in pthread_create thread");
            return 1;
        }
    } 


    for (int j = 0; j < NUM_THREADS; j++){
        pthread_join(thread[j], NULL);
    }

    unsigned int sum = 0;
    for (int l = 0; l < NUM_THREADS; l++){
        sum += in_circle[l];
    }
    double pi = 4.0 * sum / total_points;
    printf("The approximate value of pi for %d amount of points is: %.4f \n", total_points, pi);

    return 0;
}


void *generate(void* num){
    int* val = (int*)num;
    double x[1];
    double y[1];
    printf("idx %d\n", points_each_threads[*val]);

    for (unsigned int i = 0; i < points_each_threads[*val]; i++){
        *(x) = (double)rand() / RAND_MAX;
        *(y) = (double)rand() / RAND_MAX;
        
        if (pow(*x, 2) + pow(*y, 2) <= 1){
            in_circle[*val]++;
        }
    }
    pthread_exit(0);
}
