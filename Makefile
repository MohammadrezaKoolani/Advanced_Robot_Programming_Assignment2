CXX = g++
CXXFLAGS = -Wall -std=c++11 \
           -I/usr/local/include -I/usr/include/fastdds -I/usr/include/fastcdr \
           -I/usr/local/include/fastdds/dds/core/detail -I./src
LDFLAGS = -L/usr/local/lib -lfastdds -lfastcdr -lpthread -lncurses

SRC = src/main.c src/blackboard.c src/drone.c \
      src/ObstaclesPubSubTypes.cxx src/TargetsPubSubTypes.cxx \
      src/ObstaclesTypeObjectSupport.cxx src/TargetsTypeObjectSupport.cxx \
      src/dds_publisher.cpp src/dds_subscriber.cpp src/globals.c

OBJ = $(patsubst %.c, %.o, $(patsubst %.cpp, %.o, $(patsubst %.cxx, %.o, $(SRC))))

EXEC = drone_simulator

all: $(EXEC)

$(EXEC): $(OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)

%.o: %.c
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.cxx
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f src/*.o $(EXEC)
