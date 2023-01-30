cmake_minimum_required(VERSION 3.0.2)
project(my_turtlesim_controller)

add_compile_options(-O -Wall -std=c++14 -fpermissive)

find_package(catkin REQUIRED)

find_package(catkin REQUIRED COMPONENTS
        message_generation
        geometry_msgs
        std_msgs
        roscpp
        )

catkin_package(
        CATKIN_DEPENDS
        roscpp
)

#########
##BUILD##
#########

include_directories(
        include
        ${catkin_INCLUDE_DIRS}
)

add_executable(my_turtlesim_controller
        src/my_turtlesim_controller.cpp
        )

add_dependencies(my_turtlesim_controller ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})

target_link_libraries(my_turtlesim_controller
        ${catkin_LIBRARIES}
        -lstdc++
        )