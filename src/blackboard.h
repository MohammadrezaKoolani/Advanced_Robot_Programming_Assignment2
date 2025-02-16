#ifndef BLACKBOARD_H
#define BLACKBOARD_H

#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_KEY 12345  // Shared memory key
#define SHM_SIZE sizeof(Blackboard)


typedef struct {
    int x, y;
    double velocity_x, velocity_y;
} Drone;

typedef struct {
    int drone_x;
    int drone_y;
    int obstacles[6][2];
    int targets[6][2];
    int target_ids[6];
} Blackboard;

// Declare create_blackboard function
Blackboard* create_blackboard();

// Remove unused create_shared_memory
double calculate_repulsion(int drone_x, int drone_y, int obstacle_x, int obstacle_y);
double calculate_attraction(int drone_x, int drone_y, int target_x, int target_y);
void update_drone_position(Drone* drone, double force_x, double force_y);
void update_blackboard_from_dds(Blackboard* blackboard);

#endif  // BLACKBOARD_H
