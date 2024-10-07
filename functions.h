#ifndef FUNCTIONS_H
#define FUNCTIONS_H

#define SIZE 30

extern pthread_mutex_t matrix_mutex;  

void *sensor_node(void *arg);
void *generate_fire(void *arg);
void *control_center(void *arg);
void combat_fire();

#endif
