#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <sys/stat.h>
#include <string.h>
#include <math.h>
#include "blackboard.h"
#include "dds_publisher.h"
#include "dds_subscriber.h"

int is_collision_with_obstacle(int next_x, int next_y, Blackboard *blackboard);
int collect_target(int x, int y, Blackboard *blackboard);

void ensure_log_directory_exists() {
    struct stat st = {0};
    if (stat("log", &st) == -1) {
        mkdir("log", 0700);
    }
}

void display_environment(Blackboard *blackboard, Drone *drone) {
    clear();
    for (int i = 0; i < 6; i++) {
        if (blackboard->obstacles[i][0] != -1 && blackboard->obstacles[i][1] != -1)
            mvaddch(blackboard->obstacles[i][1], blackboard->obstacles[i][0], 'O');
    }
    for (int i = 0; i < 6; i++) {
        if (blackboard->targets[i][0] != -1 && blackboard->targets[i][1] != -1)
            mvaddch(blackboard->targets[i][1], blackboard->targets[i][0], '1' + i);
    }
    mvaddch((int)drone->y, (int)drone->x, 'X');
    refresh();
}

int main(int argc, char *argv[]) {
    srand(time(NULL));
    if (argc != 2) {
        fprintf(stderr, "Usage: %s [publisher|subscriber|display]\n", argv[0]);
        return 1;
    }

    if (strcmp(argv[1], "publisher") == 0) {
        DDSPublisher publisher;
        if (!publisher.init()) {
            fprintf(stderr, "[ERROR] Failed to initialize DDS Publisher.\n");
            return 1;
        }

        int max_x = 80, max_y = 20;
        while (1) {
            Obstacles obstacle;
            Targets target;
            std::vector<int32_t> obs_x(6), obs_y(6), tgt_x(6), tgt_y(6);

            for (int i = 0; i < 6; ++i) {
                obs_x[i] = rand() % max_x;
                obs_y[i] = rand() % max_y;
                tgt_x[i] = rand() % max_x;
                tgt_y[i] = rand() % max_y;
            }

            obstacle.obstacles_x(obs_x);
            obstacle.obstacles_y(obs_y);
            obstacle.obstacles_number(6);
            target.targets_x(tgt_x);
            target.targets_y(tgt_y);
            target.targets_number(6);

            publisher.publishObstacle(obstacle);
            publisher.publishTarget(target);
            printf("[INFO] Published %d Obstacles and %d Targets.\n", 6, 6);
            fflush(stdout);
            sleep(30);
        }

    } else if (strcmp(argv[1], "subscriber") == 0) {
        Blackboard blackboard;
        memset(&blackboard, 0, sizeof(Blackboard));
        DDSSubscriber subscriber;
        if (!subscriber.init(&blackboard)) {
            fprintf(stderr, "[ERROR] Failed to initialize DDS Subscriber.\n");
            return 1;
        }
        while (1) {
            subscriber.listen(&blackboard);
            sleep(1);
        }

    } else if (strcmp(argv[1], "display") == 0) {
        initscr();
        cbreak();
        keypad(stdscr, TRUE);
        noecho();
        ensure_log_directory_exists();

        Blackboard *blackboard = create_blackboard();
        Drone drone = {10, 10, 0.0, 0.0};
        int ch;
        int collected_targets = 0;

        while ((ch = getch()) != 'q') {
            mvaddch((int)drone.y, (int)drone.x, ' '); 
            int next_x = drone.x, next_y = drone.y;

            if (ch == KEY_UP) next_y--;
            if (ch == KEY_DOWN) next_y++;
            if (ch == KEY_LEFT) next_x--;
            if (ch == KEY_RIGHT) next_x++;

            if (!is_collision_with_obstacle(next_x, next_y, blackboard)) {
                drone.x = next_x;
                drone.y = next_y;
            }

            if (collect_target(drone.x, drone.y, blackboard)) {
                collected_targets++;
            }

            if (collected_targets == 6) {
                collected_targets = 0;
                for (int i = 0; i < 6; i++) {
                    blackboard->obstacles[i][0] = rand() % 80;
                    blackboard->obstacles[i][1] = rand() % 20;
                    blackboard->targets[i][0] = rand() % 80;
                    blackboard->targets[i][1] = rand() % 20;
                }
            }

            display_environment(blackboard, &drone);
            usleep(100000);
        }
        endwin();
        free(blackboard);
    } else {
        fprintf(stderr, "[ERROR] Invalid mode specified.\n");
        return 1;
    }
    return 0;
}

int is_collision_with_obstacle(int next_x, int next_y, Blackboard *blackboard) {
    for (int i = 0; i < 6; i++) {
        if (next_x == blackboard->obstacles[i][0] && next_y == blackboard->obstacles[i][1]) {
            return 1;
        }
    }
    return 0;
}

int collect_target(int x, int y, Blackboard *blackboard) {
    for (int i = 0; i < 6; i++) {
        if (x == blackboard->targets[i][0] && y == blackboard->targets[i][1]) {
            blackboard->targets[i][0] = -1;
            blackboard->targets[i][1] = -1;
            return 1;
        }
    }
    return 0;
}
