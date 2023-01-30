//
// Created by jongsoo on 21. 3. 28..
//

#ifndef SRC_RANDOM_TURTLESIM_CONTROLLER_HPP
#define SRC_RANDOM_TURTLESIM_CONTROLLER_HPP

#include "ros/ros.h"
#include <iostream>
#include <std_msgs/Float64.h>
#include <cstdlib>
#include <ctime>
#include <random_gen_x/control_msg.h>
#include <geometry_msgs/Twist.h>

namespace random_turtlesim_controller {
    class RandomTurtlesimController {
    public:
        RandomTurtlesimController();

        ~RandomTurtlesimController();

        void callbackPrevMsg(const random_gen_x::control_msg::ConstPtr &ptr);

        float genYaw();

    private:
        ros::NodeHandle nh;
        ros::Publisher pub_cmd_vel;
        ros::Subscriber sub_prev_msg_;
    };
}

#endif //SRC_RANDOM_TURTLESIM_CONTROLLER_HPP
