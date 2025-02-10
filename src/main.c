#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>  // For rand() and srand()
#include <time.h>    // For time(NULL)
#include "../include/blackboard.h"
#include "../include/dds_handler.h"
#include "../include/globals.h"
#include "fastdds/dds/domain/DomainParticipantFactory.hpp"
#include "fastdds/dds/publisher/Publisher.hpp"
#include "fastdds/dds/topic/Topic.hpp"
#include "fastdds/dds/publisher/DataWriter.hpp"
#include "fastdds/dds/topic/TypeSupport.hpp"


// Watchdog variables
int last_movement_time = 0;
int movement_flag = 0;
const int WATCHDOG_TIMEOUT = 5;

int main() {
    srand(time(NULL));
    initscr();
    cbreak();
    keypad(stdscr, TRUE);
    noecho();

    mvprintw(0, 0, "Drone Simulator - Press 'q' to quit");
    refresh();

    // Initialize the blackboard and generate obstacles/targets
    Blackboard* blackboard = create_shared_memory();
    generate_obstacles(blackboard, 10, COLS, LINES);
    generate_targets(blackboard, 5, COLS, LINES);

    // Initialize the drone
    Drone drone = {blackboard->drone_x, blackboard->drone_y, 0.0, 0.0};

    // Set up DDS for publishing
    setup_dds_publisher();

    // Verify DDS setup
    if (!participant || !publisher || !obstacle_writer || !target_writer) {
        fprintf(stderr, "Failed to initialize DDS components\n");
        exit(1);
    }

    int ch;
    while ((ch = getch()) != 'q') {
        mvaddch(drone.y, drone.x, ' ');

        // Handle drone movement with keyboard input
        if (ch == KEY_UP) drone.y--;
        if (ch == KEY_DOWN) drone.y++;
        if (ch == KEY_LEFT) drone.x--;
        if (ch == KEY_RIGHT) drone.x++;

        // Reset the watchdog timer if movement occurs
        if (ch == KEY_UP || ch == KEY_DOWN || ch == KEY_LEFT || ch == KEY_RIGHT) {
            movement_flag = 1;
            last_movement_time = time(NULL);
        }

        double total_force_x = 0.0;
        double total_force_y = 0.0;

        // Apply repulsion from obstacles
        for (int i = 0; i < 10; i++) {
            double force = calculate_repulsion(drone.x, drone.y, blackboard->obstacles[i][0], blackboard->obstacles[i][1]);
            total_force_x += force * (drone.x - blackboard->obstacles[i][0]);
            total_force_y += force * (drone.y - blackboard->obstacles[i][1]);
        }

        // Apply attraction to targets
        for (int i = 0; i < 5; i++) {
            double force = calculate_attraction(drone.x, drone.y, blackboard->targets[i][0], blackboard->targets[i][1]);
            total_force_x -= force * (drone.x - blackboard->targets[i][0]);
            total_force_y -= force * (drone.y - blackboard->targets[i][1]);
        }

        // Update the drone's position based on the computed forces
        update_drone_position(&drone, total_force_x, total_force_y);

        // Log the drone's new position
        FILE* logfile = fopen("log/drone_log.txt", "a");
        fprintf(logfile, "Drone moved to position (%d, %d)\n", drone.x, drone.y);
        fclose(logfile);

        // Watchdog timer to exit if no movement is detected
        if (time(NULL) - last_movement_time > WATCHDOG_TIMEOUT && movement_flag == 0) {
            logfile = fopen("log/watchdog_log.txt", "a");
            fprintf(logfile, "Watchdog: No movement detected. Exiting...\n");
            fclose(logfile);
            mvprintw(LINES - 1, 0, "Watchdog timeout! Exiting simulation.");
            refresh();
            sleep(2);
            endwin();
            exit(0);
        }

        // Publish the drone's position over DDS
        publish_drone_position(&drone);

        // Visual update of the drone's new position
        mvaddch(drone.y, drone.x, 'X');
        refresh();
    }

    // Clean up the simulation window
    endwin();
    return 0;
}