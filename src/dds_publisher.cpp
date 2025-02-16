#include "dds_publisher.h"
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/core/detail/DDSReturnCode.hpp>
#include <iostream>

using namespace eprosima::fastdds::dds;

DDSPublisher::DDSPublisher()
    : participant_(nullptr), publisher_(nullptr),
      obstacles_topic_(nullptr), targets_topic_(nullptr),
      obstacles_writer_(nullptr), targets_writer_(nullptr),
      obstacles_type_(new ObstaclesPubSubType()),
      targets_type_(new TargetsPubSubType()) {
    std::cout << "[DEBUG] DDSPublisher constructor called.\n";
}

DDSPublisher::~DDSPublisher() {
    std::cout << "[DEBUG] DDSPublisher destructor called.\n";
    if (publisher_) {
        if (obstacles_writer_) publisher_->delete_datawriter(obstacles_writer_);
        if (targets_writer_) publisher_->delete_datawriter(targets_writer_);
        participant_->delete_publisher(publisher_);
    }
    if (participant_) {
        DomainParticipantFactory::get_instance()->delete_participant(participant_);
    }
}

bool DDSPublisher::init() {
    std::cout << "[DEBUG] Initializing DDS Publisher.\n";
    
    participant_ = DomainParticipantFactory::get_instance()->create_participant(0, PARTICIPANT_QOS_DEFAULT);
    if (!participant_) {
        std::cerr << "[ERROR] Failed to create DomainParticipant.\n";
        return false;
    }

    publisher_ = participant_->create_publisher(PUBLISHER_QOS_DEFAULT, nullptr, StatusMask::all());
    if (!publisher_) {
        std::cerr << "[ERROR] Failed to create Publisher.\n";
        return false;
    }

    if (obstacles_type_.register_type(participant_) != RETCODE_OK) {
        std::cerr << "[ERROR] Obstacles type registration failed.\n";
        return false;
    }
    if (targets_type_.register_type(participant_) != RETCODE_OK) {
        std::cerr << "[ERROR] Targets type registration failed.\n";
        return false;
    }

    obstacles_topic_ = participant_->create_topic("ObstaclesTopic", obstacles_type_.get_type_name(), TOPIC_QOS_DEFAULT, nullptr, StatusMask::all());
    if (!obstacles_topic_) {
        std::cerr << "[ERROR] Failed to create ObstaclesTopic.\n";
        return false;
    }

    targets_topic_ = participant_->create_topic("TargetsTopic", targets_type_.get_type_name(), TOPIC_QOS_DEFAULT, nullptr, StatusMask::all());
    if (!targets_topic_) {
        std::cerr << "[ERROR] Failed to create TargetsTopic.\n";
        return false;
    }

    DataWriterQos writer_qos = DATAWRITER_QOS_DEFAULT;
    writer_qos.reliability().kind = RELIABLE_RELIABILITY_QOS;
    writer_qos.durability().kind = TRANSIENT_LOCAL_DURABILITY_QOS;

    obstacles_writer_ = publisher_->create_datawriter(obstacles_topic_, writer_qos, nullptr, StatusMask::all());
    if (!obstacles_writer_) {
        std::cerr << "[ERROR] Failed to create DataWriter for Obstacles.\n";
        return false;
    }

    targets_writer_ = publisher_->create_datawriter(targets_topic_, writer_qos, nullptr, StatusMask::all());
    if (!targets_writer_) {
        std::cerr << "[ERROR] Failed to create DataWriter for Targets.\n";
        return false;
    }

    std::cout << "[DEBUG] DDS Publisher initialized successfully.\n";
    return true;
}

void DDSPublisher::publishObstacle(const Obstacles& obstacle) {
    std::cout << "[DEBUG] Publishing Obstacles: Count = " << obstacle.obstacles_number() << "\n";
    if (obstacles_writer_->write((void*)&obstacle) != RETCODE_OK) {
        std::cerr << "[ERROR] Failed to write Obstacle data.\n";
    } else {
        std::cout << "[DEBUG] Obstacle data published successfully.\n";
    }
}

void DDSPublisher::publishTarget(const Targets& target) {
    std::cout << "[DEBUG] Publishing Targets: Count = " << target.targets_number() << "\n";
    if (targets_writer_->write((void*)&target) != RETCODE_OK) {
        std::cerr << "[ERROR] Failed to write Target data.\n";
    } else {
        std::cout << "[DEBUG] Target data published successfully.\n";
    }
}
