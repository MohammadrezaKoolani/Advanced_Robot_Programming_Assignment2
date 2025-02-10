#include "../include/dds_handler.h"
#include "../include/ObstaclesPubSubTypes.h"
#include "../include/TargetsPubSubTypes.h"
#include "../include/blackboard.h"
#include "../include/globals.h"
#include "fastdds/dds/domain/DomainParticipantFactory.hpp"
#include "fastdds/dds/domain/DomainParticipant.hpp"
#include "fastdds/dds/publisher/Publisher.hpp"
#include "fastdds/dds/topic/Topic.hpp"
#include "fastdds/dds/publisher/DataWriter.hpp"
#include "fastdds/dds/topic/TypeSupport.hpp"
#include <fstream>  // For logging errors to a file

using namespace eprosima::fastdds::dds;

// Define DDS global variables
DomainParticipant* participant = nullptr;
Publisher* publisher = nullptr;
DataWriter* obstacle_writer = nullptr;
DataWriter* target_writer = nullptr;

// Convert ReturnCode_t to readable message
const char* return_code_to_string(ReturnCode_t code) {
    if (code == ReturnCode_t::RETCODE_OK) {
        return "RETCODE_OK";
    } else if (code == ReturnCode_t::RETCODE_ERROR) {
        return "RETCODE_ERROR";
    } else if (code == ReturnCode_t::RETCODE_BAD_PARAMETER) {
        return "RETCODE_BAD_PARAMETER";
    } else if (code == ReturnCode_t::RETCODE_PRECONDITION_NOT_MET) {
        return "RETCODE_PRECONDITION_NOT_MET";
    } else if (code == ReturnCode_t::RETCODE_OUT_OF_RESOURCES) {
        return "RETCODE_OUT_OF_RESOURCES";
    } else if (code == ReturnCode_t::RETCODE_NOT_ENABLED) {
        return "RETCODE_NOT_ENABLED";
    } else if (code == ReturnCode_t::RETCODE_IMMUTABLE_POLICY) {
        return "RETCODE_IMMUTABLE_POLICY";
    } else if (code == ReturnCode_t::RETCODE_INCONSISTENT_POLICY) {
        return "RETCODE_INCONSISTENT_POLICY";
    } else if (code == ReturnCode_t::RETCODE_ALREADY_DELETED) {
        return "RETCODE_ALREADY_DELETED";
    } else if (code == ReturnCode_t::RETCODE_TIMEOUT) {
        return "RETCODE_TIMEOUT";
    } else {
        return "UNKNOWN_ERROR";
    }
}

// Log DDS errors to a file
void log_dds_error(const std::string& message) {
    std::ofstream log_file("log/dds_error_log.txt", std::ios_base::app);
    log_file << message << std::endl;
}

// Setup DDS Publisher for Obstacles and Targets
void setup_dds_publisher() {
    participant = DomainParticipantFactory::get_instance()->create_participant(0, PARTICIPANT_QOS_DEFAULT);
    if (!participant) {
        log_dds_error("Error: Failed to create DDS participant.");
        exit(1);
    } else {
        printf("DDS participant created successfully.\n");
    }

    TypeSupport obstacle_type(new ObstaclesPubSubType());
    if (obstacle_type.register_type(participant) != ReturnCode_t::RETCODE_OK) {
        log_dds_error("Error: Failed to register Obstacles type.");
        exit(1);
    }

    TypeSupport target_type(new TargetsPubSubType());
    if (target_type.register_type(participant) != ReturnCode_t::RETCODE_OK) {
        log_dds_error("Error: Failed to register Targets type.");
        exit(1);
    }

    Topic* obstacle_topic = participant->create_topic("ObstaclesTopic", obstacle_type.get_type_name(), TOPIC_QOS_DEFAULT);
    Topic* target_topic = participant->create_topic("TargetsTopic", target_type.get_type_name(), TOPIC_QOS_DEFAULT);

    if (!obstacle_topic || !target_topic) {
        log_dds_error("Error: Failed to create DDS topics.");
        exit(1);
    }

    publisher = participant->create_publisher(PUBLISHER_QOS_DEFAULT, nullptr);
    if (!publisher) {
        log_dds_error("Error: Failed to create DDS publisher.");
        exit(1);
    }

    obstacle_writer = publisher->create_datawriter(obstacle_topic, DATAWRITER_QOS_DEFAULT, nullptr);
    if (!obstacle_writer) {
        log_dds_error("Error: Failed to create DDS data writer for Obstacles.");
        exit(1);
    }

    target_writer = publisher->create_datawriter(target_topic, DATAWRITER_QOS_DEFAULT, nullptr);
    if (!target_writer) {
        log_dds_error("Error: Failed to create DDS data writer for Targets.");
        exit(1);
    }
}

// Publish drone position as an obstacle
void publish_drone_position(Drone* drone) {
    if (!obstacle_writer) {
        log_dds_error("Error: Obstacle writer is not initialized.");
        return;
    }

    Obstacles obstacle_msg;
    obstacle_msg.obstacles_x().push_back(drone->x);
    obstacle_msg.obstacles_y().push_back(drone->y);
    obstacle_msg.obstacles_number(1);

    ReturnCode_t result = obstacle_writer->write(&obstacle_msg);
    if (result != ReturnCode_t::RETCODE_OK) {
        log_dds_error("Error: Failed to publish drone position. DDS Write Error: " + std::string(return_code_to_string(result)));
    }
}

// Publish target positions
void publish_target_position(int target_x, int target_y) {
    if (!target_writer) {
        log_dds_error("Error: Target writer is not initialized.");
        return;
    }

    Targets target_msg;
    target_msg.targets_x().push_back(target_x);
    target_msg.targets_y().push_back(target_y);
    target_msg.targets_number(1);

    ReturnCode_t result = target_writer->write(&target_msg);
    if (result != ReturnCode_t::RETCODE_OK) {
        log_dds_error("Error: Failed to publish target position. DDS Write Error: " + std::string(return_code_to_string(result)));
    }
}
