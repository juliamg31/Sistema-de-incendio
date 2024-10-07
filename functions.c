#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include "functions.h"

extern char forest[SIZE][SIZE];
extern pthread_mutex_t matrix_mutex;  
pthread_cond_t fire_cond = PTHREAD_COND_INITIALIZER;  
pthread_mutex_t fire_mutex = PTHREAD_MUTEX_INITIALIZER;  

void *sensor_node(void *arg) {
    char *cell = (char *)arg;
    while (1) {
        pthread_mutex_lock(&matrix_mutex);
        if (*cell == '@') {  // Se houver fogo na célula
            printf("Incêndio detectado!\n");
            pthread_mutex_lock(&fire_mutex);
            pthread_cond_signal(&fire_cond);  // Notificar a central de controle
            pthread_mutex_unlock(&fire_mutex);
        }
        pthread_mutex_unlock(&matrix_mutex);
        sleep(1);
    }
}

void *generate_fire(void *arg) {
    while (1) {
        pthread_mutex_lock(&matrix_mutex);  
        int x = rand() % SIZE; 
        int y = rand() % SIZE;

        if (forest[x][y] == 'T') {
            forest[x][y] = '@';  
            printf("Fogo iniciado na posição (%d, %d)\n", x, y);  
        } else {
            printf("Tentativa de iniciar fogo em uma célula ocupada na posição (%d, %d)\n", x, y);
        }

        pthread_mutex_unlock(&matrix_mutex); 
        sleep(3);  
    }
}
void *control_center(void *arg) {
    while (1) {
        pthread_mutex_lock(&fire_mutex);
        pthread_cond_wait(&fire_cond, &fire_mutex); 
        printf("Central de controle notificada de incêndio. Combatendo fogo!\n");
        combat_fire();
        pthread_mutex_unlock(&fire_mutex);
    }
}

void combat_fire() {
    pthread_mutex_lock(&matrix_mutex);
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            if (forest[i][j] == '@') {
                forest[i][j] = '/'; 
            }
        }
    }
    pthread_mutex_unlock(&matrix_mutex);
}
