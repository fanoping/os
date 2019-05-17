#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#define NUM_THREADS 10

void *generate(void* num);


typedef struct montecarlo
{
    unsigned long long    total_points;
    unsigned long long    current_points;
    unsigned long long    incircle_points;
    double                pi;
} monte_carlo;


typedef struct singlethread
{
    pthread_t             thread_id;
    pthread_attr_t        thread_attr;
    int                   status;
} single_thread;

pthread_mutex_t mutex;


int main(int argc, char *argv[])
{
    if (argc != 2 || atoll(argv[1]) <= 0){
        fprintf(stderr, "Please enter an positive integer.\n" );
        return 1;
    }

    // initialize threads
    single_thread threads[NUM_THREADS];

    // initialize monte carlo
    monte_carlo mc = {0, 0, 0, 0.0};

    // initialize mutex
    if (pthread_mutex_init(&mutex, NULL)){
        printf("Fail to initialize mutex lock.\n");
        return 1;
    }

    mc.total_points = atoll(argv[1]);

    printf("Using %d threads...\n", NUM_THREADS);


    for (int k = 0; k < NUM_THREADS; k++){
        threads[k].status = 0;
        pthread_attr_init(&(threads[k].thread_attr));
        if (pthread_create(&threads[k].thread_id, &threads[k].thread_attr, generate, &(mc))){
            fprintf(stderr,"Error in pthread_create thread %d\n", k);
            threads[k].status = 1;
        }
    } 

    for (int k = 0; k < NUM_THREADS; k++){
        pthread_join(threads[k].thread_id, NULL);
    }
    printf("In circle: %8llu | Total points: %8llu | approximate pi: %.6f\n", mc.incircle_points, mc.total_points, mc.pi);
    printf("The approximate value of pi for %llu points is: %.6f\n", mc.total_points, mc.pi);

    return 0;
}


void *generate(void* mc_struct){
    

    monte_carlo* monte = (monte_carlo*)mc_struct;
    
    double x;
    double y;

    pthread_mutex_lock(&mutex);

    while(monte->current_points < monte->total_points){
        x = (double)rand() / RAND_MAX;
        y = (double)rand() / RAND_MAX;

        
        
        if (pow(x, 2) + pow(y, 2) <= 1){
            monte->incircle_points++;
        }
        monte->current_points++;
        monte->pi = 4.0 * monte->incircle_points / monte->current_points;

        if (monte->current_points % 10 == 0){
            printf("In circle: %8llu | Current points: %8llu | approximate pi: %.6f\n", monte->incircle_points, monte->current_points, monte->pi);
            printf("\033[A\033[2K");
        }
        
    }
    pthread_mutex_unlock(&mutex);
    pthread_exit(0);
}
