#ifndef DDS_SUBSCRIBER_H
#define DDS_SUBSCRIBER_H

#include "blackboard.h"
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/topic/Topic.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include "obstacles_targets.h"
#include "ObstaclesPubSubTypes.hpp"
#include "TargetsPubSubTypes.hpp"

using namespace eprosima::fastdds::dds;

class DDSSubscriber {
public:
    DDSSubscriber();
    ~DDSSubscriber();
    bool init(Blackboard* blackboard);  // Pass shared blackboard
    void listen(Blackboard* blackboard);
    

private:
    DomainParticipant* participant_;
    Subscriber* subscriber_;
    Topic* obstacles_topic_;
    Topic* targets_topic_;
    DataReader* obstacles_reader_;
    DataReader* targets_reader_;
    TypeSupport obstacles_type_;
    TypeSupport targets_type_;
};

#endif // DDS_SUBSCRIBER_H
