cmake_minimum_required(VERSION 2.8.3)
project(t2g5e6_package)

find_package(catkin REQUIRED COMPONENTS
    roscpp
    rospy
    sensor_msgs
    std_msgs

)

find_package(rviz REQUIRED)

catkin_package(
    CATKIN_DEPENDS 
    sensor_msgs
    std_msgs 
)

include_directories(
    include
    ${catkin_INCLUDE_DIRS}
)

link_directories(${GAZEBO_LIBRARY_DIRS})
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${GAZEBO_CXX_FLAGS}")

# Declare a C++ library
add_library(${PROJECT_NAME}
    src/t2g5e6
)

add_dependencies(${PROJECT_NAME} sensor_msgs geometry_msgs)

target_link_libraries(${PROJECT_NAME}
  ${catkin_LIBRARIES} ${EXTRA_LIBS}
)

add_executable(${PROJECT_NAME}_node src/t2g5e6)
target_link_libraries(${PROJECT_NAME}_node ${catkin_LIBRARIES})