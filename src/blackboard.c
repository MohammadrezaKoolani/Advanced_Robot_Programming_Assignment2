#define _POSIX_C_SOURCE 200809L
#include <ncurses.h>
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include "blackboard.h"
#include "dds_subscriber.h"

static DDSSubscriber* subscriber = NULL;

Blackboard* create_blackboard() {
    int shmid = shmget(SHM_KEY, SHM_SIZE, 0666 | IPC_CREAT);  // Create shared memory segment
    if (shmid == -1) {
        fprintf(stderr, "[ERROR] Failed to create shared memory.\n");
        exit(1);
    }

    Blackboard* blackboard = (Blackboard*)shmat(shmid, NULL, 0);  // Attach to shared memory
    if (blackboard == (void*)-1) {
        fprintf(stderr, "[ERROR] Failed to attach shared memory.\n");
        exit(1);
    }

    memset(blackboard, 0, sizeof(Blackboard));
    memset(blackboard->obstacles, -1, sizeof(blackboard->obstacles));
    memset(blackboard->targets, -1, sizeof(blackboard->targets));
    memset(blackboard->target_ids, -1, sizeof(blackboard->target_ids));

    printf("[DEBUG] Blackboard created in shared memory.\n");
    return blackboard;
}


void update_blackboard_from_dds(Blackboard* blackboard) {
    if (!subscriber) {
        subscriber = new DDSSubscriber();
        if (!subscriber->init(blackboard)) {
            fprintf(stderr, "[ERROR] Failed to initialize DDS Subscriber.\n");
            exit(1);
        }
    }

    subscriber->listen(blackboard);

    for (int i = 0; i < 6; i++) {
        printf("[DEBUG] Obstacle %d at (%d, %d)\n", i, blackboard->obstacles[i][0], blackboard->obstacles[i][1]);
        printf("[DEBUG] Target %d at (%d, %d)\n", i, blackboard->targets[i][0], blackboard->targets[i][1]);
    }
}

double calculate_repulsion(int drone_x, int drone_y, int obstacle_x, int obstacle_y) {
    double eta = 1.0;
    double rho_max = 5.0;
    double distance = sqrt(pow(drone_x - obstacle_x, 2) + pow(drone_y - obstacle_y, 2));
    if (distance > rho_max) return 0.0;
    return eta * (1.0 / distance - 1.0 / rho_max) / (distance * distance);
}

double calculate_attraction(int drone_x, int drone_y, int target_x, int target_y) {
    double xi = 0.1;
    double rho_goal = 5.0;
    double distance = sqrt(pow(drone_x - target_x, 2) + pow(drone_y - target_y, 2));
    return (distance <= rho_goal) ? 0.5 * xi * distance * distance : xi * rho_goal * (distance - 0.5 * rho_goal);
}
