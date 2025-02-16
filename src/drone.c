#include <math.h>
#include <stdio.h>
#include "blackboard.h"  // Include blackboard.h for the drone structure and functions

// Function to update the drone's position based on forces
void update_drone_position(Drone* drone, double force_x, double force_y) {
    double mass = 1.0;                // Drone mass (kg)
    double viscous_friction = 0.1;    // Drone viscous friction coefficient

    // Update velocity using forces and mass
    drone->velocity_x += force_x / mass;
    drone->velocity_y += force_y / mass;

    // Update position based on velocity
    drone->x += (int)round(drone->velocity_x);
    drone->y += (int)round(drone->velocity_y);

    // Apply viscous friction (air resistance)
    drone->velocity_x *= (1.0 - viscous_friction);
    drone->velocity_y *= (1.0 - viscous_friction);
}
