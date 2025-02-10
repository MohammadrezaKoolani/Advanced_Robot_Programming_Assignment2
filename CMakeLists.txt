
cmake_minimum_required(VERSION 2.8.12)

project("generated_code")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find requirements
find_package(fastcdr REQUIRED)
find_package(fastrtps REQUIRED)


message(STATUS "Configuring Obstacles...")
add_library(Obstacles_lib Obstacles.cxx)
target_link_libraries(Obstacles_lib fastcdr fastrtps)

add_executable(Obstacles ObstaclesPubSubTypes.cxx ObstaclesPublisher.cxx ObstaclesSubscriber.cxx ObstaclesPubSubMain.cxx)
target_link_libraries(Obstacles fastcdr fastrtps
        Obstacles_lib )



