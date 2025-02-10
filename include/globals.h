#ifndef GLOBALS_H
#define GLOBALS_H

#include "fastdds/dds/domain/DomainParticipant.hpp"
#include "fastdds/dds/publisher/Publisher.hpp"
#include "fastdds/dds/publisher/DataWriter.hpp"

using namespace eprosima::fastdds::dds;

extern DomainParticipant* participant;  // Declare as extern
extern Publisher* publisher;
extern DataWriter* writer;

#endif
