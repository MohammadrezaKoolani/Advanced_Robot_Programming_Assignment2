#ifndef DDS_HANDLER_H
#define DDS_HANDLER_H

#include "fastdds/dds/domain/DomainParticipant.hpp"
#include "fastdds/dds/publisher/Publisher.hpp"
#include "fastdds/dds/publisher/DataWriter.hpp"
#include "../include/globals.h"
#include "../include/blackboard.h"  // <-- Added to access the Drone struct

using namespace eprosima::fastdds::dds;

// DDS global variables
extern DomainParticipant* participant;
extern Publisher* publisher;
extern DataWriter* obstacle_writer;
extern DataWriter* target_writer;

// DDS function declarations
void setup_dds_publisher();
void publish_drone_position(Drone* drone);
void publish_target_position(int target_x, int target_y);

#endif  // DDS_HANDLER_H
