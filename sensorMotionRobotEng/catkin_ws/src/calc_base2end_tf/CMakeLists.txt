cmake_minimum_required(VERSION 3.0.2)
project(calc_base2end_tf)

add_compile_options(-O -Wall -std=c++14 -fpermissive)

find_package(catkin REQUIRED)

find_package(catkin REQUIRED COMPONENTS
  std_msgs
  roscpp
)

catkin_package(
  CATKIN_DEPENDS 
  roscpp
)

###########
## Build ##
###########

include_directories(
    include
    ${catkin_INCLUDE_DIRS}
)

add_executable(calc_base2end_tf
    src/calc_base2end_tf.cpp
)
add_dependencies(calc_base2end_tf ${${PROJECT_NAME}_EXPORTED_TARGETS} ${catkin_EXPORTED_TARGETS})
target_link_libraries(calc_base2end_tf
    ${catkin_LIBRARIES}
)