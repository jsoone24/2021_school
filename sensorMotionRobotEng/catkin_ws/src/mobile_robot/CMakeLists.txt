cmake_minimum_required(VERSION 3.0.2)
project(mobile_robot)

# add_compile_options(-std=c++11)

find_package(catkin REQUIRED COMPONENTS
  roscpp
  std_msgs
  rospy
  urdf
  tf
)

catkin_package(
  CATKIN_DEPENDS 
  roscpp
  rospy
  urdf
  tf
)

###########
## Build ##
###########

include_directories(
  include
  ${catkin_INCLUDE_DIRS}
)

add_executable(state_publisher
    src/state_publisher.cpp
)
add_dependencies(state_publisher ${${PROJECT_NAME}_EXPORTED_TARGETS} 
    ${catkin_EXPORTED_TARGETS})

target_link_libraries(state_publisher
    ${catkin_LIBRARIES}
)

add_executable(mobile_robot_odometry
    src/mobile_robot_odometry.cpp
)
add_dependencies(mobile_robot_odometry ${${PROJECT_NAME}_EXPORTED_TARGETS} 
    ${catkin_EXPORTED_TARGETS})

target_link_libraries(mobile_robot_odometry
    ${catkin_LIBRARIES}
)

