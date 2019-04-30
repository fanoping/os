#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>


void *average(void* num);
void *minimum(void* num);
void *maximum(void* num);

int ave;
int min;
int max;

typedef struct Arr{
    size_t length;
    int* elements;
} arr;

int main(int argc, char *argv[])
{
    pthread_t thread_1, thread_2, thread_3;
    //pthread_attr_t attr;
    
    if (argc <= 1){
        fprintf(stderr, "Enter at least one integer.\n" );
        return 1;
    }

    int copy[argc-1];
    for(int i= 0; i < (argc -1); i++)
    {
        copy[i] = atoi(argv[i+1]);
    }

    arr data = {argc - 1, copy};
    
    //pthread_attr_init(&attr);
    if (pthread_create(&thread_1, NULL, average, &data)){
        fprintf(stderr,"Error in pthread_create thread_1");
        return 1;
    }
    if (pthread_create(&thread_2, NULL, maximum, &data)){
        fprintf(stderr,"Error in pthread_create thread_2");
        return 1;
    }
    if (pthread_create(&thread_3, NULL, minimum, &data)){
        fprintf(stderr,"Error in pthread_create thread_3");
        return 1;
    }

    pthread_join(thread_1, NULL);
    pthread_join(thread_2, NULL);
    pthread_join(thread_3, NULL);

    printf("The average value is %d\n", ave);
    printf("The minimum value is %d\n", min);
    printf("The maximum value is %d\n", max);

    return 0;
}


void *maximum(void* num){
    arr* val = (arr *)num;
    max = *(val->elements);

    for (int i = 0; i < val->length; i++){
        if (max < *(val->elements + i)){
            max = *(val->elements + i);
        }
    }
    pthread_exit(0);
}


void *minimum(void* num){
    arr* val = (arr *)num;
    min = *(val->elements);

    for (int i = 0; i < val->length; i++){
        if (min > *(val->elements + i)){
            min = *(val->elements + i);
        }
    }
    pthread_exit(0);
}

void *average(void* num){
    arr* val = (arr *)num;
    ave = 0;

    for (int i = 0; i < val->length; i++){
        ave += *(val->elements + i);
    }
    ave /= val->length;
    pthread_exit(0);
}
