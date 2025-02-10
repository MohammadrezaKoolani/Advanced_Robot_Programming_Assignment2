# Compiler settings
CC = gcc
CXX = g++
CFLAGS = -Wall -Wextra -g -std=c11 -I./include
CXXFLAGS=-Wall -Wextra -g -std=c++11 -I./include \
         -I/usr/include/fastdds \
         -I/usr/include/fastrtps \
         -I/usr/local/include/fastdds \
         -I/usr/local/include/fastrtps

LDFLAGS = -lncurses -lm -lfastcdr -lfastrtps

# Directories
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# Source files
C_SRCS = $(SRC_DIR)/blackboard.c \
         $(SRC_DIR)/drone.c

CXX_SRCS = $(SRC_DIR)/Obstacles.cxx \
           $(SRC_DIR)/ObstaclesPubSubTypes.cxx \
           $(SRC_DIR)/Targets.cxx \
           $(SRC_DIR)/TargetsPubSubTypes.cxx \
           $(SRC_DIR)/dds_handler.cpp \
           $(SRC_DIR)/main.c


# Object files
C_OBJS = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_SRCS))
CXX_OBJS = $(patsubst $(SRC_DIR)/%.cxx, $(BUILD_DIR)/%.o, $(CXX_SRCS))

# Target executable
TARGET = $(BIN_DIR)/drone_simulator_v2

# Default target
all: $(TARGET)

# Linking target
$(TARGET): $(C_OBJS) $(CXX_OBJS)
	@mkdir -p $(BIN_DIR)
	$(CXX) -o $@ $^ $(LDFLAGS)


# Compile C files with g++ if they include C++ headers
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Compile C++ files
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cxx
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
