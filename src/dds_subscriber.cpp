#include "dds_subscriber.h"
#include "blackboard.h"
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include <fastdds/dds/core/detail/DDSReturnCode.hpp>
#include <iostream>
#include <cstring>

using namespace eprosima::fastdds::dds;

class CustomSubscriberListener : public DataReaderListener {
public:
    Blackboard* blackboard_;

    CustomSubscriberListener(Blackboard* bb) : blackboard_(bb) {
        std::cout << "[DEBUG] CustomSubscriberListener initialized.\n";
        memset(blackboard_->obstacles, -1, sizeof(blackboard_->obstacles));
        memset(blackboard_->targets, -1, sizeof(blackboard_->targets));
    }

    void on_data_available(DataReader* reader) override {
        if (!reader) {
            std::cerr << "[ERROR] Reader is null.\n";
            return;
        }

        SampleInfo info;
        std::string topic_name = reader->get_topicdescription()->get_name();
        std::cout << "[DEBUG] Data available for topic: " << topic_name << "\n";

        if (topic_name == "ObstaclesTopic") {
            Obstacles obstacles_data;
            while (reader->take_next_sample(&obstacles_data, &info) == RETCODE_OK) {
                if (info.valid_data) {
                    std::cout << "[DEBUG] Received Obstacles Count: " << obstacles_data.obstacles_number() << "\n";
                    for (int i = 0; i < 6; ++i) {
                        blackboard_->obstacles[i][0] = obstacles_data.obstacles_x()[i];
                        blackboard_->obstacles[i][1] = obstacles_data.obstacles_y()[i];
                        std::cout << "[DEBUG] Obstacle " << i << ": ("
                                  << blackboard_->obstacles[i][0] << ", " << blackboard_->obstacles[i][1] << ")\n";
                    }
                } else {
                    std::cerr << "[ERROR] Invalid Obstacle data.\n";
                }
            }
        } else if (topic_name == "TargetsTopic") {
            Targets targets_data;
            while (reader->take_next_sample(&targets_data, &info) == RETCODE_OK) {
                if (info.valid_data) {
                    std::cout << "[DEBUG] Received Targets Count: " << targets_data.targets_number() << "\n";
                    for (int i = 0; i < 6; ++i) {
                        blackboard_->targets[i][0] = targets_data.targets_x()[i];
                        blackboard_->targets[i][1] = targets_data.targets_y()[i];
                        blackboard_->target_ids[i] = i + 1;
                        std::cout << "[DEBUG] Target " << i << ": ("
                                << blackboard_->targets[i][0] << ", " << blackboard_->targets[i][1]
                                 << ", ID=" << blackboard_->target_ids[i] << ")\n";
                    }
                } else {
                    std::cerr << "[ERROR] Invalid Target data.\n";
                }
            }
        } else {
            std::cerr << "[ERROR] Unknown topic: " << topic_name << "\n";
        }
    }
};

DDSSubscriber::DDSSubscriber()
    : participant_(nullptr), subscriber_(nullptr),
      obstacles_topic_(nullptr), targets_topic_(nullptr),
      obstacles_reader_(nullptr), targets_reader_(nullptr),
      obstacles_type_(new ObstaclesPubSubType()),
      targets_type_(new TargetsPubSubType()) {

}

void DDSSubscriber::listen(Blackboard* blackboard) {
    std::cout << "[DEBUG] Listening for DDS data...\n";
    
    while (true) {
        if (obstacles_reader_ != nullptr) {
            obstacles_reader_->wait_for_unread_message({5, 0});  // Wait up to 5 seconds
        }
        if (targets_reader_ != nullptr) {
            targets_reader_->wait_for_unread_message({5, 0});
        }
        sleep(1);
    }
}



bool DDSSubscriber::init(Blackboard* blackboard) {
    std::cout << "[DEBUG] Initializing DDS Subscriber.\n";

    // Attach to existing shared memory blackboard
    int shmid = shmget(SHM_KEY, SHM_SIZE, 0666 | IPC_CREAT);
    if (shmid == -1) {
        std::cerr << "[ERROR] Failed to attach to shared memory.\n";
        return false;
    }
    blackboard = (Blackboard*)shmat(shmid, NULL, 0);
    if (blackboard == (void*)-1) {
        std::cerr << "[ERROR] Failed to attach shared memory.\n";
        return false;
    }

    participant_ = DomainParticipantFactory::get_instance()->create_participant(0, PARTICIPANT_QOS_DEFAULT);
    if (!participant_) {
        std::cerr << "[ERROR] Failed to create DomainParticipant.\n";
        return false;
    }

    subscriber_ = participant_->create_subscriber(SUBSCRIBER_QOS_DEFAULT, nullptr, StatusMask::all());
    if (!subscriber_) {
        std::cerr << "[ERROR] Failed to create Subscriber.\n";
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

    DataReaderQos reader_qos = DATAREADER_QOS_DEFAULT;
    reader_qos.reliability().kind = RELIABLE_RELIABILITY_QOS;
    reader_qos.durability().kind = TRANSIENT_LOCAL_DURABILITY_QOS;

    CustomSubscriberListener* listener = new CustomSubscriberListener(blackboard);

    obstacles_reader_ = subscriber_->create_datareader(obstacles_topic_, reader_qos, listener, StatusMask::all());
    if (!obstacles_reader_) {
        std::cerr << "[ERROR] Failed to create DataReader for Obstacles.\n";
        return false;
    }

    targets_reader_ = subscriber_->create_datareader(targets_topic_, reader_qos, listener, StatusMask::all());
    if (!targets_reader_) {
        std::cerr << "[ERROR] Failed to create DataReader for Targets.\n";
        return false;
    }

    std::cout << "[DEBUG] DDS Subscriber initialized successfully.\n";
    return true;
}

DDSSubscriber::~DDSSubscriber() {
    std::cout << "[DEBUG] DDSSubscriber destructor called.\n";
    if (subscriber_) {
        if (obstacles_reader_) subscriber_->delete_datareader(obstacles_reader_);
        if (targets_reader_) subscriber_->delete_datareader(targets_reader_);
        participant_->delete_subscriber(subscriber_);
    }
    if (participant_) {
        DomainParticipantFactory::get_instance()->delete_participant(participant_);
    }
}
