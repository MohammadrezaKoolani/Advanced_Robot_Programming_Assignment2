#include <math.h>
#include "../include/blackboard.h"
#include "../include/globals.h"
#include "../include/dds_handler.h"

// Update the drone's position based on forces
void update_drone_position(Drone* drone, double force_x, double force_y) {
    double mass = 1.0;
    double viscous_friction = 1.0;

    // Update velocities based on applied forces
    drone->velocity_x += force_x / mass;
    drone->velocity_y += force_y / mass;

    // Update position based on updated velocities
    drone->x += (int)drone->velocity_x;
    drone->y += (int)drone->velocity_y;

    // Apply viscous friction to slow down the drone
    drone->velocity_x *= (1 - viscous_friction);
    drone->velocity_y *= (1 - viscous_friction);

    // Publish the updated drone position via DDS
    publish_drone_position(drone);
}
