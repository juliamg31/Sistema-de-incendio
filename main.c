#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "functions.h"

#define SIZE 30  

char forest[SIZE][SIZE]; 

pthread_mutex_t matrix_mutex = PTHREAD_MUTEX_INITIALIZER; 

void initialize_forest() {
    pthread_mutex_lock(&matrix_mutex); 
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            forest[i][j] = 'T';  
        }
    }
    pthread_mutex_unlock(&matrix_mutex); 
}

void print_forest() {
    pthread_mutex_lock(&matrix_mutex);
    printf("\nEstado atual da floresta:\n");
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            printf("%c ", forest[i][j]);
        }
        printf("\n");
    }
    pthread_mutex_unlock(&matrix_mutex);
}

int main() {
    initialize_forest();

    pthread_t sensor_threads[SIZE][SIZE];
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            pthread_create(&sensor_threads[i][j], NULL, sensor_node, (void *) &forest[i][j]);
        }
    }

    pthread_t fire_thread;
    pthread_create(&fire_thread, NULL, generate_fire, NULL);


    pthread_t control_thread;
    pthread_create(&control_thread, NULL, control_center, NULL);


    while (1) {
        print_forest();
        sleep(2);
    }

    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            pthread_join(sensor_threads[i][j], NULL);
        }
    }
    pthread_join(fire_thread, NULL);
    pthread_join(control_thread, NULL);

    return 0;
}
