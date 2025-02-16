#ifndef DDS_PUBLISHER_H
#define DDS_PUBLISHER_H

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/topic/Topic.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include "ObstaclesPubSubTypes.hpp"
#include "TargetsPubSubTypes.hpp"
#include "obstacles_targets.h"  // Include the header for Obstacle and Target

using namespace eprosima::fastdds::dds;

class DDSPublisher {
public:
    DDSPublisher();
    ~DDSPublisher();  // Destructor to clean up DDS entities
    bool init();
    void publishObstacle(const Obstacles& obstacle);
    void publishTarget(const Targets& target);
    

private:
    DomainParticipant* participant_;
    Publisher* publisher_;
    Topic* obstacles_topic_;
    Topic* targets_topic_;
    DataWriter* obstacles_writer_;
    DataWriter* targets_writer_;
    TypeSupport obstacles_type_;
    TypeSupport targets_type_;
};

#endif // DDS_PUBLISHER_H
