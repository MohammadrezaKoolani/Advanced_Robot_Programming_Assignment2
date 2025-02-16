
cmake_minimum_required(VERSION 3.20)

project("generated_code")

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_EXTENSIONS OFF)

# Find requirements
find_package(fastcdr REQUIRED)
find_package(fastdds 3 REQUIRED)


message(STATUS "Configuring Obstacles...")
add_library(Obstacles_lib ../idl_files/ObstaclesTypeObjectSupport.cxx ../idl_files/ObstaclesPubSubTypes.cxx)
target_link_libraries(Obstacles_lib fastcdr fastdds)

add_executable(Obstacles ../idl_files/ObstaclesApplication.cxx ../idl_files/ObstaclesPublisherApp.cxx ../idl_files/ObstaclesSubscriberApp.cxx ../idl_files/Obstaclesmain.cxx)
target_link_libraries(Obstacles fastcdr fastdds
        Obstacles_lib 
        )



